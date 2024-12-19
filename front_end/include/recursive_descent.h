#ifndef RECURSIVE_DESCENT_H
#define RECURSIVE_DESCENT_H

front_end_error_t recursive_descent (array_of_tokens_t* tokens, name_table_t* name_table, list_of_local_name_tables_t* list_of_local_name_tables, node_t* root_node);

#endif
