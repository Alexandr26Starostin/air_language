#ifndef TREE_H
#define TREE_H

node_t*           create_node (node_type_t type, double value, node_t* left, node_t* right, node_t* parent);
front_end_error_t delete_node (node_t* node);
front_end_error_t dump_tree   (node_t* node, dump_tree_t* tree_dump);

#endif