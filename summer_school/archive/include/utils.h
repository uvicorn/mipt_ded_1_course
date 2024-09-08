#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include "consts.h"

typedef unsigned long long mask_element;

enum NODE_TYPE {
    ORIGINAL,
    MERGER
};

typedef struct {
    uint32_t symbol;
    int ctr;
    uint32_t childs[2];
    enum NODE_TYPE node_type;
    int node_id;
} Node;

#define PRINT_NODE_st(node) fprintf(stderr, "Node %d: symbol=%d, ctr=%d, childs={%d, %d}, node_type=%d\n", node->node_id, node->symbol, node->ctr, node->childs[0], node->childs[1], node->node_type);
#define PRINT_NODE_t(node) fprintf(stderr, "Node %d: symbol=%d, ctr=%d, childs={%d, %d}, node_type=%d\n", node.node_id, node.symbol, node.ctr, node.childs[0], node.childs[1], node.node_type);

typedef struct{
    size_t root;
    Node* nodes;
    size_t tree_size;
} Tree;


typedef struct {
    mask_element blocks[MASK_SIZE]; // TODO: подумать переписывать ли на указатель с malloc
    uint16_t bit_length;
} Mask;

void quickSort_nodes(Node* arr, int low, int high);

#endif
