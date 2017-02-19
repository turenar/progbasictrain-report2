#include "config.inc.h"

#include "gmp_wrapper.h"

#ifdef USE_GMP
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#else

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

void mpf_init(mpf_ptr a) {
	*a = 0;
}

void mpf_init2(mpf_ptr a, mp_bitcnt_t prec) {
	(void) prec;
	*a = 0;
}

void mpq_init(mpq_ptr a) {
	*a = 0;
}

void mpq_inits(mpq_ptr a, ...) {
	*a = 0;
	va_list l;
	va_start(l, a);
	while (1) {
		mpq_ptr p = va_arg(l, mpq_ptr);
		if (p == NULL) {
			break;
		}
		*p = 0;
	}
	va_end(l);
}

int mpf_set_str(mpf_ptr a, const char* b, mp_bitcnt_t c) {
	UNUSED_VAR(c);
	char* endptr;
	*a = strtod(b, &endptr);
	return *endptr != '\0';
}

void mpf_set_q(mpf_ptr a, mpf_srcptr b) {
	*a = *b;
}

void mpq_set(mpq_ptr a, mpq_srcptr b) {
	*a = *b;
}

void mpq_set_f(mpq_ptr a, const mpf_ptr b) {
	*a = *b;
}

void mpq_set_d(mpq_ptr a, double b) {
	*a = b;
}

void mpq_set_si(mpq_ptr a, signed long b, signed long c) {
	*a = b / (double) c;
}

void mpq_set_ui(mpq_ptr a, unsigned long b, unsigned long c) {
	*a = b / (double) c;
}

double mpq_get_d(mpq_srcptr a) {
	return *a;
}

int mpq_sgn(mpq_srcptr a) {
	return *a < 0 ? -1 : (*a > 0 ? 1 : 0);
}

int mpq_cmp(mpq_srcptr a, mpq_srcptr b) {
	return *a < *b ? -1 : (*a > *b ? 1 : 0);
}

int mpq_cmp_si(mpq_srcptr a, signed long b, signed long c) {
	mpq_t k;
	mpq_init(k);
	mpq_set_si(k, b, c);
	int r = mpq_cmp(a, k);
	mpq_clear(k);
	return r;
}

int mpq_cmp_ui(mpq_srcptr a, unsigned long b, unsigned long c) {
	return mpq_cmp_si(a, (signed long) b, (signed long) c);
}

void mpq_abs(mpq_ptr a, mpq_srcptr b) {
	*a = fabs(*b);
}

void mpq_add(mpq_ptr a, mpq_srcptr b, mpq_srcptr c) {
	*a = *b + *c;
}

void mpq_sub(mpq_ptr a, mpq_srcptr b, mpq_srcptr c) {
	*a = *b - *c;
}

void mpq_mul(mpq_ptr a, mpq_srcptr b, mpq_srcptr c) {
	*a = *b * *c;
}

void mpq_div(mpq_ptr a, mpq_srcptr b, mpq_srcptr c) {
	*a = *b / *c;
}

void mpf_clear(mpf_ptr a) {
	UNUSED_VAR(a);
}

void mpq_clear(mpq_ptr a) {
	UNUSED_VAR(a);
}

void mpq_clears(mpq_ptr a, ...) {
	UNUSED_VAR(a);
}

#endif
