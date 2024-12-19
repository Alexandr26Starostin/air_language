#ifndef OPERATIONS_WITH_FILES_H
#define OPERATIONS_WITH_FILES_H

front_end_error_t read_program_file   (int argc, char** argv, char** ptr_str_with_program);
FILE*             find_program_file   (int argc, char** argv, operation_with_file_t operation_with_file);
front_end_error_t write_tree_in_file  (int argc, char** argv, node_t* root_node);
front_end_error_t write_table_in_file (int argc, char** argv, list_of_local_name_tables_t* list_of_local_name_tables, name_table_t* name_table, char* str_with_program);

#endif