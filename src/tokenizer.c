#include "config.inc.h"

#include "tokenizer.h"
#include <stdbool.h>
#include <stdlib.h>

struct mat_tokenizer {
	const char* /*const*/ orig;
	const char* row_start;
	const char* token_start;
	const char* token_end;
	char buf[256];
	size_t row;
	mat_tokenizer_token_type_t token_type;
};

const char* mat_tokenizer_name[MAT_TOKEN_TOKEN_TYPE_MAX] = {
		"INIT",
		"UNKNOWN",
		"EOD",
		"fn-name",
		"fn-opbr",
		"fn-argsp",
		"fn-clbr",
		"var",
		"literal"
};


static bool is_alphabet(char);
static bool is_numeric(char);
static bool is_alnum(char);
static bool is_space(char);
static const char* skip_space_chars(mat_tokenizer_t*, const char*);

/*
 * expr          := SP (<fn> | <var> | <literal>) SP
 * fn            := <fn-name> <op-bracket> SP <expr> SP (<arg-separator> SP <expr> SP)? <close-bracket>
 * op-bracket    := "["
 * close-bracket := "]"
 * arg-separator := ","
 * fn-name       := "[:alpha:]" ("[:alnum:]")+
 * var           := "[:alpha:]"
 * literal       := ([-+])? ("[:numeric:]")+ ("." ("[:numeric:]")* )?
 *                | "." ("[:numeric:]")+
 */

mat_tokenizer_t* mat_tokenizer_new(const char* str) {
	mat_tokenizer_t* ret = (mat_tokenizer_t*) malloc(sizeof(mat_tokenizer_t));
	ret->orig = str;
	ret->row_start = str;
	ret->token_start = str;
	ret->token_end = str;
	ret->token_type = MAT_TOKEN_INITIAL;
	ret->row = 1;
	return ret;
}

void mat_tokenizer_free(mat_tokenizer_t* tokenizer) {
	if (tokenizer) {
		free(tokenizer);
	}
}

mat_tokenizer_token_type_t mat_tokenizer_next(mat_tokenizer_t* tokenizer) {
	const char* const start = skip_space_chars(tokenizer, tokenizer->token_end);
	const char* p = start;
	char c = *p++;
	mat_tokenizer_token_type_t type;
	if (c == '\0') {
		type = MAT_TOKEN_END_OF_EXPRESSION;
	} else if (is_alphabet(c)) {
		if (is_alnum(*p)) {
			for (p = p + 1; is_alnum(*p); ++p) {
				// skip alnum chars
			}
			type = MAT_TOKEN_FUNC_NAME;
		} else {
			tokenizer->token_end = p;
			type = MAT_TOKEN_VARIABLE;
		}
	} else if (c == '-' || is_numeric(c)) {
		for (; is_numeric(*p); ++p) {
			// skip numeric chars
		}
		if (c == '-' && start + 1 == p) {
			type = MAT_TOKEN_UNKNOWN;
			goto error;
		}
		if (*p == '.') {
			for (p = p + 1; is_numeric(*p); ++p) {
				// skip numeric chars (if exists)
			}
		}
		type = MAT_TOKEN_LITERAL;
	} else if (c == '.') {
		for (; is_numeric(*p); ++p) {
			// skip numeric chars
		}
		if (start + 1 == p) {
			type = MAT_TOKEN_UNKNOWN;
		} else {
			type = MAT_TOKEN_LITERAL;
		}
	} else if (c == '[') {
		type = MAT_TOKEN_FUNC_OPENING_BRACKET;
	} else if (c == ']') {
		type = MAT_TOKEN_FUNC_CLOSING_BRACKET;
	} else if (c == ',') {
		type = MAT_TOKEN_FUNC_ARG_SEPARATOR;
	} else {
		type = MAT_TOKEN_UNKNOWN;
	}

error:
	tokenizer->token_start = start;
	tokenizer->token_end = p;
	tokenizer->token_type = type;
	return type;
}

const char* mat_tokenizer_get_token(mat_tokenizer_t* tokenizer) {
	const char* start = tokenizer->token_start;
	const char* end = tokenizer->token_end;
	char* out_p = tokenizer->buf;
	for (const char* in_p = start; in_p != end;) {
		*out_p++ = *in_p++;
	}
	*out_p = '\0';
	return tokenizer->buf;
}

char* mat_tokenizer_dup_token(mat_tokenizer_t* tokenizer) {
	const char* start = tokenizer->token_start;
	const char* end = tokenizer->token_end;
	char* out = (char*) malloc((size_t) (end - start + 1u));
	char* out_p = out;
	for (const char* in_p = start; in_p != end;) {
		*out_p++ = *in_p++;
	}
	*out_p = '\0';
	return out;
}

const char* mat_tokenizer_get_str(mat_tokenizer_t* tokenizer) {
	return tokenizer->orig;
}

const char* mat_tokenizer_get_row_str(mat_tokenizer_t* tokenizer) {
	return tokenizer->row_start;
}

size_t mat_tokenizer_get_row(mat_tokenizer_t* tokenizer) {
	return tokenizer->row;
}

size_t mat_tokenizer_get_col(mat_tokenizer_t* tokenizer) {
	return (size_t) (tokenizer->token_start - tokenizer->row_start + 1u);
}

size_t mat_tokenizer_get_token_len(mat_tokenizer_t* tokenizer) {
	return (size_t) (tokenizer->token_end - tokenizer->token_start);
}

const char* mat_tokenizer_get_token_type_name(mat_tokenizer_token_type_t t) {
	if (t >= MAT_TOKEN_TOKEN_TYPE_MAX) {
		return "<illegal>";
	} else {
		return mat_tokenizer_name[t];
	}
}


static bool is_alphabet(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static bool is_numeric(char c) {
	return ('0' <= c && c <= '9');
}

static bool is_alnum(char c) {
	return is_alphabet(c) || is_numeric(c);
}

static bool is_space(char c) {
	return c == '\r' || c == '\n' || c == ' ' || c == '\t';
}

static const char* skip_space_chars(mat_tokenizer_t* tokenizer, const char* str) {
	const char* p = str;
	for (; is_space(*p); ++p) {
		if (*p == '\n') {
			tokenizer->row_start = p + 1;
			tokenizer->row++;
		}
	}
	return p;
}
