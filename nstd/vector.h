#pragma once

#include <iterator>
#include <exception>
#include <new>
#include "algorithm.h"

namespace nstd { template <typename T> class vector; }

template <typename T>
class nstd::vector
{
public:

    // Types
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type&&                          rvalue_reference;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Constructors
    vector() noexcept : size_(0), capacity_(0), data_(nullptr) {}
    vector(size_type cnt) : size_(cnt), capacity_(size_)
    {
        data_ = allocate_data(capacity_);
        nstd::construct(begin(), end());
    }
    vector(size_type cnt, const_reference val) : size_(cnt), capacity_(size_)
    {
        data_ = allocate_data(capacity_);
        nstd::construct_fill(begin(), end(), val);
    }
    vector(const vector& other) : size_(other.size()), capacity_(size_)
    {
        data_ = allocate_data(capacity_);
        nstd::construct_copy(other.begin(), other.end(), begin());
    }
    vector(vector&& other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_)
    {
        other.steal_contents();
    }
    vector(std::initializer_list<value_type> vals) : size_(vals.size()), capacity_(size_)
    {
        data_ = allocate_data(capacity_);
        nstd::construct_copy(vals.begin(), vals.end(), begin());
    }
    template <typename InputIter, typename = std::_RequireInputIter<InputIter>>
    vector(InputIter first, InputIter last) : vector()
    {
        nstd::copy(first, last, std::back_insert_iterator<vector>(*this));
    }

    // Destructor
    ~vector() noexcept
    {
        destroy_data();
    }

    // Assignment
    vector& operator=(const vector& other)
    {
        if (this == &other) return *this;
        new_data(other.size());
        nstd::construct_copy(other.begin(), other.end(), begin());
        return *this;
    }
    vector& operator=(vector&& other) noexcept
    {
        if (this == &other) return *this;
        destroy_data();
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.steal_contents();
        return *this;
    }
    void assign(size_type cnt, const_reference val)
    {
        new_data(cnt);
        nstd::construct_fill_n(begin(), cnt, val);
    }
    void assign(std::initializer_list<value_type> vals)
    {
        new_data(vals.size());
        nstd::construct_copy(vals.begin(), vals.end(), begin());
    }
    template <typename InputIter, typename = std::_RequireInputIter<InputIter>>
    void assign(InputIter first, InputIter last)
    {
        clear();
        nstd::copy(first, last, std::back_insert_iterator<vector>(*this));
    }

    // Iterators
    iterator begin()                       noexcept { return data_;         }
    const_iterator begin()           const noexcept { return data_;         }
    const_iterator cbegin()          const noexcept { return data_;         }
    iterator end()                         noexcept { return data_ + size_; }
    const_iterator end()             const noexcept { return data_ + size_; }
    const_iterator cend()            const noexcept { return data_ + size_; }
    reverse_iterator rbegin()              noexcept { return reverse_iterator(data_ + size_); }
    const_reverse_iterator rbegin()  const noexcept { return reverse_iterator(data_ + size_); }
    const_reverse_iterator crbegin() const noexcept { return reverse_iterator(data_ + size_); }
    reverse_iterator rend()                noexcept { return reverse_iterator(data_);         }
    const_reverse_iterator rend()    const noexcept { return reverse_iterator(data_);         }
    const_reverse_iterator crend()   const noexcept { return reverse_iterator(data_);         }

    // Size
    bool empty()                     const noexcept { return size_ == 0; }
    size_type size()                 const noexcept { return size_;      }
    size_type max_size()             const noexcept { return SIZE_MAX;   }
    size_type capacity()             const noexcept { return capacity_;  }
    void reserve(size_type cnt)                     { if (capacity_ < cnt) change_capacity(cnt);    }
    void shrink_to_fit()                            { if (capacity > size_) change_capacity(size_); }

    // Element access
    reference operator[](size_type idx)             { return data_[idx];       }
    const_reference operator[](size_type idx) const { return data_[idx];       }
    reference front()                               { return data_[0];         }
    const_reference front()                   const { return data_[0];         }
    reference back()                                { return data_[size_ - 1]; }
    const_reference back()                    const { return data_[size_ - 1]; }
    pointer data()                         noexcept { return data_;            }
    const_pointer data()             const noexcept { return data_;            }
    reference at(size_type idx)                     { if (idx >= size_) throw std::out_of_range(); return data_[idx]; }
    const_reference at(size_type idx)         const { if (idx >= size_) throw std::out_of_range(); return data_[idx]; }

