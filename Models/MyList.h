#pragma once
#include <cstddef>
#include <iostream>

// для элементов списка
template <class T>
class ItemList {
public:
    typedef T objType;
    ItemList() : m_value(nullptr), m_next(nullptr), m_back(nullptr) {}
    explicit ItemList(const T& v) : m_value(new T(v)), m_next(nullptr), m_back(nullptr) {}
    ItemList(const T& v, ItemList<T>* p_b) : m_value(new T(v)), m_back(p_b), m_next(nullptr) {}
    ItemList(const T& v, ItemList<T>* p_b, ItemList<T>* p_n) : m_value(new T(v)), m_back(p_b), m_next(p_n) {}
    ItemList(const ItemList<T>& it) : m_value(it.m_value ? new T(*it.m_value) : nullptr), m_back(it.m_back), m_next(it.m_next) {}
    ~ItemList() { delete m_value; }

    void set(const T& v) { if (m_value) *m_value = v; else m_value = new T(v); }
    T& get() { return *m_value; }
    const T& get() const { return *m_value; }
    void setNext(ItemList<T>* p_n) { m_next = p_n; }
    ItemList<T>* getNext() { return m_next; }
    void setBack(ItemList<T>* p_b) { m_back = p_b; }
    ItemList<T>* getBack() { return m_back; }

    friend std::ostream& operator<<(std::ostream& out, const ItemList<T>& it) {
        if (it.m_value) out << *it.m_value;
        return out;
    }

private:
    T* m_value;
    ItemList<T>* m_next;
    ItemList<T>* m_back;
};

// для управления списком
template <class T>
class MyList {
public:
    template <class V>
    class IteratorList {
    public:
        IteratorList(const IteratorList<V>& it) : m_item(it.m_item) {}
        explicit IteratorList(V* p) : m_item(p) {}
        bool operator==(const IteratorList<V>& it) const { return m_item == it.m_item; }
        bool operator!=(const IteratorList<V>& it) const { return m_item != it.m_item; }
        IteratorList<V>& operator++() { m_item = m_item->getNext(); return *this; }
        V& operator*() const { return *m_item; }
        const V* operator->() const { return m_item; }
    private:
        V* m_item;
    };

    typedef IteratorList<ItemList<T>> iterator;
    typedef IteratorList<ItemList<T>> const_iterator;

    MyList() : m_start(nullptr), m_end(nullptr), m_len(0) {}
    explicit MyList(ItemList<T>* it) { initList(*it); }
    ~MyList() { erase(); }

    bool isEmpty() const { return m_start == nullptr; }
    size_t len() const { return m_len; }

    void add(ItemList<T>* it) {
        if (isEmpty()) {
            initList(*it);
        } else {
            m_end->getBack()->setNext(it);
            it->setBack(m_end->getBack());
            it->setNext(m_end);
            m_end->setBack(it);
            ++m_len;
        }
    }

    void pushstart(ItemList<T>* it) {
        if (isEmpty()) {
            initList(*it);
        } else {
            it->setNext(m_start);
            m_start->setBack(it);
            m_start = it;
            ++m_len;
        }
    }

    void delEnd() {
        if (isEmpty()) return;
        if (len() == 1) {
            delete m_start;
            m_start = nullptr;
            delete m_end;
            m_end = nullptr;
            m_len = 0;
        } else {
            ItemList<T>* temp = m_end->getBack();
            temp->getBack()->setNext(m_end);
            m_end->setBack(temp->getBack());
            delete temp;
            --m_len;
        }
    }

    void delstart() {
        if (isEmpty()) return;
        if (len() == 1) {
            delete m_start;
            m_start = nullptr;
            delete m_end;
            m_end = nullptr;
            m_len = 0;
        } else {
            ItemList<T>* temp = m_start;
            m_start = m_start->getNext();
            m_start->setBack(nullptr);
            delete temp;
            --m_len;
        }
    }

    void erase() {
        while (m_start != nullptr) {
            ItemList<T>* temp = m_start;
            m_start = m_start->getNext();
            delete temp;
        }
        delete m_end;
        m_start = m_end = nullptr;
        m_len = 0;
    }

    void sort() {
        if (m_len <= 1) return;
        ItemList<T>* current = m_start;
        ItemList<T>* sorted = nullptr;
        while (current != m_end) {
            ItemList<T>* next = current->getNext();
            if (sorted == nullptr || sorted->get() > current->get()) {
                current->setNext(sorted);
                if (sorted) sorted->setBack(current);
                current->setBack(nullptr);
                sorted = current;
            } else {
                ItemList<T>* temp = sorted;
                while (temp->getNext() != m_end && temp->getNext()->get() < current->get()) {
                    temp = temp->getNext();
                }
                current->setNext(temp->getNext());
                if (temp->getNext()) temp->getNext()->setBack(current);
                temp->setNext(current);
                current->setBack(temp);
            }
            current = next;
        }
        m_start = sorted;
        while (sorted->getNext() != m_end) {
            sorted = sorted->getNext();
        }
        sorted->setNext(m_end);
        m_end->setBack(sorted);
    }

    iterator begin() { return iterator(m_start); }
    iterator end() { return iterator(m_end); }
    const_iterator begin() const { return const_iterator(m_start); }
    const_iterator end() const { return const_iterator(m_end); }

    friend std::ostream& operator<<(std::ostream& out, const MyList<T>& lst) {
        if (lst.isEmpty()) {
            out << "List is empty\n";
            return out;
        }
        for (const ItemList<T>& it : lst) {
            out << it << "\n---\n";
        }
        return out;
    }

private:
    ItemList<T>* m_start;
    ItemList<T>* m_end;
    size_t m_len;

    void initList(ItemList<T>& it) {
        m_start = &it;
        m_end = new ItemList<T>();
        m_start->setNext(m_end);
        m_end->setBack(m_start);
        m_len = 1;
    }
};