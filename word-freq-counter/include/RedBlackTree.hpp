#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include "Node.hpp"
#include <iostream>

template <typename Key, typename Value>
class RedBlackTree {
private:
    Node<Key, Value>* m_root;
    Node<Key, Value>* m_nil;
    
    int m_size;

    int left_rotates, right_rotates;
    mutable size_t key_comparisons;

    static constexpr bool RED = 0;
    static constexpr bool BLACK = 1;

public:
    RedBlackTree();
    ~RedBlackTree();
    void insert(const Key& key);
    void update(const Key& key, const Value& new_value);
    Value get(const Key& key) const;
    void remove(const Key& key);
    bool contains(const Key& key) const;
    void forEach(std::function<void(const Key&, const Value&)> func) const;
    int size() const;
    void clear();
    void print(std::ostream& out = std::cout) const;
    size_t get_comparisons() const;

private:
    Node<Key, Value>* rotateLeft(Node<Key, Value>* x);
    Node<Key, Value>* rotateRight(Node<Key, Value>* y);
    void insertFixup(Node<Key, Value>* z);
    void printInOrder(Node<Key, Value>* node, std::ostream& out) const;
    void transplant(Node<Key, Value>* u, Node<Key, Value>* v);
    Node<Key, Value>* minimum(Node<Key, Value>* node) const;
    void deleteFixup(Node<Key, Value>* x);
};

template <typename Key, typename Value>
RedBlackTree<Key, Value>::RedBlackTree() {
    m_nil = new Node<Key, Value>();
    m_nil->color = BLACK;
    m_nil->left = m_nil->right = m_nil->p = m_nil;
    m_root = m_nil;
    m_size = left_rotates = right_rotates = key_comparisons = 0;
}

template <typename Key, typename Value>
RedBlackTree<Key, Value>::~RedBlackTree() {
    clear();
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::insert(const Key& key) {
    Node<Key, Value>* y = m_nil;
    Node<Key, Value>* x = m_root;

    while (x != m_nil) {
        y = x;
        key_comparisons++;
        if (key == x->key) {
            x->value++;
            return;
        } else if (key < x->key) {
            key_comparisons++;
            x = x->left;
        } else {
            key_comparisons++;
            x = x->right;
        }
    }

    Node<Key, Value>* z = new Node<Key, Value>(key, 1, RED, m_nil, m_nil, m_nil);
    z->p = y;
    if (y == m_nil){
        m_root = z;
        key_comparisons++;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    insertFixup(z);
    m_size++;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::update(const Key& key, const Value& new_value) {
    Node<Key, Value>* node = m_root;
    while (node != m_nil) {
        key_comparisons++;
        if (key == node->key) {
            node->value = new_value;
            return;
        } else if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    throw std::runtime_error("Chave não encontrada para atualização");
}

template <typename Key, typename Value>
Value RedBlackTree<Key, Value>::get(const Key& key) const {
    Node<Key, Value>* node = m_root;
    while (node != m_nil) {
        key_comparisons++;
        if (key == node->key)
            return node->value;
        else if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    throw std::runtime_error("Chave não encontrada");
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::remove(const Key& key) {
    Node<Key, Value>* z = m_root;
    while (z != m_nil) {
        key_comparisons++;
        if (key == z->key) break;
        else if (key < z->key) z = z->left;
        else z = z->right;
    }

    if (z == m_nil) return; // chave não encontrada

    if (z->value > 1) {
        z->value--; // apenas decrementa a contagem
        return;
    }

    Node<Key, Value>* y = z;
    Node<Key, Value>* x;
    bool y_original_color = y->color;

    if (z->left == m_nil) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == m_nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->p == z) {
            x->p = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    delete z;

    if (y_original_color == BLACK) {
        deleteFixup(x);
    }
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::contains(const Key& key) const {
    Node<Key, Value>* node = m_root;
    while (node != m_nil) {
        key_comparisons++;
        if (key == node->key)
            return true;
        else if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return false;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::forEach(std::function<void(const Key&, const Value&)> func) const {
    std::function<void(Node<Key, Value>*)> inOrder = [&](Node<Key, Value>* node) {
        if (node == m_nil) return;
        inOrder(node->left);
        func(node->key, node->value);
        inOrder(node->right);
    };
    inOrder(m_root);
}

template <typename Key, typename Value>
int RedBlackTree<Key, Value>::size() const {
    return m_size;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::clear() {
    std::function<void(Node<Key, Value>*)> destroy = [&](Node<Key, Value>* node) {
        if (node == m_nil) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    };
    destroy(m_root);
    m_root = m_nil;
    m_size = 0;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::print(std::ostream& out) const {
    printInOrder(m_root, out);
    std::cout << "\n";
}

template <typename Key, typename Value>
Node<Key, Value>* RedBlackTree<Key, Value>::rotateLeft(Node<Key, Value>* x) {
    Node<Key, Value>* y = x->right;
    x->right = y->left;
    if (y->left != m_nil) y->left->p = x;
    y->p = x->p;

    if (x->p == m_nil) m_root = y;
    else if (x == x->p->left) x->p->left = y;
    else x->p->right = y;

    y->left = x;
    x->p = y;
    return y;
}

template <typename Key, typename Value>
Node<Key, Value>* RedBlackTree<Key, Value>::rotateRight(Node<Key, Value>* y) {
    Node<Key, Value>* x = y->left;
    y->left = x->right;
    if (x->right != m_nil) x->right->p = y;
    x->p = y->p;

    if (y->p == m_nil) m_root = x;
    else if (y == y->p->right) y->p->right = x;
    else y->p->left = x;

    x->right = y;
    y->p = x;
    return x;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::insertFixup(Node<Key, Value>* z) {
    while (z->p->color == RED) {
        Node<Key, Value>* gp = z->p->p;
        if (z->p == gp->left) {
            Node<Key, Value>* y = gp->right;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->p->right) {
                    z = z->p;
                    rotateLeft(z);
                }
                z->p->color = BLACK;
                gp->color = RED;
                rotateRight(gp);
            }
        } else {
            Node<Key, Value>* y = gp->left;
            if (y->color == RED) {
                z->p->color = BLACK;
                y->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->p->left) {
                    z = z->p;
                    rotateRight(z);
                }
                z->p->color = BLACK;
                gp->color = RED;
                rotateLeft(gp);
            }
        }
    }
    m_root->color = BLACK;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::printInOrder(Node<Key, Value>* node, std::ostream& out) const {
    if (node == m_nil) return;

    printInOrder(node->left, out);

    out << node->key << " : " << node->value << '\n';

    printInOrder(node->right, out);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::transplant(Node<Key, Value>* u, Node<Key, Value>* v) {
    if (u->p == m_nil) {
        m_root = v;
    } else if (u == u->p->left) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }
    v->p = u->p;
}

template <typename Key, typename Value>
Node<Key, Value>* RedBlackTree<Key, Value>::minimum(Node<Key, Value>* node) const {
    while (node->left != m_nil) {
        node = node->left;
    }
    return node;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::deleteFixup(Node<Key, Value>* x) {
    while (x != m_root && x->color == BLACK) {
        if (x == x->p->left) {
            Node<Key, Value>* w = x->p->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rotateLeft(x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                rotateLeft(x->p);
                x = m_root;
            }
        } else {
            Node<Key, Value>* w = x->p->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                rotateRight(x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->p;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                rotateRight(x->p);
                x = m_root;
            }
        }
    }
    x->color = BLACK;
}

template <typename Key, typename Value>
size_t RedBlackTree<Key, Value>::get_comparisons() const{
    return key_comparisons;
}
#endif // RED_BLACK_TREE_HPP