    // Modifiers
    void clear()                           noexcept { shrink_resize(0);             }
    void push_back(const_reference val)             { emplace_back(val);            }
    void push_back(rvalue_reference val)            { emplace_back(std::move(val)); }
    void pop_back()                                 { data_[--size_].~value_type(); }
    iterator insert(const_iterator pos, const_reference val)
    {
        return emplace(pos, val);
    }
    iterator insert(const_iterator pos, rvalue_reference val)
    {
        return emplace(pos, std::move(val));
    }
    iterator insert(const_iterator pos, size_type cnt, const_reference val)
    {
        difference_type offset = pos - begin();
        iterator mid = shift_right(offset, cnt);
        iterator new_pos = iter_at(offset);
        nstd::fill(new_pos, mid, val);
        nstd::construct_fill(mid, new_pos + cnt, val);
        return new_pos;
    }
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals)
    {
        difference_type offset = pos - begin();
        iterator mid = shift_right(offset, vals.size());
        iterator new_pos = iter_at(offset);
        auto curr = nstd::copy_to(vals.begin(), new_pos, mid);
        nstd::construct_copy(curr, vals.end(), mid);
        return new_pos;
    }
    template <typename InputIter, typename = std::_RequireInputIter<InputIter>>
    iterator insert(const_iterator pos, InputIter first, InputIter last)
    {
        difference_type offset = pos - begin();
        nstd::copy(first, last, std::insert_iterator<vector>(*this, iter_at(offset)));
        return iter_at(offset);
    }
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args)
    {
        difference_type offset = pos - begin();
        iterator mid = shift_right(offset, 1);
        iterator new_pos = iter_at(offset);
        iterator left = new_pos;
        if (left != mid)
            *left = std::move(value_type(std::forward<Args>(args)...));
        else
            new(left) value_type(std::forward<Args>(args)...);
        return new_pos;
    }
    template <typename... Args>
    reference emplace_back(Args&&... args)
    {
        expand(size_ + 1);
        reference obj = data_[size_++];
        new(&obj) value_type(std::forward<Args>(args)...);
        return obj;
    }
    iterator erase(const_iterator pos)
    {
        difference_type offset = pos - begin();
        shift_left(pos + 1 - begin(), 1);
        return iter_at(offset);
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        difference_type offset = first - begin();
        shift_left(last - begin(), last - first);
        return iter_at(offset);
    }
    void resize(size_type cnt)
    {
        if (cnt < size_) shrink_resize(cnt);
        else
        {
            expand(cnt);
            nstd::construct(end(), iter_at(cnt));
            size_ = cnt;
        }
    }
    void resize(size_type cnt, const_reference val)
    {
        if (cnt < size_) shrink_resize(cnt);
        else
        {
            expand(cnt);
            nstd::construct_fill(end(), iter_at(cnt), val);
            size_ = cnt;
        }
    }
    void swap(vector& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

private:

    // Utility functions
    iterator iter_at(size_type idx) noexcept
    {
        return data_ + idx;
    }
    void shrink_resize(size_type cnt) noexcept
    {
        nstd::destruct(iter_at(cnt), end());
        size_ = cnt;
    }
    size_type expand_size(size_type cnt) const noexcept
    {
        return nstd::max(capacity_ * 2, cnt);
    }
    void expand(size_type cnt)
    {
        if (capacity_ < cnt) change_capacity(expand_size(cnt));
    }
    void shift_left(difference_type from, difference_type dist)
    {
        nstd::move(iter_at(from), end(), iter_at(from - dist));
        shrink_resize(size_ - dist);
    }
    iterator shift_right(difference_type from, difference_type dist)
    {
        size_type new_size = size_ + dist;
        if (capacity_ < new_size)
        {
            size_type new_capacity = expand_size(new_size);
            pointer new_data = allocate_data(new_capacity);
            nstd::construct_move(begin(), iter_at(from), new_data);
            nstd::construct_move(iter_at(from), end(), new_data + from + dist);
            destroy_data();
            data_ = new_data;
            size_ = new_size;
            capacity_ = new_capacity;
            return iter_at(from);
        }
        else
        {
            size_type num = nstd::min(size_ - from, size_type(dist));
            iterator mid = nstd::construct_move_backward_n(end(), num, iter_at(new_size));
            nstd::move_backward(iter_at(from), iter_at(size_ - num), mid);
            iterator ret = nstd::min(end(), iter_at(from + dist));
            size_ = new_size;
            return ret;
        }
    }
    void change_capacity(size_type cnt)
    {
        pointer new_data = allocate_data(cnt);
        nstd::construct_move(data_, data_ + size_, new_data);
        destroy_data();
        data_ = new_data;
        capacity_ = cnt;
    }
    void new_data(size_type cnt)
    {
        clear();
        reserve(cnt);
        size_ = cnt;
    }
    void destroy_data() noexcept
    {
        nstd::destruct(begin(), end());
        deallocate_data(data_, capacity_);
    }

    // Data management
    static pointer allocate_data(size_type cnt)
    {
        return static_cast<pointer>(::operator new(cnt * sizeof(value_type)));
    }
    static void deallocate_data(pointer data, size_type cnt)
    {
        ::operator delete(data);
    }

    // Leave in valid state on move
    void steal_contents() { size_ = 0; capacity_ = 0; data_ = nullptr; }

    pointer data_;
    size_type size_;
    size_type capacity_;
};

template <typename T>
void std::swap(nstd::vector<T>& lhs, nstd::vector<T>& rhs)
{
    lhs.swap(rhs);
}
template <typename T>
bool operator==(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    if (lhs.size() != rhs.size()) return false;
    return nstd::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T>
bool operator!=(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    if (lhs.size() != rhs.size()) return true;
    return !nstd::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T>
bool operator<(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    return nstd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename T>
bool operator<=(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    return !nstd::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}
template <typename T>
bool operator>(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    return nstd::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}
template <typename T>
bool operator>=(const nstd::vector<T>& lhs, const nstd::vector<T>& rhs)
{
    return !nstd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
