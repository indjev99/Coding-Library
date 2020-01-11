#pragma once

#include <utility>

namespace nstd
{
    template <typename T>
    constexpr const T& min(const T&, const T&);
    template <typename T, typename Compare>
    constexpr const T& min(const T&, const T&, Compare cmp);

    template <typename T>
    constexpr const T& max(const T&, const T&);
    template <typename T, typename Compare>
    constexpr const T& max(const T&, const T&, Compare cmp);

    template <typename InputIter1, typename InputIter2>
    constexpr bool equal(InputIter1, InputIter1, InputIter2);
    template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
    constexpr bool equal(InputIter1, InputIter1, InputIter2, BinaryPredicate);
    template <typename InputIter1, typename InputIter2>
    constexpr bool equal(InputIter1, InputIter1, InputIter2, InputIter2);
    template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
    constexpr bool equal(InputIter1, InputIter1, InputIter2, InputIter2, BinaryPredicate);

    template <typename InputIter1, typename InputIter2>
    constexpr bool lexicographical_compare(InputIter1, InputIter1, InputIter2, InputIter2);
    template <typename InputIter1, typename InputIter2, typename Compare>
    constexpr bool lexicographical_compare(InputIter1, InputIter1, InputIter2, InputIter2, Compare);

    template <typename OutputIter, typename T>
    OutputIter fill(OutputIter, OutputIter, const T&);
    template <typename OutputIter, typename Size, typename T>
    OutputIter fill_n(OutputIter, Size, const T&);

    template <typename InputIter, typename OutputIter>
    OutputIter copy(InputIter, InputIter, OutputIter);
    template <typename InputIter, typename OutputIter>
    InputIter copy_to(InputIter, OutputIter, OutputIter);
    template <typename InputIter, typename Size, typename OutputIter>
    OutputIter copy_n(InputIter, Size, OutputIter);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 copy_backward(BidirIt1, BidirIt1, BidirIt2);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt1 copy_backward_to(BidirIt1, BidirIt2, BidirIt2);
    template <typename BidirIt1, typename Size, typename BidirIt2>
    BidirIt2 copy_backward_n(BidirIt1, Size, BidirIt2);

    template <typename InputIter, typename OutputIter>
    OutputIter move(InputIter, InputIter, OutputIter);
    template <typename InputIter, typename OutputIter>
    InputIter move_to(InputIter, OutputIter, OutputIter);
    template <typename InputIter, typename Size, typename OutputIter>
    OutputIter move_n(InputIter, Size, OutputIter);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 move_backward(BidirIt1, BidirIt1, BidirIt2);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt1 move_backward_to(BidirIt1, BidirIt2, BidirIt2);
    template <typename BidirIt1, typename Size, typename BidirIt2>
    BidirIt2 move_backward_n(BidirIt1, Size, BidirIt2);

    template <typename OutputIter, typename... Args>
    OutputIter construct(OutputIter, OutputIter, Args&&...);
    template <typename OutputIter, typename Size, typename... Args>
    OutputIter construct_n(OutputIter, Size, Args&&...);

    template <typename OutputIter, typename T>
    OutputIter construct_fill(OutputIter, OutputIter, const T&);
    template <typename OutputIter, typename Size, typename T>
    OutputIter construct_fill_n(OutputIter, Size, const T&);

    template <typename InputIter, typename OutputIter>
    OutputIter construct_copy(InputIter, InputIter, OutputIter);
    template <typename InputIter, typename OutputIter>
    InputIter construct_copy_to(InputIter, OutputIter, OutputIter);
    template <typename InputIter, typename Size, typename OutputIter>
    OutputIter construct_copy_n(InputIter, Size, OutputIter);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 construct_copy_backward(BidirIt1, BidirIt1, BidirIt2);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt1 construct_copy_backward_to(BidirIt1, BidirIt2, BidirIt2);
    template <typename BidirIt1, typename Size, typename BidirIt2>
    BidirIt2 construct_copy_backward_n(BidirIt1, Size, BidirIt2);

    template <typename InputIter, typename OutputIter>
    OutputIter construct_move(InputIter, InputIter, OutputIter);
    template <typename InputIter, typename OutputIter>
    InputIter construct_move_to(InputIter, OutputIter, OutputIter);
    template <typename InputIter, typename Size, typename OutputIter>
    OutputIter construct_move_n(InputIter, Size, OutputIter);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt2 construct_move_backward(BidirIt1, BidirIt1, BidirIt2);
    template <typename BidirIt1, typename BidirIt2>
    BidirIt1 construct_move_backward_to(BidirIt1, BidirIt2, BidirIt2);
    template <typename BidirIt1, typename Size, typename BidirIt2>
    BidirIt2 construct_move_backward_n(BidirIt1, Size, BidirIt2);

