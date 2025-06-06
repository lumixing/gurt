#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	char *buf;
	size_t used;
	size_t size;
} Arena;

static inline Arena arena_init(size_t size, bool dynamic) {
	char *buf = (char*)malloc(size);
	if (!buf) {
		perror("could not init arena");
		exit(1);
	}
	return (Arena){buf, 0, size};
}

static inline void arena_free(Arena arena) {
#ifdef ARENA_DEBUG
	printf("arena freed %zu bytes\n", arena.used);
#endif
	free(arena.buf);
}

static inline void arena_clear(Arena *arena) {
	arena->used = 0;
}

static inline void *arena_alloc(Arena *arena, size_t size) {
	if (arena->used + size > arena->size) {
		return NULL;
	}

	void *ptr = arena->buf + arena->used;
	arena->used += size;
#ifdef ARENA_DEBUG
	printf("arena allocated %zu bytes\n", size);
#endif
	return ptr;
}
