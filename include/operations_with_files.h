#ifndef OPERATIONS_WITH_FILES_H
#define OPERATIONS_WITH_FILES_H

language_error_t read_file_to_str                     (int argc, char** argv, char** ptr_str, operation_with_file_t operation_with_file, language_error_t error_of_search);
FILE*            find_program_file                    (int argc, char** argv, operation_with_file_t operation_with_file);
language_error_t write_tree_in_file                   (int argc, char** argv, node_t* root_node);
language_error_t write_table_in_file                  (int argc, char** argv, list_of_local_name_tables_t* list_of_local_name_tables, name_table_t* name_table, char* str_with_program);
language_error_t create_tree_from_str                 (char* str_with_tree, node_t** ptr_node, size_t* ptr_index_in_str);
language_error_t create_table_from_str                (char* str_with_table, name_table_t* name_table, size_t* ptr_index_in_str);
language_error_t create_list_of_local_tables_from_str (char* str_with_table, list_of_local_name_tables_t* list_of_local_name_tables, size_t* ptr_index_in_str);

#endif