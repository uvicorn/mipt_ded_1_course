// #include "tree.hpp"

// using VarName = char;

// Node* differentiate(Node* root){
//     if (root == nullptr)
//         return nullptr;

//     #define BIND_HANDLER(OP_NAME, HANDLER)      \
//         case (OP_NAME):                         \
//             return HANDLER(root);               \

// }


// namespace Handlers {
//     inline Node ln(Node* root){
//         // Node new_node = {.type = FUNC, .func_type = DIV};
//         Node new_node = Node(FuncName::DIV, 2);
//         new_node.childs[0] = root;
//         new_node.childs[1] = differentiate(root);
//         return new_node;
//     }

//     #define LINEAR_DIFFERENTIAL(OP, FUNCNAME, ARGS_COUNT) inline Node OP(Node* root){   \
//         Node new_node = Node(FUNCNAME, ARGS_COUNT);                     \
//         new_node.childs[0] = differentiate(root->childs[0]);                            \
//         new_node.childs[1] = differentiate(root->childs[1]);                            \
//         return new_node;                                                                \
//     }
//     LINEAR_DIFFERENTIAL(add, FuncName::ADD, 2);
//     LINEAR_DIFFERENTIAL(sub, FuncName::ADD, 2);
//     LINEAR_DIFFERENTIAL(unary_minus, FuncName::UNARY_MINUS, 1);

//     inline Node* mul(Node* root){
//         Node* new_node      = new Node(FuncName::ADD, 2);
//         Node* new_node_left = new Node(FuncName::MUL, 2);
//         Node* new_node_right= new Node(FuncName::MUL, 2);

//         new_node_left->childs[0] = root->childs[0];
//         new_node_left->childs[1] = differentiate(root->childs[1]);

//         new_node_right->childs[0] = root->childs[1];
//         new_node_right->childs[1] = differentiate(root->childs[0]);
//         new_node->childs[0] = new_node_left;
//         new_node->childs[1] = new_node_right;
//         return new_node;
//     }

//     // (a/b)' = (a'b - b'a)/(b*b)
//     inline Node* div(Node* root){
//         Node* new_node    = new Node(FuncName::DIV, 2);
//         Node* numerator   = new Node(FuncName::SUB, 2);
//         Node* denominator = new Node(FuncName::MUL, 2);

//         denominator->childs[0] = root->childs[0];
//         denominator->childs[1] = differentiate(root->childs[1]);

//         // new_node_right->childs[0] = root->childs[1];
//         // new_node_right->childs[1] = differentiate(root->childs[0]);
//         // new_node->childs[0] = new_node_left;
//         // new_node->childs[1] = new_node_right;
//         return new_node;
//     }


//     // inline Node add(Node* root){
//     //     Node new_node = Node(NodeType::FUNC, FuncName::ADD, 2);
//     //     new_node.childs[0] = differentiate(root->childs[0]);
//     //     new_node.childs[1] = differentiate(root->childs[1]);
//     //     return new_node;
//     // }
//     // static (Node*)(Node*) table[20];
// }

