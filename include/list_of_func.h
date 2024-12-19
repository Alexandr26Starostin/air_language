#ifndef LIST_OF_FUNC_H
#define LIST_OF_FUNC_H

language_error_t create_list_of_func             (list_of_func_t* list_of_func);
language_error_t delete_list_of_func             (list_of_func_t* list_of_func);
language_error_t dump_list_of_func               (list_of_func_t* list_of_func);
language_error_t print_name_func_in_file_by_code (FILE* file, list_of_func_t* list_of_func, base_func_t code_find_func);
long             find_word_in_list_of_func       (list_of_func_t* list_of_func, char* find_word);

#endif