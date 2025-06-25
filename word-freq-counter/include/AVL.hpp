#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <algorithm>
#include <functional>

#include "Node.hpp"

template <typename Key, typename Value>
class AVL {
public:
    AVL(); 
    void insert(const Key& k); 
    void update(const Key& key, const Value& new_value);
    Value get(const Key& key) const;
    void remove(const Key& k); 
    bool contains(const Key& k) const; 
    void forEach(std::function<void(const Key&, const Value&)> func) const;
    int size() const;
    void clear(); 
    
private:
    Node<Key, Value>* m_root;
    
    int m_size;
    
    int left_rotates, right_rotates;
    mutable size_t key_comparisons;

public:
    int height(Node<Key, Value>* node) const; 
    int balance(Node<Key, Value>* node) const; 
    
    void show() const; 
    void print(std::ostream& out = std::cout) const; 
    
    size_t get_comparisons() const; 
    int get_left_rotations() const; 
    int get_right_rotations() const; 
    ~AVL();

private:
    Node<Key, Value>* _insert(Node<Key, Value>* node, const Key& k); 
    Node<Key, Value>* _remove(Node<Key, Value>* node, const Key& k); 
    Node<Key, Value>* _remove_node(Node<Key, Value>* node); 
    Node<Key, Value>* _contains(Node<Key, Value>* node, const Key& k) const; 

    Node<Key, Value>* fixup_node(Node<Key, Value>* node); 
    Node<Key, Value>* left_rotation(Node<Key, Value>* p); 
    Node<Key, Value>* right_rotation(Node<Key, Value>* p); 
    Node<Key, Value>* _clear(Node<Key, Value>* node); 

private:
    void bshow(Node<Key, Value>* node, std::string heranca) const; 
    void printInOrder(Node<Key, Value>* node, std::ostream& out) const; 
};

template <typename Key, typename Value>
AVL<Key, Value>::AVL(){
    m_root = nullptr;
    m_size = left_rotates = right_rotates = key_comparisons = 0;
    std::cout << "dicionário construído com valores padrão" << std::endl;
}

template <typename Key, typename Value>
AVL<Key, Value>::~AVL(){
    clear();
}

template <typename Key, typename Value>
void AVL<Key, Value>::insert(const Key& key){
    m_root = _insert(m_root, key);
}

