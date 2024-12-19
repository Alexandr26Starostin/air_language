#ifndef TOKENS_H
#define TOKENS_H

language_error_t create_tokens 			      (array_of_tokens_t* tokens);
language_error_t delete_tokens 			      (array_of_tokens_t* tokens);
language_error_t add_token_in_array_of_tokens (array_of_tokens_t* tokens, token_type_t  type, double value);
language_error_t dump_array_of_tokens         (array_of_tokens_t* tokens);

#endif