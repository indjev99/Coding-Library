#pragma once

#include <iterator>
#include <algorithm>

namespace nstd { template <typename T> class vector; }

/// TO-DO: Fix to use allocators

template <typename T>
class nstd::vector
{
public:

    // Types
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;

    // Constructors
    vector()                              noexcept : size_(0), capacity_(0), data_(nullptr) {}
    vector(size_type cnt)                          : size_(cnt), capacity_(cnt), data_(new value_type[capacity_]) {}
    vector(size_type cnt, const_reference val)     : vector(cnt)         { std::fill(data_, data_ + size_, val); }
    vector(const vector& other)                    : vector(other.size_) { std::copy(other.data_, other.data_ + size_, data_); }
    vector(vector&& other)                noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) { other.steal_contents(); }
    vector(std::initializer_list<value_type> vals) : vector(vals.size()) { std::copy(vals.begin(), vals.end(), data_); }
    template <class InputIter, typename = std::_RequireInputIter<InputIter>>
    vector(InputIter first, InputIter last)        : vector()            { std::copy(first, last, std::back_insert_iterator<vector>(*this)); }

    // Destructor
    ~vector() { delete[] data_; }

    // Operator =
    vector& operator=(const vector& other)
    {
        if (this == &other) return *this;
        allocate_new_data(other.size_);
        std::copy(other.data_, other.data_ + size_, data_);
        return *this;
    }
    vector& operator=(vector&& other) noexcept
    {
        if (this == &other) return *this;
        size_ = other.size_;
        capacity_ = other.capacity_;
        delete[] data_;
        data_ = other.data_;
        other.steal_contents();
        return *this;
    }

    // Assign
    void assign(size_type cnt, const value_type& val)   { allocate_new_data(cnt); std::fill(data_, data_ + size_, val);               }
    void assign(std::initializer_list<value_type> vals) { allocate_new_data(vals.size()); std::copy(vals.begin(), vals.end(), data_); }
    template <class InputIter, typename = std::_RequireInputIter<InputIter>>
    void assign(InputIter first, InputIter last)        { clear(); std::copy(first, last, std::back_insert_iterator<vector>(*this));  }

    // Iterators
    iterator begin()                      noexcept { return data_;         }
    const_iterator begin()          const noexcept { return data_;         }
    const_iterator cbegin()         const noexcept { return data_;         }
    iterator end()                        noexcept { return data_ + size_; }
    const_iterator end()            const noexcept { return data_ + size_; }
    const_iterator cend()           const noexcept { return data_ + size_; }
    reverse_iterator rbegin()             noexcept { return reverse_iterator(data_ + size_); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(data_ + size_); }
    const_reverse_iterator crbegin()const noexcept { return reverse_iterator(data_ + size_); }
    reverse_iterator rend()               noexcept { return reverse_iterator(data_);         }
    const_reverse_iterator rend()   const noexcept { return reverse_iterator(data_);         }
    const_reverse_iterator crend()  const noexcept { return reverse_iterator(data_);         }

    // Capacity
    bool empty()                    const noexcept { return size_ == 0; }
    size_type size()                const noexcept { return size_;      }
    size_type max_size()            const noexcept { return SIZE_MAX;   }
    size_type capacity()            const noexcept { return capacity_;  }
    void reserve(size_type cnt)                    { if (capacity_ < cnt) change_capacity(cnt);    }
    void shrink_to_fit()                           { if (capacity > size_) change_capacity(size_); }

    // Element access
    reference operator[](size_type idx)            { return data_[idx];       }
    const_reference operator[](size_type idx)const { return data_[idx];       }
    reference front()                              { return data_[0];         }
    const_reference front()                  const { return data_[0];         }
    reference back()                               { return data_[size_ - 1]; }
    const_reference back()                   const { return data_[size_ - 1]; }
    pointer data()                        noexcept { return data_;            }
    const_pointer data()            const noexcept { return data_;            }
    reference at(size_type idx)                    { if (idx >= size_) throw std::out_of_range(); return data_[idx]; }
    const_reference at(size_type idx)        const { if (idx >= size_) throw std::out_of_range(); return data_[idx]; }

    // Modifiers
    void clear()                          noexcept { size_ = 0;                      }
    void push_back(const value_type& val)          { expand(); data_[size_++] = val; }
    void push_back(value_type&& val)               { expand(); data_[size_++] = val; }
    void pop_back()                                { --size_;                        }
    void resize(size_type cnt)                     { reserve(cnt); size_ = cnt;      }
    iterator insert(const_iterator pos, const value_type& val)
    {
        iterator new_pos = shift_right(pos - data_, 1);
        *new_pos = val;
        return new_pos;
    }
    iterator insert(const_iterator pos, value_type&& val)
    {
        iterator new_pos = shift_right(pos - data_, 1);
        *new_pos = val;
        return new_pos;
    }
    iterator insert(const_iterator pos, size_type cnt, const value_type& val)
    {
        iterator new_pos = shift_right(pos - data_, cnt);
        std::fill(new_pos, new_pos + cnt, val);
        return new_pos;
    }
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals)
    {
        iterator new_pos = shift_right(pos - data_, vals.size());
        std::copy(vals.begin(), vals.end(), new_pos);
        return new_pos;
    }
    template <class InputIter, typename = std::_RequireInputIter<InputIter>>
    iterator insert(const_iterator pos, InputIter first, InputIter last)
    {
        difference_type offset = pos - data_;
        std::copy(first, last, std::insert_iterator<vector>(*this, data_ + offset));
        return data_ + offset;
    }
    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        iterator new_pos = shift_right(pos - data_, 1);
        reference obj = *new_pos;
        obj.~value_type();
        new(&obj) value_type(args...);
        return new_pos;
    }
    template <class... Args>
    reference emplace_back(Args&&... args)
    {
        expand();
        reference obj = data_[size_++];
        obj.~value_type();
        new(&obj) value_type(args...);
        return obj;
    }
    iterator erase(const_iterator pos)
    {
        shift_left(pos + 1 - data_, 1);
        return data_ + (pos - data_);
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        shift_left(last - data_, last - first);
        return data_ + (first - data_);
    }
    void resize(size_type cnt, const_reference val)
    {
        size_t old_size = size_;
        resize(cnt);
        if (old_size < size_) std::fill(data_ + old_size, data_ + size_, val);
    }
    void swap(vector& other) noexcept
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

