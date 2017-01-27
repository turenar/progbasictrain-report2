#include <stdio.h>
#include <stdbool.h>
#include "tokenizer.h"

int main() {
	const char* expr = "Plus [ Times [ Sin [ 13.4 ] , 3 ] , 2 ]";
	printf("expression:\t%s\n", expr);

	mat_tokenizer_t* tokenizer = mat_tokenizer_new(expr);
	while (true) {
		mat_tokenizer_token_type_t t = mat_tokenizer_next(tokenizer);
		printf("%s:\t%s\n", mat_tokenizer_get_token_type_name(t), mat_tokenizer_get_token(tokenizer));
		if (t == MAT_TOKEN_END_OF_EXPRESSION || t == MAT_TOKEN_UNKNOWN) {
			break;
		}
	}
	mat_tokenizer_free(tokenizer);
}