    template <typename OutputIter>
    OutputIter destruct(OutputIter, OutputIter);
    template <typename OutputIter, typename Size>
    OutputIter destruct_n(OutputIter, Size);
}

template <typename T>
constexpr const T& nstd::min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}
template <typename T, typename Compare>
constexpr const T& nstd::min(const T& a, const T& b, Compare cmp)
{
    return cmp(b, a) ? b : a;
}
template <typename T>
constexpr const T& nstd::max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}
template <typename T, typename Compare>
constexpr const T& nstd::max(const T& a, const T& b, Compare cmp)
{
    return cmp(a, b) ? b : a;
}

template <typename InputIter1, typename InputIter2>
constexpr bool nstd::equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
    while (first1 != last1)
        if (!(*first1++ == *first2++)) return false;
    return true;
}
template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
constexpr bool nstd::equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, BinaryPredicate p)
{
    while (first1 != last1)
        if (!p(*first1++, *first2++)) return false;
    return true;
}
template <typename InputIter1, typename InputIter2>
constexpr bool nstd::equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
{
    while (first1 != last1 && first2 != last2)
        if (!(*first1++ == *first2++)) return false;
    return first1 == last1 && first2 == last2;
}
template <typename InputIter1, typename InputIter2, typename BinaryPredicate>
constexpr bool nstd::equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, BinaryPredicate p)
{
    while (first1 != last1 && first2 != last2)
        if (!p(*first1++, *first2++)) return false;
    return first1 == last1 && first2 == last2;
}
template <typename InputIter1, typename InputIter2>
constexpr bool nstd::lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
{
    while (first1 != last1 && first2 != last2)
    {
        if (*first1 < *first2) return true;
        if (*first2++ < *first1++) return false;
    }
    return first1 == last1 && first2 != last2;
}
template <typename InputIter1, typename InputIter2, typename Compare>
constexpr bool nstd::lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare cmp)
{
    while (first1 != last1 && first2 != last2)
    {
        if (cmp(*first1, *first2)) return true;
        if (cmp(*first2++, *first1++)) return false;
    }
    return first1 == last1 && first2 != last2;
}