private:

    void shift_left(difference_type from, difference_type dist)
    {
        std::move(data_ + from, data_ + size_, data_ + from - dist);
        size_ -= dist;
    }
    iterator shift_right(difference_type from, difference_type dist)
    {
        size_type new_size = size_ + dist;
        iterator from_it = data_ + from;
        if (capacity_ < new_size)
        {
            capacity_ *= 2;
            if (capacity_ < new_size) capacity_ = new_size;
            value_type* new_data = new value_type[capacity_];
            std::move(data_, from_it, new_data);
            std::move(from_it, data_ + size_, new_data + from + dist);
            delete[] data_;
            data_ = new_data;
        }
        else
        {
            std::move_backward(from_it, data_ + size_, data_ + size_ + dist);
        }
        size_ = new_size;
        return data_ + from;
    }
    void expand()
    {
        if (size_ == capacity_) change_capacity(capacity_ ? capacity_ * 2 : 1);
    }
    void change_capacity(size_type cnt)
    {
        capacity_ = cnt;
        value_type* new_data = new value_type[capacity_];
        std::move(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
    }
    void allocate_new_data(size_type cnt)
    {
        size_ = cnt;
        if (capacity_ < size_)
        {
            capacity_ = size_;
            delete[] data_;
            data_ = new value_type[capacity_];
        }
    }

    void steal_contents() { size_ = 0; capacity_ = 0; data_ = nullptr; }

    size_type size_;
    size_type capacity_;
    value_type* data_;
};

/// TO-DO: Overload operators and std::swap
