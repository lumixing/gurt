#define _GNU_SOURCE // for pthread_tryjoin_np
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
// #include <unistd.h>

#define MAX_RESPONSE_SIZE 10000

typedef struct {
	char *buf;
	size_t len;
} string;

#define panic(fmt, ...) do { \
	endwin(); \
	curl_global_cleanup(); \
    printf("panic(%s:%d): " fmt "\n", \
            __FILE__, __LINE__, ##__VA_ARGS__); \
    exit(1); \
} while(0)

static size_t write_callback(void *content, size_t size, size_t nmemb, string *res) {
	size_t real_size = size * nmemb;

	if (res->len + real_size >= MAX_RESPONSE_SIZE) {
		printf("error: buffer overflow protection (%zu, %zu)\n", res->len, real_size);
		return 0;
	}

	res->buf = malloc(MAX_RESPONSE_SIZE);
	memcpy(&(res->buf[res->len]), content, real_size);
	res->len += real_size;
	res->buf[res->len] = '\0';

	return real_size;
}

void *thread_func(void *arg) {
	printf("starting thread func\n");

	CURL *curl = (CURL*)arg;
	string *res = malloc(sizeof(string));

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org/?format=json");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

	CURLcode res_code = curl_easy_perform(curl);

	if (res_code != CURLE_OK) {
		panic("res_code is not ok: %d\n", res_code);
	}

	printf("returning thread func %s\n", res->buf);

	return (void*)res;
}

int main() {
	curl_global_init(CURL_GLOBAL_DEFAULT);

	CURL *curl = curl_easy_init();
	if (!curl) {
		printf("could not init curl\n");
		return 1;
	}
	
	initscr();

	string res = {0};
	
	int i = 0;
	int j = 0;
	pthread_t thread;
	bool e = false;

	while (1) {
		clear();
		printw("%d %d\n", i++, j);
		if (e) {
			if (res.len) {
				printw("leaked ip %s", res.buf);
			} else {
				printw("leaking ip...");
			}
		} else {
			printw("press e 2 leak ip!!1");
		}
		refresh();
		timeout(16);
		int ch = getch();
		if (ch == 'q') break;
		if (ch == ' ') j++;
		if (ch == 'e') {
			int thread_res = pthread_create(&thread, NULL, thread_func, curl);
			if (thread_res) {
				printf("could not create thread\n");
				return 1;
			}
			e = true;
		}

		if (e) {
			void *thread_arg;
			
			// int thread_join_res = pthread_join(thread, &thread_arg);
			int thread_join_res = pthread_tryjoin_np(thread, &thread_arg);
			// if (thread_join_res) {
			// 	printf("could not join thread\n");
			// 	return 1;
			// }
			
			if (thread_join_res == 0) {
				res = *(string*)thread_arg;
				free(thread_arg);
			}
		}
	}
	
	endwin();

	free(res.buf);
	curl_global_cleanup();

	return 0;
}
