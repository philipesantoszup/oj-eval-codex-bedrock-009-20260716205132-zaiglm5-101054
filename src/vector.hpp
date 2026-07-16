#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
    T *data_;
    size_t size_;
    size_t capacity_;
    
    void expand() {
        if (capacity_ == 0) {
            capacity_ = 1;
        } else {
            capacity_ *= 2;
        }
        T *new_data = static_cast<T*>(operator new(sizeof(T) * capacity_));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(data_[i]);
            data_[i].~T();
        }
        operator delete(data_);
        data_ = new_data;
    }

public:
    class const_iterator;
    class iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

    private:
        T *ptr_;
        const vector *container_;
        
    public:
        iterator(T *p = nullptr, const vector *c = nullptr) : ptr_(p), container_(c) {}
        friend class const_iterator;
        
        iterator operator+(const int &n) const {
            return iterator(ptr_ + n, container_);
        }
        iterator operator-(const int &n) const {
            return iterator(ptr_ - n, container_);
        }
        int operator-(const iterator &rhs) const {
            if (container_ != rhs.container_) {
                throw invalid_iterator();
            }
            return static_cast<int>(ptr_ - rhs.ptr_);
        }
        iterator& operator+=(const int &n) {
            ptr_ += n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            ptr_ -= n;
            return *this;
        }
        iterator operator++(int) { 
            iterator tmp = *this;
            ++ptr_;
            return tmp;
        }
        iterator& operator++() { 
            ++ptr_;
            return *this;
        }
        iterator operator--(int) { 
            iterator tmp = *this;
            --ptr_;
            return tmp;
        }
        iterator& operator--() { 
            --ptr_;
            return *this;
        }
        T& operator*() const { return *ptr_; }
        bool operator==(const iterator &rhs) const { return ptr_ == rhs.ptr_; }
        bool operator==(const const_iterator &rhs) const { return ptr_ == rhs.ptr_; }
        bool operator!=(const iterator &rhs) const { return ptr_ != rhs.ptr_; }
        bool operator!=(const const_iterator &rhs) const { return ptr_ != rhs.ptr_; }
    };
    
    class const_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

    private:
        const T *ptr_;
        const vector *container_;

    public:
        const_iterator(const T *p = nullptr, const vector *c = nullptr) : ptr_(p), container_(c) {}
        const_iterator(const iterator &it) : ptr_(it.ptr_), container_(it.container_) {}
        
        const_iterator operator+(const int &n) const {
            return const_iterator(ptr_ + n, container_);
        }
        const_iterator operator-(const int &n) const {
            return const_iterator(ptr_ - n, container_);
        }
        int operator-(const const_iterator &rhs) const {
            if (container_ != rhs.container_) {
                throw invalid_iterator();
            }
            return static_cast<int>(ptr_ - rhs.ptr_);
        }
        const_iterator& operator+=(const int &n) {
            ptr_ += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            ptr_ -= n;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++ptr_;
            return tmp;
        }
        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --ptr_;
            return tmp;
        }
        const_iterator& operator--() {
            --ptr_;
            return *this;
        }
        const T& operator*() const { return *ptr_; }
        bool operator==(const iterator &rhs) const { return ptr_ == rhs.ptr_; }
        bool operator==(const const_iterator &rhs) const { return ptr_ == rhs.ptr_; }
        bool operator!=(const iterator &rhs) const { return ptr_ != rhs.ptr_; }
        bool operator!=(const const_iterator &rhs) const { return ptr_ != rhs.ptr_; }
    };

    vector() : data_(nullptr), size_(0), capacity_(0) {}
    vector(const vector &other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) T(other.data_[i]);
            }
        }
    }
    ~vector() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        operator delete(data_);
    }
    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        operator delete(data_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        if (capacity_ > 0) {
            data_ = static_cast<T*>(operator new(sizeof(T) * capacity_));
            for (size_t i = 0; i < size_; ++i) {
                new (data_ + i) T(other.data_[i]);
            }
        } else {
            data_ = nullptr;
        }
        return *this;
    }
    T & at(const size_t &pos) {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    const T & at(const size_t &pos) const {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    T & operator[](const size_t &pos) {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    const T & operator[](const size_t &pos) const {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    const T & front() const {
        if (size_ == 0) throw container_is_empty();
        return data_[0];
    }
    const T & back() const {
        if (size_ == 0) throw container_is_empty();
        return data_[size_ - 1];
    }
    iterator begin() { return iterator(data_, this); }
    const_iterator begin() const { return const_iterator(data_, this); }
    const_iterator cbegin() const { return const_iterator(data_, this); }
    iterator end() { return iterator(data_ + size_, this); }
    const_iterator end() const { return const_iterator(data_ + size_, this); }
    const_iterator cend() const { return const_iterator(data_ + size_, this); }
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    iterator insert(iterator pos, const T &value) {
        size_t index = pos - begin();
        if (index > size_) throw index_out_of_bound();
        if (size_ >= capacity_) expand();
        
        // Move elements to make room for new element
        // Create new element at the end using copy constructor
        new (data_ + size_) T(data_[size_ - 1]);
        // Move elements backwards
        for (size_t i = size_ - 1; i > index; --i) {
            data_[i] = data_[i - 1];
        }
        data_[index] = value;
        ++size_;
        
        return iterator(data_ + index, this);
    }
    iterator insert(const size_t &ind, const T &value) {
        if (ind > size_) throw index_out_of_bound();
        if (size_ >= capacity_) expand();
        
        new (data_ + size_) T(data_[size_ - 1]);
        for (size_t i = size_ - 1; i > ind; --i) {
            data_[i] = data_[i - 1];
        }
        data_[ind] = value;
        ++size_;
        
        return iterator(data_ + ind, this);
    }
    iterator erase(iterator pos) {
        size_t index = pos - begin();
        if (index >= size_) throw index_out_of_bound();
        
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        data_[size_ - 1].~T();
        --size_;
        
        return iterator(data_ + index, this);
    }
    iterator erase(const size_t &ind) {
        if (ind >= size_) throw index_out_of_bound();
        
        for (size_t i = ind; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        data_[size_ - 1].~T();
        --size_;
        
        return iterator(data_ + ind, this);
    }
    void push_back(const T &value) {
        if (size_ >= capacity_) expand();
        new (data_ + size_) T(value);
        ++size_;
    }
    void pop_back() {
        if (size_ == 0) throw container_is_empty();
        --size_;
        data_[size_].~T();
    }
};

}

#endif
