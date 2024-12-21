#ifndef WRITE_PROGRAM_FROM_TREE_H
#define WRITE_PROGRAM_FROM_TREE_H

language_error_t write_program_from_tree (int argc, char** argv, node_t* node, char* str_with_table, 
                                          name_table_t* name_table, list_of_func_t* list_of_func);

#endif