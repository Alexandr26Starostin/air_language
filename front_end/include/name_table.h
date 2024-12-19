#ifndef NAME_TABLE_H
#define NAME_TABLE_H

front_end_error_t create_name_table              (name_table_t* name_table);
front_end_error_t delete_name_table              (name_table_t* name_table);
front_end_error_t add_name_in_table              (name_table_t* name_table, size_t index_to_name_in_str, size_t len_name, char* str_with_program);
front_end_error_t dump_name_table                (name_table_t* name_table, char* str_with_program);
size_t            find_position_name             (name_table_t* name_table, size_t find_index, size_t len_name, char* str_with_program);
front_end_error_t print_symbols_from_str         (char* str, size_t count_of_symbols);
front_end_error_t print_symbols_from_str_in_file (FILE* file, char* str, size_t count_of_symbols);

#endif