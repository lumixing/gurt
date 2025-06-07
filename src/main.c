#define _GNU_SOURCE // for pthread_tryjoin_np
#define HTTPSERVER_IMPL

#include "log.h"
#include <ncurses.h>
#include <curl/curl.h>
#include <pthread.h>
#include "str.h"
#include <unistd.h>
#include <string.h>

#define TUI_TIMEOUT 16 // 60fps
#define HTTP_MAX_RESPONSE_SIZE 10000

typedef struct {
	bool is_running;
	CURL *curl;
	pthread_t http_thr;
	bool http_is_thr;
	str *ip;
	pthread_t fifo_thr;
	char *fifo_str;
} State;

static size_t http_write_callback(void *content, size_t size, size_t nmemb, str *res) {
	size_t real_size = size * nmemb;
	log("str %zu %zu", res->len, res->cap);

	if (res->len + real_size >= HTTP_MAX_RESPONSE_SIZE) {
		log("error: buffer overflow protection (%zu, %zu)\n", res->len, real_size);
		return 0;
	}

	*res = str_new_size(real_size);
	res->len = real_size;
	memcpy(res->buf, content, res->len);

	return real_size;
}

void *http_thr_func(void *arg) {
	log("starting http_thr_func");

	CURL *curl = (CURL*)arg;
	str *res = (str*)malloc(sizeof(str));
	*res = (str){0};
	if (!res) {
		log("error: could not malloc str");
		return NULL;
	}
	log("str %zu %zu", res->len, res->cap);

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org/?format=json");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

	CURLcode res_code = curl_easy_perform(curl);

	if (res_code != CURLE_OK) {
		log("error: res_code is not ok: %d", res_code);
		return NULL;
	}

	log("returning http_thr_func with (%zu) %s", res->len, res->buf);

	return (void*)res;
}

void *fifo_thr_func(void *arg) {
	log("starting fifo_thr_func");

	char **fifo_str = (char**)arg;

	char buf[256];
	bool running = true;

	while (running) {
		FILE *fifo_file = fopen("/tmp/gurtsend", "r");
		while (fgets(buf, sizeof(buf), fifo_file)) {
			buf[strcspn(buf, "\n")] = '\0';
			log("received fifo: %s", buf);
			*fifo_str = buf;
			if (strcmp(buf, "exit") == 0)
				running = false;
		}
		fclose(fifo_file);
		usleep(100000);
	}

	log("returning fifo_thr_func");

	return NULL;
}

int main() {
	if (!log_init("log.txt")) {
		printf("could not init log file\n");
		return 1;
	}
	log("hello log");

	str_init_arena(1024);

	State state = {0};
	state.is_running = true;
	
	state.curl = curl_easy_init();
	if (!state.curl) {
		printf("could not init curl\n");
		return 1;
	}

	state.http_is_thr = false;
	state.ip = NULL;

	state.fifo_str = calloc(1, 256);

	int fifo_thr_res = pthread_create(&state.fifo_thr, NULL, fifo_thr_func, &state.fifo_str);
	if (fifo_thr_res) {
		log("error: could not create fifo thread");
		state.is_running = false;
	}

	initscr();
	log("init screen");

	while (state.is_running) {
		// int fifo_thr_join_res = pthread_tryjoin_np(state.fifo_thr, NULL);

		if (state.http_is_thr) {
			void *http_thr_arg;
			int http_thr_join_res = pthread_tryjoin_np(state.http_thr, &http_thr_arg);
			
			if (http_thr_join_res == 0) {
				state.ip = (str*)http_thr_arg;
				log("http thr joined: %s", str_cstr(*state.ip));
				state.http_is_thr = false;
			}
		}

		clear();

		printw("hello bitches!!1 (%d)\n", rand());
		if (state.ip) {
			printw("ur ip: %s\n", str_cstr(*state.ip));
		} else {
			if (state.http_is_thr)
				printw("leaking ip..\n");
			else
				printw("press e to leak ur ip\n");
		}

		if (!state.fifo_str[0]) {
			printw("waiting for fifo..\n");
		} else {
			printw("fifo: %s\n", state.fifo_str);
		}

		refresh();
		timeout(TUI_TIMEOUT);

		int ch = getch();
		switch (ch) {
		case 'q':
			state.is_running = false;
			break;
		case 'e':;
			int http_thr_res = pthread_create(&state.http_thr, NULL, http_thr_func, state.curl);
			if (http_thr_res) {
				log("error: could not create http thread");
				state.is_running = false;
			}
			state.http_is_thr = true;
			break;
		}

		str_clear_cstr_arena();
	}

	endwin();
	log("ended screen");

	curl_global_cleanup();

	free(state.fifo_str);
	free(state.ip);

	str_free_arena();
	log_free();

	return 0;
}
