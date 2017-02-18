#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <expected> <actual>\n", argv[0]);
		return 1;
	}

	char* endptr;
	double expected = strtof(argv[1], &endptr);
	if (*endptr) {
		fprintf(stderr, "parse error: %s\n", argv[1]);
		return 1;
	}
	double actual = strtof(argv[2], &endptr);
	if (*endptr) {
		fprintf(stderr, "parse error: %s\n", argv[2]);
		return 1;
	}
	int result = fabs(expected - actual) > 0.000001;
	if (result) {
		fprintf(stderr, "assertion failed: expected=%f, actual=%f\n", expected, actual);
	}
	return result;
}
