#ifndef WRITE_TREE_IN_NASM_H
#define WRITE_TREE_IN_NASM_H

language_error_t write_tree_in_nasm (int argc, char** argv, node_t* root_node, name_table_t* name_table, 
                                    list_of_local_name_tables_t* list_of_local_name_tables, char* str_with_table);

#endif