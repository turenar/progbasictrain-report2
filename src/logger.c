#include "config.inc.h"

#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

static void log_log(const char* file, int line, const char* color, const char* fmt, va_list args) {
	const char* filename = file;
	for (const char* p = file; *p != '\0'; p++) {
		if (*p == '/' || *p == '\\') {
			filename = p + 1;
		}
	}
#ifdef WIN32
	// コンソールが色表示に対応していない
	UNUSED_VAR(color);
	fprintf(stderr, "[%s:%d] ", filename, line);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	fflush(stderr);
#else
	fprintf(stderr, "\033[%sm[%s:%d] ", color, filename, line);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\033[0m\n");
#endif
}

void log_debug(const char* file, int line, const char* fmt, ...) {
#ifdef _DEBUG
	va_list args;
	va_start(args, fmt);
	log_log(file, line, "36", fmt, args);
	va_end(args);
#else
	UNUSED_VAR(file);
	UNUSED_VAR(line);
	UNUSED_VAR(fmt);
#endif
}

void log_info(const char* file, int line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(file, line, "34", fmt, args);
	va_end(args);
}

void log_warn(const char* file, int line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(file, line, "1;33", fmt, args);
	va_end(args);
}

void log_error(const char* file, int line, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_log(file, line, "1;31", fmt, args);
	va_end(args);
}
