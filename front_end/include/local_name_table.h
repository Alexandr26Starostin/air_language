#ifndef LOCAL_NAME_TABLE_H
#define LOCAL_NAME_TABLE_H

front_end_error_t create_list_of_local_name_tables  (list_of_local_name_tables_t* list_of_local_name_tables);
front_end_error_t add_new_local_name_table_in_list  (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table);
front_end_error_t add_new_name_in_local_name_table  (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table, size_t index_id_in_name_table, type_id_t type_id);
front_end_error_t delete_list_of_local_name_tables  (list_of_local_name_tables_t* list_of_local_name_tables);
front_end_error_t dump_list_of_local_name_tables    (list_of_local_name_tables_t* list_of_local_name_tables);
size_t            find_position_of_local_table    (list_of_local_name_tables_t* list_of_local_name_tables, long scope_of_local_name_table);

#endif