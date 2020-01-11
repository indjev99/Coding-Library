#pragma once

namespace nstd { template <typename T> class list; }

template <typename T>
class nstd::list
{
public:

    // Types
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;

private:

    struct node
    {
        value_type* val;
        node* next;
        node* prev;
    };
    template <typename V, typename N>
    class iterator_t
    {
    public:

        // Types
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef V                               value_type;
        typedef value_type&                     reference;
        typedef value_type*                     pointer;
        typedef ptrdiff_t                       difference_type;

        // Constructors
        iterator_t(): ptr(nullptr) {}
        iterator_t(const iterator_t& other): ptr(other.ptr) {}

        // Assignment
        iterator_t& operator=(const iterator_t& other) { ptr = other.ptr; }

        // Access
        reference operator*() const { return *ptr->val; }
        pointer operator->()  const { return ptr->val;  }

        // Iteration
        iterator_t& operator++()    { ptr = ptr->next; return *this; }
        iterator_t& operator--()    { ptr = ptr->prev; return *this; }
        iterator_t operator++(int)  { iterator_t temp = *this; ptr = ptr->next; return temp; }
        iterator_t operator--(int)  { iterator_t temp = *this; ptr = ptr->prev; return temp; }

        // Comparisons
        bool operator==(const iterator_t& other) { return ptr == other.ptr; }
        bool operator!=(const iterator_t& other) { return ptr != other.ptr; }

    private:

        iterator_t(N* curr): ptr(curr) {}
        N* ptr;

        friend class list;
    };

public:

    // Types
    typedef iterator_t<value_type, node>             iterator;
    typedef iterator_t<const value_type, const node> const_iterator;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

    // Constructors
    list() noexcept             : size_(0) { new(&dummy) node{nullptr, &dummy, &dummy}; }

    // Destructor
    ~list()                                { while (size_ > 0) pop_back(); }

    // Iterators
    iterator begin()                       { return iterator(dummy.next);       }
    const_iterator begin()           const { return const_iterator(dummy.next); }
    const_iterator cbegin()          const { return const_iterator(dummy.next); }
    iterator end()                         { return iterator(&dummy);           }
    const_iterator end()             const { return const_iterator(&dummy);     }
    const_iterator cend()            const { return const_iterator(&dummy);     }
    reverse_iterator rbegin()              { return reverse_iterator(iterator(dummy.prev));       }
    const_reverse_iterator rbegin()  const { return reverse_iterator(const_iterator(dummy.prev)); }
    const_reverse_iterator crbegin() const { return reverse_iterator(const_iterator(dummy.prev)); }
    reverse_iterator rend()                { return reverse_iterator(iterator(&dummy));           }
    const_reverse_iterator rend()    const { return reverse_iterator(const_iterator(&dummy));     }
    const_reverse_iterator crend()   const { return reverse_iterator(const_iterator(&dummy));     }

    // Element access
    reference front()                      { return *dummy.next->val; }
    const_reference front()          const { return *dummy.next->val; }
    reference back()                       { return *dummy.prev->val; }
    const_reference back()           const { return *dummy.prev->val; }

    // Size
    bool empty()            const noexcept { return size_ == 0; }
    size_type size()        const noexcept { return size_;      }

    // Modifiers
    void push_back(const_reference val)    { create_node(&dummy, val);                }
    void push_back(value_type&& val)       { create_node(&dummy, std::move(val));     }
    void push_front(const_reference val)   { create_node(dummy.next, val);            }
    void push_front(value_type&& val)      { create_node(dummy.next, std::move(val)); }
    void pop_back()                        { remove_node(dummy.prev);                 }
    void pop_front()                       { remove_node(dummy.next);                 }

private:

    node* create_node(node* loc, const_reference val)
    {
        ++size_;
        node* curr =  new node{new value_type(val), loc, loc->prev};
        curr->prev->next = curr;
        curr->next->prev = curr;
        return curr;
    }
    node* create_node(node* loc, value_type&& val)
    {
        ++size_;
        node* curr =  new node{new value_type(std::move(val)), loc, loc->prev};
        curr->prev->next = curr;
        curr->next->prev = curr;
        return curr;
    }
    void remove_node(node* curr)
    {
        ++size_;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr->val;
        delete curr;
    }

    size_type size_;
    node dummy;
};
