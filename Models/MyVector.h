#pragma once
#include <cstddef>
#include <iostream>
#include <new>

template <typename T>
class MyVector {
public:
    class Iterator {
        friend class MyVector;
    public:
        Iterator(const Iterator& it) : m_ptr(it.m_ptr) {}
        bool operator==(const Iterator& it) const { return m_ptr == it.m_ptr; }
        bool operator!=(const Iterator& it) const { return m_ptr != it.m_ptr; }
        Iterator& operator++() { ++m_ptr; return *this; }
        T& operator*() const { return *m_ptr; }
    private:
        T* m_ptr;
        Iterator(T* p) : m_ptr(p) {}
    };

    typedef Iterator iterator;
    typedef Iterator const_iterator;

    MyVector() : m_array(nullptr), m_end(nullptr), m_size(0), m_capacity(0) {}

    MyVector(size_t size) : m_size(size), m_capacity(size) {
        m_array = static_cast<T*>(operator new[](m_size * sizeof(T)));
        m_end = m_array + m_size;
        for (size_t i = 0; i < m_size; ++i) {
            new (&m_array[i]) T();
        }
    }

    MyVector(size_t size, const T& value) : m_size(size), m_capacity(size) {
        m_array = static_cast<T*>(operator new[](m_size * sizeof(T)));
        for (size_t i = 0; i < m_size; ++i) {
            new (&m_array[i]) T(value);
        }
        m_end = m_array + m_size;
    }

    ~MyVector() {
        for (size_t i = 0; i < m_size; ++i) {
            m_array[i].~T();
        }
        operator delete[](m_array);
    }

    void push_back(const T& value) {
        if (m_size >= m_capacity) {
            size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
            T* new_array = static_cast<T*>(operator new[](new_capacity * sizeof(T)));
            for (size_t i = 0; i < m_size; ++i) {
                new (&new_array[i]) T(m_array[i]); // копируем существующие элементы
                m_array[i].~T();
            }
            operator delete[](m_array);
            m_array = new_array;
            m_capacity = new_capacity;
            m_end = m_array + m_size;
        }
        new (&m_array[m_size]) T(value);
        ++m_size;
        m_end = m_array + m_size;
    }

    void erase(size_t index) {
        if (index >= m_size) return;
        for (size_t i = index; i < m_size - 1; ++i) {
            m_array[i] = m_array[i + 1];
        }
        --m_size;
        m_array[m_size].~T();
        m_end = m_array + m_size;
    }

    T& operator[](size_t index) { return m_array[index]; }
    const T& operator[](size_t index) const { return m_array[index]; }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    iterator begin() { return iterator(m_array); }
    iterator end() { return iterator(m_end); }
    const_iterator begin() const { return const_iterator(m_array); }
    const_iterator end() const { return const_iterator(m_end); }

    void sort() {
        for (size_t i = 0; i < m_size - 1; ++i) {
            for (size_t j = 0; j < m_size - i - 1; ++j) {
                if (m_array[j] > m_array[j + 1]) {
                    T temp = m_array[j];
                    m_array[j] = m_array[j + 1];
                    m_array[j + 1] = temp;
                }
            }
        }
    }

    MyVector filter(bool (*predicate)(const T&)) {
        MyVector result;
        for (size_t i = 0; i < m_size; ++i) {
            if (predicate(m_array[i])) {
                result.push_back(m_array[i]);
            }
        }
        return result;
    }

private:
    T* m_array;
    T* m_end;
    size_t m_size;
    size_t m_capacity;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MyVector<T>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i] << "\n---\n";
    }
    return os;
}