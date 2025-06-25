#ifndef CHAINED_HASHTABLE_HPP
#define CHAINED_HASHTABLE_HPP

#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <functional>
#include <stdexcept>
#include <cmath>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class ChainedHashTable {
private:
    std::vector<std::list<std::pair<Key, Value>>> m_table;
    size_t m_table_size;
    size_t m_number_of_elements;
    float m_max_load_factor;
    Hash m_hashing;
    mutable size_t key_comparisons = 0;

    size_t hash_code(const Key& k) const;
    size_t get_next_prime(size_t x);
    void rehash(size_t m);

public:
    ChainedHashTable(size_t tableSize = 19, float load_factor = 1.0);
    ~ChainedHashTable() = default;

    bool add(const Key& k, const Value& v);
    void update(const Key& k, const Value& new_value);
    Value get(const Key& k) const;
    bool remove(const Key& k);
    bool contains(const Key& k) const;
    void forEach(std::function<void(const Key&, const Value&)> func) const;
    void clear();
    size_t size() const;
    size_t bucket_count() const;
    size_t bucket_size(size_t n) const;
    size_t bucket(const Key& k) const;
    float load_factor() const;
    float max_load_factor() const;
    void set_max_load_factor(float lf);
    void reserve(size_t n);
    size_t get_comparisons() const;
};

template <typename Key, typename Value, typename Hash>
ChainedHashTable<Key, Value, Hash>::ChainedHashTable(size_t tableSize, float load_factor) {
    m_table_size = get_next_prime(tableSize);
    m_table.resize(m_table_size);
    m_number_of_elements = 0;
    m_max_load_factor = (load_factor <= 0) ? 1.0 : load_factor;
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::get_next_prime(size_t x) {
    if (x <= 2) return 3;
    x = (x % 2 == 0) ? x + 1 : x;
    while (true) {
        bool not_prime = false;
        for (int i = 3; i <= sqrt(x); i += 2) {
            if (x % i == 0) {
                not_prime = true;
                break;
            }
        }
        if (!not_prime) break;
        x += 2;
    }
    return x;
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::hash_code(const Key& k) const {
    return m_hashing(k) % m_table_size;
}

template <typename Key, typename Value, typename Hash>
bool ChainedHashTable<Key, Value, Hash>::add(const Key& k, const Value& v) {
    if (load_factor() >= m_max_load_factor) {
        rehash(2 * m_table_size);
    }
    size_t slot = hash_code(k);
    for (const auto& p : m_table[slot]) {
        key_comparisons++;
        if (p.first == k) return false;
    }
    m_table[slot].emplace_back(k, v);
    m_number_of_elements++;
    return true;
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::update(const Key& k, const Value& new_value) {
    size_t slot = hash_code(k);
    for (auto& p : m_table[slot]) {
        key_comparisons++;
        if (p.first == k) {
            p.second = new_value;
            return;
        }
    }
    throw std::runtime_error("Chave não encontrada para atualização");
}

template <typename Key, typename Value, typename Hash>
Value ChainedHashTable<Key, Value, Hash>::get(const Key& k) const {
    size_t slot = hash_code(k);
    for (const auto& p : m_table[slot]) {
        key_comparisons++;
        if (p.first == k) return p.second;
    }
    throw std::runtime_error("Chave não encontrada");
}

template <typename Key, typename Value, typename Hash>
bool ChainedHashTable<Key, Value, Hash>::remove(const Key& k) {
    size_t slot = hash_code(k);
    for (auto it = m_table[slot].begin(); it != m_table[slot].end(); ++it) {
        key_comparisons++;
        if (it->first == k) {
            m_table[slot].erase(it);
            m_number_of_elements--;
            return true;
        }
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
bool ChainedHashTable<Key, Value, Hash>::contains(const Key& k) const {
    size_t slot = hash_code(k);
    for (const auto& p : m_table[slot]) {
        if (p.first == k) return true;
    }
    return false;
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::forEach(std::function<void(const Key&, const Value&)> func) const {
    for (const auto& bucket : m_table) {
        for (const auto& p : bucket) {
            func(p.first, p.second);
        }
    }
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::clear() {
    for (auto& bucket : m_table) {
        bucket.clear();
    }
    m_number_of_elements = 0;
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::size() const {
    return m_number_of_elements;
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::bucket_count() const {
    return m_table_size;
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::bucket_size(size_t n) const {
    if (n >= m_table_size) throw std::out_of_range("invalid index");
    return m_table[n].size();
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::bucket(const Key& k) const {
    return hash_code(k);
}

template <typename Key, typename Value, typename Hash>
float ChainedHashTable<Key, Value, Hash>::load_factor() const {
    return static_cast<float>(m_number_of_elements) / m_table_size;
}

template <typename Key, typename Value, typename Hash>
float ChainedHashTable<Key, Value, Hash>::max_load_factor() const {
    return m_max_load_factor;
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::set_max_load_factor(float lf) {
    if (lf <= 0) throw std::out_of_range("invalid load factor");
    m_max_load_factor = lf;
    reserve(m_number_of_elements);
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::reserve(size_t n) {
    if (n > m_table_size * m_max_load_factor) {
        rehash(n / m_max_load_factor);
    }
}

template <typename Key, typename Value, typename Hash>
void ChainedHashTable<Key, Value, Hash>::rehash(size_t m) {
    size_t new_table_size = get_next_prime(m);
    if (new_table_size > m_table_size) {
        std::vector<std::list<std::pair<Key, Value>>> old_table = std::move(m_table);
        m_table.resize(new_table_size);
        for (auto& bucket : m_table) bucket.clear();
        m_table_size = new_table_size;
        m_number_of_elements = 0;
        for (const auto& bucket : old_table) {
            for (const auto& p : bucket) {
                add(p.first, p.second);
            }
        }
    }
}

template <typename Key, typename Value, typename Hash>
size_t ChainedHashTable<Key, Value, Hash>::get_comparisons() const {
    return key_comparisons;
}

#endif // CHAINED_HASHTABLE_HPP