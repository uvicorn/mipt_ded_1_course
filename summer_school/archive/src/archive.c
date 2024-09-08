#include "consts.h"
#include "utils.h"
#include "stdlib.h"
#include "assert.h"
#include "bitstream.h"
#include "values.h"
#include "archive.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include "output.h"

// FIXME: tree.h tree.c
// FIXME: CounterTable{tableSize, int*}
void create_counter_table(const uint8_t* buffer, size_t size, int* counter_table){
    for (size_t i = 0; i < size; i++)
        counter_table[buffer[i]]++;
}

void merge_nodes(Node* node0, Node* node1, Node* node_out, int node_out_id){
    /* PRINT_NODE_st(node0) */
    /* PRINT_NODE_st(node1) */
    node_out->node_type = MERGER;
    node_out->ctr = node0->ctr + node1->ctr;
    node_out->childs[0] = node0->node_id;
    node_out->childs[1] = node1->node_id;
    node_out->node_id = node_out_id;
    node_out->symbol = -1337;
}

void create_mask_table(Node* nodes, Node* root, Mask mask, Mask* mask_table){
    PRINT_NODE_st(root)
    if(root->node_type == MERGER){
        Mask mask_left_node = {};
        memcpy(mask_left_node.blocks, mask.blocks, MASK_SIZE * sizeof(mask_element));
        mask_left_node.bit_length = mask.bit_length + 1;
        create_mask_table(nodes, &nodes[root->childs[0]], mask_left_node, mask_table); // left

        Mask mask_right_node = {};
        memcpy(mask_right_node.blocks, mask.blocks, MASK_SIZE);
        mask_right_node.bit_length = mask.bit_length + 1;

        int mask_bit_length = sizeof(mask_element) * BITSPERBYTE;
        int masks_arr_bit_length = MASK_SIZE * mask_bit_length; // 64 - unsigned long long bit length
        assert(mask_right_node.bit_length < masks_arr_bit_length);
        
        mask_right_node.blocks[mask_right_node.bit_length / mask_bit_length] += 1 << ((mask_right_node.bit_length - 1) % mask_bit_length); // TODO: перепроверить
        create_mask_table(nodes, &nodes[root->childs[1]], mask_right_node, mask_table); // right

    } else if (root->node_type == ORIGINAL){
        mask_table[root->symbol].bit_length = mask.bit_length;
        memcpy(mask_table[root->symbol].blocks, mask.blocks, sizeof(mask_element) * MASK_SIZE);
    } else {
       assert(!"ERROR: create_mask_table failed!");
    }

}

// nodes        - 
// bad naming
int create_huffman_tree(Tree* tree) {
    int left_index = 0;
    int original_nodes_count = tree->tree_size;
    int last_node_id = original_nodes_count - 1; // все ок

    Node* sorted_nodes = (Node *)calloc(original_nodes_count, sizeof(Node)); // FIXME:
    memcpy(sorted_nodes, tree->nodes, original_nodes_count * sizeof(Node));

    while (left_index < original_nodes_count-1){ // ВСЕ ОК, ИНАЧЕ ХИП ОВЕРФЛОУ
        quickSort_nodes(sorted_nodes, left_index, original_nodes_count - 1); // TODO: написать эффективный алгос

        int new_node_id = ++last_node_id;
        merge_nodes(&sorted_nodes[left_index], &sorted_nodes[left_index + 1], &tree->nodes[new_node_id], new_node_id);
        sorted_nodes[left_index + 1] = tree->nodes[new_node_id]; // TODO: спросить хорошо ли копировать, или лучше на ссылках сделать

        left_index++;
        tree->tree_size++;
    }
    free(sorted_nodes);
    return last_node_id;
}


void create_encode_table(Tree* tree, Mask* mask_table){
    Mask mask = {.blocks = {}, .bit_length = 0 };
    create_mask_table(tree->nodes, &tree->nodes[tree->root], mask, mask_table);
}

void save_table(const Mask* mask_table, struct bitstream* bs){
    assert(mask_table != NULL);
    assert(bs != NULL);

    for (size_t mask_index = 0; mask_index < TABLE_SIZE; mask_index++){
        const Mask* mask = &mask_table[mask_index];
        bitstream_write_single(bs, sizeof(mask->bit_length)* BITSPERBYTE, mask->bit_length);
        bitstream_write_many(bs, mask->bit_length, mask->blocks);
    }
}

