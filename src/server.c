#include "mongoose.h"   // To build, run: cc main.c mongoose.c
#include <stdio.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include "defer.h"

struct curl_response {
	char *data;
	size_t size;
};

typedef struct {
  char *client_id, *client_secret;
  CURL *curl;
  int done;
} HTTPServerData;

char *base64_encode(char *str) {
  char cmd[1024];
  sprintf(cmd, "echo -n '%s' | base64 -w 0", str);

  FILE *file = popen(cmd, "r");
  if (!file) return NULL;
  defer { pclose(file); }

  char *result = malloc(1024); // free this!
  fgets(result, 1024, file);

  result[strcspn(result, "\n")] = 0;
  return result;
}

size_t write_callback(void *content, size_t size, size_t nmemb, struct curl_response *res) {
  size_t real_size = size * nmemb;

  res->data = realloc(res->data, res->size + real_size + 1);
  if (!res->data) {
	printf("could not realloc curl res, buy more fucking ram!!1\n");
	return 0;
  }

  memcpy(res->data + res->size, content, real_size);
  res->size += real_size;
  res->data[res->size] = 0;

  return real_size;
}

// HTTP server event handler function
void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev != MG_EV_HTTP_MSG) return;

  HTTPServerData *http_data = (HTTPServerData*)c->fn_data;
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;
  
  const char *response_body = "close this window bitch";
  mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", response_body);

  char code[512];
  mg_http_get_var(&hm->query, "code", code, 512);
  printf("code: %s\n", code);

  CURL *curl = http_data->curl;

  curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
  char post_data[512];
  sprintf(post_data, "code=%s&redirect_uri=http://localhost:5500/callback&grant_type=authorization_code", code);
  printf("post_data: %s\n", post_data);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
  defer { curl_slist_free_all(headers); }

  char auth_raw[512];
  sprintf(auth_raw, "%s:%s", http_data->client_id, http_data->client_secret);
  printf("auth_raw: %s\n", auth_raw);
  char *auth_encoded = base64_encode(auth_raw);
  defer { free(auth_encoded); }
  printf("auth_encoded: %s\n", auth_encoded);
  char auth_header[1024];
  sprintf(auth_header, "Authorization: Basic %s", auth_encoded);
  printf("auth_header: %s\n", auth_header);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  struct curl_response res = {0};
  defer { free(res.data); }
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);

  CURLcode res_code = curl_easy_perform(curl);
  if (res_code != CURLE_OK) {
	  printf("perform failed: %s\n", curl_easy_strerror(res_code));
	  return;
	}
  defer { curl_easy_cleanup(curl); }

  printf("res: %s\n", res.data);

  http_data->done = 1;
}

// make sure to free!
char *read_file(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buf = malloc(size);
  fread(buf, 1, size, file);

  return buf;
}

int main(void) {
  FILE *config_file = fopen("config.json", "r");
  defer { fclose(config_file); }

  if (!config_file) {
	printf("no config.json found\n");
	return 1;
  }

  char *config_buf = read_file(config_file);
  defer { free(config_buf); }

  json_object *config_obj = json_tokener_parse(config_buf);
  if (!config_obj) {
	printf("could not parse config\n");
	return 1;
  }
  defer { json_object_put(config_obj); }

  json_object *client_id_obj;
  json_object_object_get_ex(config_obj, "client_id", &client_id_obj);
  if (!client_id_obj) {
	printf("could not get client_id\n");
	return 1;
  }
  const char *client_id = json_object_get_string(client_id_obj);
  // printf("client_id: %s\n", client_id);

  json_object *client_secret_obj;
  json_object_object_get_ex(config_obj, "client_secret", &client_secret_obj);
  if (!client_secret_obj) {
	printf("could not get client_secret\n");
	return 1;
  }
  const char *client_secret = json_object_get_string(client_secret_obj);
  // printf("client_secret: %s\n", client_secret);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL *curl = curl_easy_init(); // if !curl check
  defer { curl_global_cleanup(); }

  HTTPServerData http_data = {client_id, client_secret, curl, 0};

  struct mg_mgr mgr;  // Declare event manager
  mg_mgr_init(&mgr);  // Initialise event manager
  defer { mg_mgr_free(&mgr); }

  mg_http_listen(&mgr, "http://0.0.0.0:5500", ev_handler, (void*)&http_data);  // Setup listener
  system("xdg-open \"https://accounts.spotify.com/authorize?response_type=code&client_id=0c16859b18244ff68b680e5ab82936f6&scope=user-read-playback-state+user-modify-playback-state+user-read-currently-playing+app-remote-control+streaming&redirect_uri=http%3A%2F%2Flocalhost%3A5500%2Fcallback\"");
  while (1) {          // Run an infinite event loop
	if (http_data.done) break;
	  mg_mgr_poll(&mgr, 1000);
  }

  return 0;
}
