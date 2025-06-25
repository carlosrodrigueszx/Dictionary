#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

template <typename Key, typename Value>
struct Node {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* p;     // pai
    int height;  // usado em AVL
    bool color;  // usado em Red-Black Tree

    // Construtor completo para Red-Black Tree
    Node(const Key& k, const Value& v, bool c, Node* l, Node* r, Node* parent)
        : key(k), value(v), left(l), right(r), p(parent), height(1), color(c) {}

    // Construtor usado por AVL (com altura)
    Node(const Key& k, const Value& v, int h, Node* l, Node* r)
        : key(k), value(v), left(l), right(r), p(nullptr), height(h), color(0) {}

    // Construtor AVL alternativo (sem valor)
    Node(const Key& k, int h, Node* l, Node* r)
        : key(k), value(), left(l), right(r), p(nullptr), height(h), color(0) {}

    // Construtor simples para testes ou inserções básicas
    Node(const Key& k, const Value& v)
        : key(k), value(v), left(nullptr), right(nullptr),
          p(nullptr), height(1), color(0) {}

    // Construtor nulo (para sentinela `nil`)
    Node()
        : key(), value(), left(nullptr), right(nullptr),
          p(nullptr), height(1), color(0) {}
};

#endif // NODE_HPP