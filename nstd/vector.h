#ifndef NSTD_VECTOR_H_INCLUDED
#define NSTD_VECTOR_H_INCLUDED

#include <iterator>
#include <algorithm>

namespace nstd { template <typename T> class vector; }

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
    typedef std::allocator<value_type>            allocator_type;

    /// TO-DO get_allocator

    // Constructors
    vector()                              noexcept : size_(0), capacity_(0), data_(nullptr) {}
    vector(size_type n)                            : size_(n), capacity_(n), data_(new value_type[capacity_]) {}
    vector(size_type n, const_reference val)       : vector(n) { std::fill(data_, data_ + size_, val); }
    vector(const vector& other)                    : vector(other.size_) { std::copy(other.data_, other.data_ + size_, data_); }
    vector(vector&& other)                noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) { other.steal_contents(); }
    vector(std::initializer_list<value_type> vals) : vector(vals.size()) { std::move(vals.begin(), vals.end(), data_); }
    template <class input_iter>
    vector(input_iter first, input_iter last)      : vector() {std::copy(first, last, std::back_insert_iterator<vector>(*this)); }

    // Destructor
    ~vector() { delete[] data_; }

    // Operator =
    vector& operator=(const vector& other)
    {
        if (this != &other)
        {
            allocate_new_data(other.size_);
            std::copy(other.data_, other.data_ + size_, data_);
        }
        return *this;
    }
    vector& operator=(vector&& other) noexcept
    {
        if (this != &other)
        {
            size_ = other.size_;
            capacity_ = other.capacity_;
            delete[] data_;
            data_ = other.data_;
            other.steal_contents();
        }
        return *this;
    }

    // Assign
    void assign(size_type n, const value_type& val)     { allocate_new_data(n); std::fill(data_, data_ + size_, val);                 }
    void assign(std::initializer_list<value_type> vals) { allocate_new_data(vals.size()); std::copy(vals.begin(), vals.end(), data_); }
    template <class input_iter>
    void assign(input_iter first, input_iter last)      { clear(); std::copy(first, last, std::back_insert_iterator<vector>(*this));  }

    // Iterators
    iterator begin()                      noexcept { return data_;         }
    const_iterator begin()          const noexcept { return data_;         }
    const_iterator cbegin()         const noexcept { return begin();       }
    iterator end()                        noexcept { return data_ + size_; }
    const_iterator end()            const noexcept { return data_ + size_; }
    const_iterator cend()           const noexcept { return end();         }
    reverse_iterator rbegin()             noexcept { return reverse_iterator(data_ + size_); }
    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(data_ + size_); }
    const_iterator crbegin()        const noexcept { return rbegin();                        }
    reverse_iterator rend()               noexcept { return reverse_iterator(data_);         }
    const_reverse_iterator rend()   const noexcept { return reverse_iterator(data_);         }
    const_iterator crend()          const noexcept { return rend();                          }

    // Capacity
    bool empty()                    const noexcept { return size_ == 0; }
    size_type size()                const noexcept { return size_;      }
    size_type max_size()            const noexcept { return SIZE_MAX;   }
    size_type capacity()            const noexcept { return capacity_;  }
    void reserve(size_type n)                      { if (capacity_ < n) change_capacity(n);        }
    void shrink_to_fit()                           { if (capacity > size_) change_capacity(size_); }

    // Element access
    reference operator[](size_type i)              { return data_[i];         }
    const_reference operator[](size_type i)  const { return data_[i];         }
    reference front()                              { return data_[0];         }
    const_reference front()                  const { return data_[0];         }
    reference back()                               { return data_[size_ - 1]; }
    const_reference back()                   const { return data_[size_ - 1]; }
    pointer data()                                 { return data_;            }
    const_pointer data()                     const { return data_;            }
    reference at(size_type i)                      { if (i >= size_) throw std::out_of_range(); return data_[i]; }
    const_reference at(size_type i)          const { if (i >= size_) throw std::out_of_range(); return data_[i]; }

    // Modifiers
    void clear()                          noexcept { size_ = 0;                      }
    void push_back(value_type val)                 { expand(); data_[size_++] = val; }
    void pop_back()                                { --size_;                        }
    void resize(size_type n)                       { reserve(n); size_ = n;          }
    void resize(size_type n, const_reference val)
    {
        size_t old_size = size_;
        resize(n);
        if (old_size < size_) std::fill(data_ + old_size, data_ + size_, val);
    }
    void swap(vector& other) noexcept
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }
    /// TO-DO: Other Modifiers

private:

    void expand() { if (size_ == capacity_) change_capacity(capacity_ ? capacity_ * 2 : 1); }

    void change_capacity(size_type n)
    {
        capacity_ = n;
        value_type* new_data = new value_type[n];
        std::move(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
    }
    void allocate_new_data(size_type n)
    {
        size_ = n;
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

#endif // NSTD_VECTOR_H_INCLUDED