template <typename OutputIter, typename T>
OutputIter nstd::fill(OutputIter d_first, OutputIter d_last, const T& val)
{
    while (d_first != d_last)
        *d_first++ = val;
    return d_first;
}
template <typename OutputIter, typename Size, typename T>
OutputIter nstd::fill_n(OutputIter d_first, Size cnt, const T& val)
{
    while (cnt-- > 0)
        *d_first++ = val;
    return d_first;
}
template <typename InputIter, typename OutputIter>
OutputIter nstd::copy(InputIter first, InputIter last, OutputIter d_first)
{
    while (first != last)
        *d_first++ = *first++;
    return d_first;
}
template <typename InputIter, typename OutputIter>
InputIter nstd::copy_to(InputIter first, OutputIter d_first, OutputIter d_last)
{
    while (d_first != d_last)
        *d_first++ = *first++;
    return first;
}
template <typename InputIter, typename Size, typename OutputIter>
OutputIter nstd::copy_n(InputIter first, Size cnt, OutputIter d_first)
{
    while (cnt-- > 0)
        *d_first++ = *first++;
    return d_first;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt2 nstd::copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    while (last != first)
        *--d_last = *--last;
    return d_last;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt1 nstd::copy_backward_to(BidirIt1 last, BidirIt2 d_first, BidirIt2 d_last)
{
    while (d_last != d_first)
        *--d_last = *--last;
    return last;
}
template <typename BidirIt1, typename Size, typename BidirIt2>
BidirIt2 nstd::copy_backward_n(BidirIt1 last, Size cnt, BidirIt2 d_last)
{
    while (cnt-- > 0)
        *--d_last = *--last;
    return d_last;
}
template <typename InputIter, typename OutputIter>
OutputIter nstd::move(InputIter first, InputIter last, OutputIter d_first)
{
    while (first != last)
        *d_first++ = std::move(*first++);
    return d_first;
}
template <typename InputIter, typename OutputIter>
InputIter nstd::move_to(InputIter first, OutputIter d_first, OutputIter d_last)
{
    while (d_first != d_last)
        *d_first++ = std::move(*first++);
    return first;
}
template <typename InputIter, typename Size, typename OutputIter>
OutputIter nstd::move_n(InputIter first, Size cnt, OutputIter d_first)
{
    while (cnt-- > 0)
        *d_first++ = std::move(*first++);
    return d_first;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt2 nstd::move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    while (last != first)
        *--d_last = std::move(*--last);
    return d_last;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt1 nstd::move_backward_to(BidirIt1 last, BidirIt2 d_first, BidirIt2 d_last)
{
    while (d_last != d_first)
        *--d_last = std::move(*--last);
    return last;
}
template <typename BidirIt1, typename Size, typename BidirIt2>
BidirIt2 nstd::move_backward_n(BidirIt1 last, Size cnt, BidirIt2 d_last)
{
    while (cnt-- > 0)
        *--d_last = std::move(*--last);
    return d_last;
}
template <typename OutputIter, typename... Args>
OutputIter nstd::construct(OutputIter d_first, OutputIter d_last, Args&&... args)
{
    while (d_first != d_last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(args...);
    return d_first;
}
template <typename OutputIter, typename Size, typename... Args>
OutputIter nstd::construct_n(OutputIter d_first, Size cnt, Args&&... args)
{
    while (cnt-- > 0)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(args...);
    return d_first;
}
template <typename OutputIter, typename T>
OutputIter nstd::construct_fill(OutputIter d_first, OutputIter d_last, const T& val)
{
    while (d_first != d_last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(val);
    return d_first;
}
template <typename OutputIter, typename Size, typename T>
OutputIter nstd::construct_fill_n(OutputIter d_first, Size cnt, const T& val)
{
    while (cnt-- > 0)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(val);
    return d_first;
}
template <typename InputIter, typename OutputIter>
OutputIter nstd::construct_copy(InputIter first, InputIter last, OutputIter d_first)
{
    while (first != last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(*first++);
    return d_first;
}
template <typename InputIter, typename OutputIter>
InputIter nstd::construct_copy_to(InputIter first, OutputIter d_first, OutputIter d_last)
{
    while (d_first != d_last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(*first++);
    return first;
}
template <typename InputIter, typename Size, typename OutputIter>
OutputIter nstd::construct_copy_n(InputIter first, Size cnt, OutputIter d_first)
{
    while (cnt-- > 0)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(*first++);
    return d_first;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt2 nstd::construct_copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    while (last != first)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(*--last);
    return d_last;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt1 nstd::construct_copy_backward_to(BidirIt1 last, BidirIt2 d_first, BidirIt2 d_last)
{
    while (d_last != d_first)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(*--last);
    return last;
}
template <typename BidirIt1, typename Size, typename BidirIt2>
BidirIt2 nstd::construct_copy_backward_n(BidirIt1 last, Size cnt, BidirIt2 d_last)
{
    while (cnt-- > 0)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(*--last);
    return d_last;
}
template <typename InputIter, typename OutputIter>
OutputIter nstd::construct_move(InputIter first, InputIter last, OutputIter d_first)
{
    while (first != last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(std::move(*first++));
    return d_first;
}
template <typename InputIter, typename OutputIter>
InputIter nstd::construct_move_to(InputIter first, OutputIter d_first, OutputIter d_last)
{
    while (d_first != d_last)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(std::move(*first++));
    return first;
}
template <typename InputIter, typename Size, typename OutputIter>
OutputIter nstd::construct_move_n(InputIter first, Size cnt, OutputIter d_first)
{
    while (cnt-- > 0)
        new(d_first++) typename std::iterator_traits<OutputIter>::value_type(std::move(*first++));
    return d_first;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt2 nstd::construct_move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
{
    while (last != first)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(std::move(*--last));
    return d_last;
}
template <typename BidirIt1, typename BidirIt2>
BidirIt1 nstd::construct_move_backward_to(BidirIt1 last, BidirIt2 d_first, BidirIt2 d_last)
{
    while (d_last != d_first)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(std::move(*--last));
    return last;
}
template <typename BidirIt1, typename Size, typename BidirIt2>
BidirIt2 nstd::construct_move_backward_n(BidirIt1 last, Size cnt, BidirIt2 d_last)
{
    while (cnt-- > 0)
        new(--d_last) typename std::iterator_traits<BidirIt2>::value_type(std::move(*--last));
    return d_last;
}
template <typename OutputIter>
OutputIter nstd::destruct(OutputIter d_first, OutputIter d_last)
{
    while (d_first != d_last)
        (d_first++)->std::iterator_traits<OutputIter>::value_type::~value_type();
    return d_first;
}
template <typename OutputIter, typename Size>
OutputIter nstd::destruct_n(OutputIter d_first, Size cnt)
{
    while (cnt-- > 0)
        (d_first++)->std::iterator_traits<OutputIter>::value_type::~value_type();
    return d_first;
}
