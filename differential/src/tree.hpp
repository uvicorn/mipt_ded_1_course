#ifndef DIFF_TREE_HPP
#define DIFF_TREE_HPP

#include <memory.h>
#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <string_view>
#include <variant>

enum class NodeType: uint8_t {
    CONST = 0,
    VAR   = 1,
    FUNC  = 2,
};

enum class FuncName: uint8_t{
    LN,

    ADD,
    SUB,
    MUL,
    DIV,
    UNARY_MINUS,
};


class Node;
using NodePtr = Node*;

class Node{
  public:
    NodeType type;
    std::size_t childs_count;
    NodePtr* childs;
    FuncName func_name;

    Node() = default;
    ~Node() = default;

    Node(FuncName func_name, Node* node1, Node* node2) :
        func_name(func_name),
        type(NodeType::FUNC)
    {
        childs = new NodePtr[2];
        childs[0] = node1;
        childs[1] = node2;
    }

    Node(FuncName func_name, size_t childs_count) :
        func_name(func_name),
        childs_count(childs_count),
        type(NodeType::FUNC)
    {
        childs = new NodePtr[childs_count];
    }

    Node(FuncName func_name, std::initializer_list<Node*> childs) :
        func_name(func_name),
        childs_count(childs.size()),
        type(NodeType::FUNC)
    {
        childs = std::move(childs);
    }

    Node* operator[](size_t index){
        return childs[index];
    }

    // Node& operator[](size_t index){
    //     return *childs[index];
    // }
    // Node operator=(Node node) = default;
    Node* copy(){
        Node* new_node = new Node();
        std::copy_n(childs, this->childs_count, this->childs);
    }
    
    // Node* my_node_copy = my_node.copy_tree()
    Node* copy_tree(){
        Node* new_node = new Node(func_name, childs_count);
        for (size_t index = 0; index < childs_count; index++){
            if (childs[index] != nullptr)
                new_node->childs[index] = childs[index]->copy_tree();
            else
                new_node->childs[index] = nullptr;
        }
        return new_node;
    }
    // ~Node(){
    //     delete childs;
    // }
};


#endif
