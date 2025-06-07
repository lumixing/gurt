#include <stdlib.h>
#include <string.h>
#include "arena.h"

#define STR_NEW_ZERO_CAP 64

typedef struct {
	char* buf;
	size_t len;
	size_t cap;
} str;

static Arena str_arena;
static Arena str_cstr_arena;

static inline void str_init_arena(size_t size) {
	str_arena = arena_init(size);
	str_cstr_arena = arena_init(size);
}

static inline void str_free_arena() {
	arena_free(str_arena);
	arena_free(str_cstr_arena);
}

static inline void str_clear_cstr_arena() {
	arena_clear(&str_cstr_arena);
}

static inline str str_new(char *strlit) {
	size_t strlitlen = strlen(strlit);
	char *buf = (char*)arena_alloc(&str_arena, strlitlen);
	if (!buf) {
		perror("could not init str");
		exit(1);
	}
	memcpy(buf, strlit, strlitlen);
	return (str){buf, strlitlen, strlitlen};
}

static inline str str_new_zero() {
	char *buf = (char*)arena_alloc(&str_arena, STR_NEW_ZERO_CAP);
	if (!buf) {
		perror("could not init str");
		exit(1);
	}
	return (str){buf, 0, STR_NEW_ZERO_CAP};
}

static inline str str_new_size(size_t size) {
	char *buf = (char*)arena_alloc(&str_arena, size);
	if (!buf) {
		perror("could not init str");
		exit(1);
	}
	return (str){buf, 0, size};
}

static inline char *str_cstr(str s) {
	char *buf = (char*)arena_alloc(&str_cstr_arena, s.len + 1);
	if (!buf) {
		printf("error: not enough space to alloc for cstr (%zu free but %zu needed)\n", str_cstr_arena.size - str_cstr_arena.used, s.len + 1);
		exit(1);
	}
	memcpy(buf, s.buf, s.len + 1);
	buf[s.len] = '\0';
	return buf;
}

static inline void str_concat(str *s1, str s2) {
	if (s1->len + s2.len > s1->cap) {
		while (s1->len + s2.len > s1->cap) {
			s1->cap *= 2;
		}
		s1->buf = (char*)realloc(s1->buf, s1->cap);
	}
	memcpy(s1->buf + s1->len, s2.buf, s2.len);
	s1->len += s2.len;
}

static inline bool str_eq(str s1, str s2) {
	return s1.len == s2.len && memcmp(s1.buf, s2.buf, s1.len) == 0;
}

static inline void str_debug(str s) {
	printf("printing str %zu/%zu:\n", s.len, s.cap);
	printf("USED BUF: ");
	for (int i = 0; i < s.cap; i++) {
		if (i == s.len) printf("\nFREE BUF: ");
		printf("%c", s.buf[i]);
	}
	printf("\n");
}
