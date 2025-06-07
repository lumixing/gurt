#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static FILE *log_file;

static inline int log_init(char* file_name) {
	log_file = fopen(file_name, "w");

	if (!log_file) {
		perror("could not init log file");
		return 0;
	}

	return 1;
}

static inline void log(char *fmt, ...) {
	fprintf(log_file, "[%ld] ", time(NULL));

	va_list args;
	va_start(args, fmt);
	vfprintf(log_file, fmt, args);
	va_end(args);

	fprintf(log_file, "\n");
	fflush(log_file);
}

static inline void log_free() {
	fclose(log_file);
}
