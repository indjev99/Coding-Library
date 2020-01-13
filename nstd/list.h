#pragma once

#include <new>

namespace nstd { template <typename T> class list; }

template <typename T>
class nstd::list
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

private:

    struct node
    {
        pointer val;
        node* next;
        node* prev;
    };
    template <bool Mutable>
    class iterator_t
    {
    public:

        // Types
        typedef std::bidirectional_iterator_tag                                          iterator_category;
        typedef value_type                                                               value_type;
        typedef typename std::conditional<Mutable, value_type&, const value_type&>::type reference;
        typedef typename std::conditional<Mutable, value_type*, const value_type*>::type pointer;
        typedef ptrdiff_t                                                                difference_type;

        // Constructors
        iterator_t()                              noexcept : ptr(nullptr)   {}
        iterator_t(const iterator_t& other)       noexcept : ptr(other.ptr) {}
        template <bool Mut2, typename = std::enable_if_t<!Mutable && Mut2>>
        iterator_t(const iterator_t<Mut2>& other) noexcept : ptr(other.ptr) {}

        // Assignment
        iterator_t& operator=(const iterator_t other)        noexcept { ptr = other.ptr; return *this; }
        template <bool Mut2, typename = std::enable_if_t<!Mutable && Mut2>>
        iterator_t& operator=(const iterator_t<Mut2>& other) noexcept { ptr = other.ptr; return *this; }

        // Access
        reference operator*() const { return *ptr->val; }
        pointer operator->()  const { return ptr->val;  }

        // Iteration
        iterator_t& operator++()    noexcept { ptr = ptr->next; return *this; }
        iterator_t& operator--()    noexcept { ptr = ptr->prev; return *this; }
        iterator_t operator++(int)  noexcept { iterator_t temp = *this; ptr = ptr->next; return temp; }
        iterator_t operator--(int)  noexcept { iterator_t temp = *this; ptr = ptr->prev; return temp; }

        // Comparisons
        template<bool Mut2>
        bool operator==(const iterator_t<Mut2>& other) const noexcept { cerr << "cmp" << endl; return ptr == other.ptr; }
        template<bool Mut2>
        bool operator!=(const iterator_t<Mut2>& other) const noexcept { return ptr != other.ptr; }

    private:

        // Node type
        typedef typename std::conditional<Mutable, node*, const node*>::type node_ptr;

        // Internal constructor
        iterator_t(node_ptr ptr) noexcept : ptr(ptr) {}

        node_ptr ptr;

        friend class list;
    };

public:

    // Types
    typedef iterator_t<true>                      iterator;
    typedef iterator_t<false>                     const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Constructors
    list() noexcept                      : size_(0) { new(&dummy) node{nullptr, &dummy, &dummy}; }

    // Destructor
    ~list()                                noexcept { while (size_ > 0) pop_back(); }

    // Iterators
    iterator begin()                       noexcept { return iterator(dummy.next);       }
    const_iterator begin()           const noexcept { return const_iterator(dummy.next); }
    const_iterator cbegin()          const noexcept { return const_iterator(dummy.next); }
    iterator end()                         noexcept { return iterator(&dummy);           }
    const_iterator end()             const noexcept { return const_iterator(&dummy);     }
    const_iterator cend()            const noexcept { return const_iterator(&dummy);     }
    reverse_iterator rbegin()              noexcept { return reverse_iterator(iterator(dummy.prev));       }
    const_reverse_iterator rbegin()  const noexcept { return reverse_iterator(const_iterator(dummy.prev)); }
    const_reverse_iterator crbegin() const noexcept { return reverse_iterator(const_iterator(dummy.prev)); }
    reverse_iterator rend()                noexcept { return reverse_iterator(iterator(&dummy));           }
    const_reverse_iterator rend()    const noexcept { return reverse_iterator(const_iterator(&dummy));     }
    const_reverse_iterator crend()   const noexcept { return reverse_iterator(const_iterator(&dummy));     }

    // Element access
    reference front()                               { return *dummy.next->val; }
    const_reference front()                   const { return *dummy.next->val; }
    reference back()                                { return *dummy.prev->val; }
    const_reference back()                    const { return *dummy.prev->val; }

    // Size
    bool empty()                     const noexcept { return size_ == 0; }
    size_type size()                 const noexcept { return size_;      }

    // Modifiers
    void push_back(const_reference val)             { create_node(&dummy, val);                }
    void push_back(rvalue_reference val)            { create_node(&dummy, std::move(val));     }
    void push_front(const_reference val)            { create_node(dummy.next, val);            }
    void push_front(rvalue_reference val)           { create_node(dummy.next, std::move(val)); }
    void pop_back()                                 { remove_node(dummy.prev);                 }
    void pop_front()                                { remove_node(dummy.next);                 }

private:

    // Utility functions
    node* create_node(node* loc, const_reference val)
    {
        return create_node_only(loc, new value_type(val));
    }
    node* create_node(node* loc, rvalue_reference val)
    {
        return create_node_only(loc, new value_type(std::move(val)));
    }
    node* create_node_only(node* loc, pointer val)
    {
        ++size_;
        node* curr = new node{val, loc, loc->prev};
        curr->prev->next = curr;
        curr->next->prev = curr;
        return curr;
    }
    void remove_node(node* curr) noexcept
    {
        --size_;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr->val;
        delete curr;
    }

    node dummy;
    size_type size_;
};
