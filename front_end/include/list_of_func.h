#ifndef LIST_OF_FUNC_H
#define LIST_OF_FUNC_H

front_end_error_t create_list_of_func       (list_of_func_t* list_of_func);
front_end_error_t delete_list_of_func       (list_of_func_t* list_of_func);
front_end_error_t dump_list_of_func         (list_of_func_t* list_of_func);
long              find_word_in_list_of_func (list_of_func_t* list_of_func, char* find_word);

#endif