template <typename Key, typename Value>
void AVL<Key, Value>::update(const Key& key, const Value& new_value) {
    Node<Key, Value>* node = m_root;
    while (node != nullptr) {
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
Value AVL<Key, Value>::get(const Key& key) const {
    Node<Key, Value>* node = m_root;
    while (node != nullptr) {
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
void AVL<Key, Value>::remove(const Key& key){
    m_root = _remove(m_root, key);
    std::cout << "chave " << key << " removida" << std::endl;
}

template <typename Key, typename Value>
bool AVL<Key, Value>::contains(const Key& k) const {
    return _contains(m_root, k) != nullptr;
}

template <typename Key, typename Value>
void AVL<Key, Value>::forEach(std::function<void(const Key&, const Value&)> func) const {
    std::function<void(Node<Key, Value>*)> inOrder = [&](Node<Key, Value>* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        func(node->key, node->value);
        inOrder(node->right);
    };
    inOrder(m_root);
}

template <typename Key, typename Value>
int AVL<Key, Value>::size() const {
    return m_size;
}

template <typename Key, typename Value>
void AVL<Key, Value>::clear(){
    m_root = _clear(m_root);
    m_size = 0;
}

template <typename Key, typename Value>
void AVL<Key, Value>::print(std::ostream& out) const{
    printInOrder(m_root, out);
}

template <typename Key, typename Value>
int AVL<Key, Value>::height(Node<Key, Value>* node) const {
    if (node == nullptr) return 0;
    return node->height;
}

template <typename Key, typename Value>
int AVL<Key, Value>::balance(Node<Key, Value>* node) const {
    if (node == nullptr) return 0;
    return height(node->right) - height(node->left);
}

template <typename Key, typename Value>
void AVL<Key, Value>::show() const {
    bshow(m_root, "");
}

template <typename Key, typename Value>
size_t AVL<Key, Value>::get_comparisons() const{
    return key_comparisons;
}

template <typename Key, typename Value>
int AVL<Key, Value>::get_left_rotations() const{
    return left_rotates;
}

template <typename Key, typename Value>
int AVL<Key, Value>::get_right_rotations() const{
    return right_rotates;
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::_insert(Node<Key, Value>* node, const Key& k){
    if (node == nullptr){ 
        m_size++;
        return new Node<Key, Value>(k, 1, 1, nullptr, nullptr);
    }
    key_comparisons++; 
    if (k == node->key) {
        node->value++;
        return node;
    }

    key_comparisons++; 
    if (k < node->key) {
        node->left = _insert(node->left, k);
    } else {
        node->right = _insert(node->right, k);
    }

    return fixup_node(node);
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::_remove(Node<Key, Value>* node, const Key& k) {
    if (node == nullptr) return nullptr;

    key_comparisons++;
    if (k < node->key) {
        node->left = _remove(node->left, k);
    } else {
        key_comparisons++;
        if (k > node->key) {
            node->right = _remove(node->right, k);
        } else {
            node = _remove_node(node);
        }
    }

    if (node == nullptr) {
        return nullptr;
    }

    return fixup_node(node);
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::_remove_node(Node<Key, Value>* node) {
    if (node->left == nullptr || node->right == nullptr) {
        Node<Key, Value>* temp = node->left ? node->left : node->right;
        
        if (temp == nullptr) {
            temp = node;
            node = nullptr;
        } else {
            *node = *temp; // Copia os dados do filho não-nulo
        }
        
        delete temp;
    } else {
        Node<Key, Value>* succ = node->right;
        while (succ->left != nullptr) {
            succ = succ->left;
        }
        
        node->key = succ->key;
        node->value = succ->value;
        node->right = _remove(node->right, succ->key);
    }
    
    return node;
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::_contains(Node<Key, Value>* node, const Key& k) const{
    if (node == nullptr) return nullptr;

    key_comparisons++;
    if (node->key == k) {
        return node;
    }

    key_comparisons++;
    if (k < node->key) {
        return _contains(node->left, k);
    } else {
        return _contains(node->right, k);
    }
}

template <typename Key, typename Value>
Node<Key, Value>*AVL<Key, Value>::fixup_node(Node<Key, Value>* node) {
    // Atualiza altura primeiro
    node->height = 1 + std::max(height(node->left), height(node->right));
    
    int bal = balance(node);

    // Caso Left-Left
    if (bal < -1 && balance(node->left) <= 0) {
        return right_rotation(node);
    }
    
    // Caso Left-Right
    if (bal < -1 && balance(node->left) > 0) {
        node->left = left_rotation(node->left);
        return right_rotation(node);
    }
    
    // Caso Right-Right
    if (bal > 1 && balance(node->right) >= 0) {
        return left_rotation(node);
    }
    
    // Caso Right-Left
    if (bal > 1 && balance(node->right) < 0) {
        node->right = right_rotation(node->right);
        return left_rotation(node);
    }
    
    return node;
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::left_rotation(Node<Key, Value>* p){
    Node<Key, Value>* u = p->right;
    p->right = u->left;
    u->left = p;
    p->height = 1 + std::max(height(p->left), height(p->right));
    u->height = 1 + std::max(height(u->left), height(u->right));
    left_rotates++;
    return u;
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::right_rotation(Node<Key, Value>* p){
    Node<Key, Value>* u = p->left;
    p->left = u->right;
    u->right = p;
    p->height = 1 + std::max(height(p->left), height(p->right));
    u->height = 1 + std::max(height(u->left), height(u->right));
    right_rotates++;
    return u;
}

template <typename Key, typename Value>
Node<Key, Value>* AVL<Key, Value>::_clear(Node<Key, Value>* node){
    if (node != nullptr) {
        node->left = _clear(node->left);
        node->right = _clear(node->right);
        delete node;
    }

    return nullptr; 
    std::cout << "Limpeza concluída." << std::endl;
}

template <typename Key, typename Value>
void AVL<Key, Value>::bshow(Node<Key, Value>* node, std::string heranca) const{
    if(node != nullptr && (node->left != nullptr || node->right != nullptr))
        bshow(node->right, heranca + "r");
    for(int i = 0; i < (int) heranca.size() - 1; i++)
        std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
    if(heranca != "")
        std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
    if(node == nullptr){
        std::cout << "#" << std::endl;
        return;
    }
    std::cout << node->key << "(" << node->value << ")" << std::endl;
    if(node != nullptr && (node->left != nullptr || node->right != nullptr))
        bshow(node->left, heranca + "l");
}

template <typename Key, typename Value>
void AVL<Key, Value>::printInOrder(Node<Key, Value>* node, std::ostream& out) const{
    if (!node) return;
    
    printInOrder(node->left, out);
    
    out << node->key << " : " << node->value << '\n';
    
    printInOrder(node->right, out);
}

#endif // AVL_HPP