void save_tree(const Tree* tree, struct bitstream* bs){
    assert(tree != NULL);
    assert(bs != NULL);
    /* fprintf(stderr, "bs bit_length=%d byte_size=%d write_ptr=%d\n", bs->bit_length,bs->byte_size,bs->write_ptr); */
    bitstream_write_single(bs, sizeof(tree->tree_size) * BITSPERBYTE, tree->tree_size);
    bitstream_write_single(bs, sizeof(tree->root) * BITSPERBYTE, tree->root);
    Node* nodes = tree->nodes;
    for (size_t node_index = 0; node_index < tree->tree_size; node_index++){
        /* fprintf(stderr, "node_index=%d tree_size=%d\n", node_index, tree->tree_size); */
        Node* node = &nodes[node_index];
        bitstream_write_single(bs, (TABLE_SIZE/256) * BITSPERBYTE, node->symbol);
        bitstream_write_single(bs, sizeof(node->childs[0]) * BITSPERBYTE, node->childs[0]);
        bitstream_write_single(bs, sizeof(node->childs[1]) * BITSPERBYTE, node->childs[1]);
        bitstream_write_single(bs, 1, node->node_type); // ONE BIT
    }
}


Output encode_buffer(const uint8_t* read_buffer, size_t size){
    int counter_table[TABLE_SIZE];
    memset(counter_table, 0, sizeof counter_table);
    create_counter_table(read_buffer, size, counter_table);

    /* for (int i=0;i<TABLE_SIZE;i++)fprintf(stderr, "%d, ", counter_table[i]); */

    Node* nodes = (Node *)calloc(TREE_SIZE, sizeof(Node));
    int last_node_id = 0;

    for (int i = 0; i < TABLE_SIZE; i++){
        if (counter_table[i] != 0){
            nodes[last_node_id] =  (Node){
                .symbol = i,
                .ctr = counter_table[i],
                .childs = {MAX_UINT, MAX_UINT},
                .node_type = ORIGINAL,
                .node_id = last_node_id
            };
            last_node_id++;
        }
    }

    Tree tree = {};
    tree.nodes = nodes;
    tree.tree_size = last_node_id;

    tree.root = create_huffman_tree(&tree);
    for (int i=0; i < tree.tree_size;i++)PRINT_NODE_t(tree.nodes[i]);
    
    Mask* mask_table = (Mask*)calloc(TABLE_SIZE, sizeof(Mask));
    
    create_encode_table(&tree, mask_table);
    
    struct bitstream bs;
    unsigned int excepted_output_bit_length = size * BITSPERBYTE + TABLE_SIZE * TABLE_SIZE + 14880; // TODO: переписать

    bitstream_create(excepted_output_bit_length, &bs);
    save_tree(&tree, &bs);

    for (size_t i = 0; i < size; i++){
        unsigned int symbol = read_buffer[i]; // TODO: переделать buffer в non char
        Mask mask = mask_table[symbol];
        bitstream_write_many(&bs, mask.bit_length, mask.blocks);
    }
    
    free(nodes); // TODO: написать для node и tree нормальный деструктор
    free(mask_table);
    Output export_output = bitstream_export(&bs, bs.byte_size);
    bitstream_delete(&bs);
    return export_output;
}


int search_code_symbol(Node* nodes, int root_node_id, Mask mask){
    int current_node_id = root_node_id;
    unsigned int code_length = mask.bit_length;

    for (unsigned int bit_ptr = 0; bit_ptr < code_length; bit_ptr++){
        Node current_node = nodes[current_node_id];
        int mask_block_index = bit_ptr / sizeof(unsigned long long);
        int mask_block_shift = bit_ptr % sizeof(unsigned long long);

        int bit = (mask.blocks[mask_block_index] >> mask_block_shift) & 1;
        current_node_id = current_node.childs[bit];
    }

    Node final_node = nodes[current_node_id];
    assert(final_node.node_type == ORIGINAL);
    return final_node.symbol;
}
