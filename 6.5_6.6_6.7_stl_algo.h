/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_ALGO_H
#define __SGI_STL_INTERNAL_ALGO_H

#include <stl_heap.h>

// See concept_checks.h for the concept-checking macros 
// __STL_REQUIRES, __STL_CONVERTIBLE, etc.


__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1209
#endif

// __median (an extension, not present in the C++ standard).

// SGI STL提供的三件中值决定函数
template <class _Tp>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c) 
{
  __STL_REQUIRES(_Tp, _LessThanComparable);
  if (__a < __b)
    // a < b < c
    if (__b < __c)
      return __b;
    // a < b, b >= c, a < c
    else if (__a < __c)
      return __c;
    else
      return __a;
  // b <= a < c
  else if (__a < __c)
    return __a;
  // b <= a, a >= c, b < c
  else if (__b < __c)
    return __c;
  else
    return __b;
}

template <class _Tp, class _Compare>
inline const _Tp&
__median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp) {
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  if (__comp(__a, __b))
    if (__comp(__b, __c))
      return __b;
    else if (__comp(__a, __c))
      return __c;
    else
      return __a;
  else if (__comp(__a, __c))
    return __a;
  else if (__comp(__b, __c))
    return __c;
  else
    return __b;
}

// for_each.  Apply a function to every element of a range.
// 将仿函数f施行于[first, last)区间的每一个元素身上。
// 因为first和last都是_InputIter，不保证接收赋值行为，f不可以改变元素内容。
// 如果想要一一修改元素内容，应该使用算法transform()。
// f可以返回一个值，但该值会被忽略。
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  for ( ; __first != __last; ++__first)
    __f(*__first);
  return __f;
}

// find and find_if.

// 根据equality操作符，循序查找[first, last)内所有的元素，找出第一个匹配“等同(equality)条件”者。
// 如果找到，就返回一个inputterator指向该元素，否则返回迭代器last。
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val, input_iterator_tag)
{
  while (__first != __last && !(*__first == __val))
    ++__first;
  return __first;
}

// 根据指定的pred运算条件（以仿函数表示），循序查找[first, last)内所有的元素，找出第一个令pred运算结果为true者。
// 如果找到，就返回一个inputterator指向该元素，否则返回迭代器last。
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred, input_iterator_tag)
{
  while (__first != __last && !__pred(*__first))
    ++__first;
  return __first;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter find(_RandomAccessIter __first, _RandomAccessIter __last,
                       const _Tp& __val,
                       random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;

    if (*__first == __val) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (*__first == __val) return __first;
    ++__first;
  case 2:
    if (*__first == __val) return __first;
    ++__first;
  case 1:
    if (*__first == __val) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter find_if(_RandomAccessIter __first, _RandomAccessIter __last,
                          _Predicate __pred,
                          random_access_iterator_tag)
{
  typename iterator_traits<_RandomAccessIter>::difference_type __trip_count
    = (__last - __first) >> 2;

  for ( ; __trip_count > 0 ; --__trip_count) {
    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;

    if (__pred(*__first)) return __first;
    ++__first;
  }

  switch(__last - __first) {
  case 3:
    if (__pred(*__first)) return __first;
    ++__first;
  case 2:
    if (__pred(*__first)) return __first;
    ++__first;
  case 1:
    if (__pred(*__first)) return __first;
    ++__first;
  case 0:
  default:
    return __last;
  }
}

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// 完全泛化版
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_InputIter>::value_type, _Tp);
  return find(__first, __last, __val, __ITERATOR_CATEGORY(__first));
}

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last,
                          _Predicate __pred) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
          typename iterator_traits<_InputIter>::value_type);
  return find_if(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}

// adjacent_find.
// 找出第一组满足条件的相邻元素。这里所谓的条件，在版本一中指“两元素相等”。
template <class _ForwardIter>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type, _EqualityComparable);
  if (__first == __last)
    return __last;
  _ForwardIter __next = __first;
  while(++__next != __last) 
  {
    // 找到相邻的元素值相同，就结束
    if (*__first == *__next)
      return __first;
    __first = __next;
  }
  return __last;
}

// 在版本二中允许用户指定一个二元运算。
template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter adjacent_find(_ForwardIter __first, _ForwardIter __last, _BinaryPredicate __binary_pred) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool, typename iterator_traits<_ForwardIter>::value_type, typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return __last;
  _ForwardIter __next = __first;
  while(++__next != __last) 
  {
    if (__binary_pred(*__first, *__next))
      return __first;
    __first = __next;
  }
  return __last;
}

// count and count_if.  There are two version of each, one whose return type
// type is void and one (present only if we have partial specialization)
// whose return type is iterator_traits<_InputIter>::difference_type.  The
// C++ standard only has the latter version, but the former, which was present
// in the HP STL, is retained for backward compatibility.

// 运用equality操作符，将[first, last)区间内的每一个元素拿来和指定值value比较，并返回与value相等的元素个数。
// 旧版，需提供计数器
template <class _InputIter, class _Tp, class _Size>
void count(_InputIter __first, _InputIter __last, const _Tp& __value, _Size& __n) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type, _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);
  for ( ; __first != __last; ++__first)
    if (*__first == __value)
      ++__n;
}

// 将指定操作（一个仿函数）pred实施与[first, last)区间内的每一个元素身上，并将“造成pred之计算结果为true”的所有元素的个数返回。
template <class _InputIter, class _Predicate, class _Size>
void count_if(_InputIter __first, _InputIter __last, _Predicate __pred, _Size& __n) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_InputIter>::value_type);
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      ++__n;
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
// 新版
template <class _InputIter, class _Tp>
typename iterator_traits<_InputIter>::difference_type
count(_InputIter __first, _InputIter __last, const _Tp& __value) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type, _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);
  // 声明一个计数器
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  for ( ; __first != __last; ++__first)
    if (*__first == __value)
      ++__n;
  return __n;
}

template <class _InputIter, class _Predicate>
typename iterator_traits<_InputIter>::difference_type
count_if(_InputIter __first, _InputIter __last, _Predicate __pred) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_InputIter>::value_type);
  typename iterator_traits<_InputIter>::difference_type __n = 0;
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      ++__n;
  return __n;
}


#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// search.

// 在序列一中查找序列二，如果完全比对成功，返回序列一中第一次找到序列二的首位置
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2) 
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_ForwardIter1>::value_type, typename iterator_traits<_ForwardIter2>::value_type);

  // Test for empty ranges
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

  // Test for a pattern of length 1.
  // 序列二仅含一个元素
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2)
    return find(__first1, __last1, *__first2);

  // General case.

  _ForwardIter2 __p1, __p;

  __p1 = __first2; 
  ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) 
  {
    // 从序列一中比对序列二的首元素
    __first1 = find(__first1, __last1, *__first2);
    if (__first1 == __last1)
      return __last1;

    __p = __p1;
    __current = __first1; 
    // 比对成功后选定第二个元素
    if (++__current == __last1)
      return __last1;
    // 从第二个元素开始对比，current是序列一的，p是序列二的
    while (*__current == *__p) 
    {
      // 完全找到序列二
      if (++__p == __last2)
        return __first1;
      // 序列一不够长
      if (++__current == __last1)
        return __last1;
    }

    ++__first1;
  }
  return __first1;
}

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                     _BinaryPred  __predicate) 
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPred, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);

  // Test for empty ranges
  if (__first1 == __last1 || __first2 == __last2)
    return __first1;

  // Test for a pattern of length 1.
  _ForwardIter2 __tmp(__first2);
  ++__tmp;
  if (__tmp == __last2) {
    while (__first1 != __last1 && !__predicate(*__first1, *__first2))
      ++__first1;
    return __first1;    
  }

  // General case.

  _ForwardIter2 __p1, __p;

  __p1 = __first2; ++__p1;

  _ForwardIter1 __current = __first1;

  while (__first1 != __last1) {
    while (__first1 != __last1) {
      if (__predicate(*__first1, *__first2))
        break;
      ++__first1;
    }
    while (__first1 != __last1 && !__predicate(*__first1, *__first2))
      ++__first1;
    if (__first1 == __last1)
      return __last1;

    __p = __p1;
    __current = __first1; 
    if (++__current == __last1) return __last1;

    while (__predicate(*__current, *__p)) {
      if (++__p == __last2)
        return __first1;
      if (++__current == __last1)
        return __last1;
    }

    ++__first1;
  }
  return __first1;
}

// search_n.  Search for __count consecutive copies of __val.

// 在序列[first, last)所涵盖的区间中，查找“包含连续count个符合条件的元素”形成的子序列，返回一个迭代器指向该子序列起始处。
template <class _ForwardIter, class _Integer, class _Tp>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last, _Integer __count, const _Tp& __val) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type, _EqualityComparable);
  __STL_REQUIRES(_Tp, _EqualityComparable);

  if (__count <= 0)
    return __first;
  else 
  {
    __first = find(__first, __last, __val);
    while (__first != __last) 
    {
      // 每次循环计数器会重置
      _Integer __n = __count - 1;
      _ForwardIter __i = __first;
      ++__i;
      while (__i != __last && __n != 0 && *__i == __val) 
      {
        ++__i;
        --__n;
      }
      if (__n == 0)
        return __first;
      else
        __first = find(__i, __last, __val);
    }
    return __last;
  }
}

// 查找“连续count个元素皆满足指定条件”所形成的那个子序列的起点，返回其发生位置。
template <class _ForwardIter, class _Integer, class _Tp, class _BinaryPred>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last, _Integer __count, const _Tp& __val, _BinaryPred __binary_pred) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPred, bool, typename iterator_traits<_ForwardIter>::value_type, _Tp);
  if (__count <= 0)
    return __first;
  else 
  {
    while (__first != __last) 
    {
      if (__binary_pred(*__first, __val))
        break;
      ++__first;
    }
    while (__first != __last) 
    {
      _Integer __n = __count - 1;
      _ForwardIter __i = __first;
      ++__i;
      while (__i != __last && __n != 0 && __binary_pred(*__i, __val)) 
      {
        ++__i;
        --__n;
      }
      if (__n == 0)
        return __first;
      else {
        while (__i != __last) 
        {
          if (__binary_pred(*__i, __val))
            break;
          ++__i;
        }
        __first = __i;
      }
    }
    return __last;
  }
} 

// swap_ranges

// 将[first1, last1)区间内的元素与“从first2开始、个数相同”的元素互相交换。
// 这两个序列可位于同一容器，也可位于不同的容器。
// 如果第二序列的长度小于第一序列，或是两序列在同一容器中且彼此重叠，执行结果未可预期。
// 次算法返回一个迭代器，指向第二序列中的最后一个被交换元素的下一个位置。
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2) 
{
  __STL_REQUIRES(_ForwardIter1, _Mutable_ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter1>::value_type, typename iterator_traits<_ForwardIter2>::value_type);
  __STL_CONVERTIBLE(typename iterator_traits<_ForwardIter2>::value_type, typename iterator_traits<_ForwardIter1>::value_type);
  for ( ; __first1 != __last1; ++__first1, ++__first2)
    iter_swap(__first1, __first2);
  return __first2;
}

// transform

// 版本一以仿函数opr作用域[first, last)中的每一个元素身上，并以其结果产生出一个新序列。
template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last, _OutputIter __result, _UnaryOperation __opr) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);

  for ( ; __first != __last; ++__first, ++__result)
    *__result = __opr(*__first);
  return __result;
}

// 版本二以仿函数__binary_op作用于一双元素身上（其中一个元素来自[first1, last)，另一个元素来自“从first2开始的序列”），并以其结果产生一个新序列。
template <class _InputIter1, class _InputIter2, class _OutputIter, class _BinaryOperation>
_OutputIter transform(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _OutputIter __result, _BinaryOperation __binary_op) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  for ( ; __first1 != __last1; ++__first1, ++__first2, ++__result)
    *__result = __binary_op(*__first1, *__first2);
  return __result;
}

// replace, replace_if, replace_copy, replace_copy_if

// 将[first, last)区间内的所有old_value都以new_value取代
template <class _ForwardIter, class _Tp>
void replace(_ForwardIter __first, _ForwardIter __last, const _Tp& __old_value, const _Tp& __new_value) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_ForwardIter>::value_type, _Tp);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)
    if (*__first == __old_value)
      *__first = __new_value;
}

template <class _ForwardIter, class _Predicate, class _Tp>
void replace_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred, const _Tp& __new_value) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)
    if (__pred(*__first))
      *__first = __new_value;
}

// 行为与replace()相似，唯一不同的是新序列会被复制到result所指的容器中。原序列没有任何改变。
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter replace_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __old_value, const _Tp& __new_value) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_InputIter>::value_type, _Tp);
  for ( ; __first != __last; ++__first, ++__result)
    *__result = *__first == __old_value ? __new_value : *__first;
  return __result;
}

// 指定pred替代equality操作符
template <class _InputIter, class _OutputIter, class _Predicate, class _Tp>
_OutputIter replace_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred, const _Tp& __new_value) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_InputIter>::value_type);
  for ( ; __first != __last; ++__first, ++__result)
    *__result = __pred(*__first) ? __new_value : *__first;
  return __result;
}

// generate and generate_n

// 将仿函数gen的运算结果填写在[first, last)区间的所有元素身上。
// 填写用的是迭代器所指元素的assignment操作符（=）
template <class _ForwardIter, class _Generator>
void generate(_ForwardIter __first, _ForwardIter __last, _Generator __gen) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_GENERATOR_CHECK(_Generator, typename iterator_traits<_ForwardIter>::value_type);
  for ( ; __first != __last; ++__first)
    *__first = __gen();
}

template <class _OutputIter, class _Size, class _Generator>
_OutputIter generate_n(_OutputIter __first, _Size __n, _Generator __gen) {
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  for ( ; __n > 0; --__n, ++__first)
    *__first = __gen();
  return __first;
}

// remove, remove_if, remove_copy, remove_copy_if

// 将结果复制到一个以result标志起始位置的容器身上。新容器可以和原容器重叠。
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __value) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_InputIter>::value_type, _Tp);
  for ( ; __first != __last; ++__first)
    if (!(*__first == __value)) 
    {
      *__result = *__first;
      ++__result;
    }
  return __result;
}

// 用一元操作符pred替代equality操作符
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_InputIter>::value_type);
  for ( ; __first != __last; ++__first)
    if (!__pred(*__first)) 
    {
      *__result = *__first;
      ++__result;
    }
  return __result;
}

/*
// 这一算法并不真正从容器中删除那些元素（换句话说容器大小并未改变），
// 而是将每一个不与value相等（也就是我们并不打算移除）的元素轮番赋值给first之后的空间。
// 返回值ForwardIterator标示出重新整理后的最后元素的下一个位置。
// 如果要删除那些残余数据，可将返回的迭代器交给区间所在的容器的erase() member function。
*/
template <class _ForwardIter, class _Tp>
_ForwardIter remove(_ForwardIter __first, _ForwardIter __last, const _Tp& __value) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_ForwardIter>::value_type, _Tp);
  __STL_CONVERTIBLE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  // 利用循环查找法找出第一个相等的元素。
  __first = find(__first, __last, __value);
  _ForwardIter __i = __first;
  // 利用“remove_copy()允许新旧容器重叠”的性质，进行移除操作。
  return __first == __last ? __first : remove_copy(++__i, __last, __first, __value);
}

template <class _ForwardIter, class _Predicate>
_ForwardIter remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_ForwardIter>::value_type);
  __first = find_if(__first, __last, __pred);
  _ForwardIter __i = __first;
  return __first == __last ? __first : remove_copy_if(++__i, __last, __first, __pred);
}

// unique and unique_copy

// 由于output iterator为write only，无法像forward iterator那般可以读取，所以不能有类似*result != *first这样的判断操作，所以需要设计此版本。
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last, _OutputIter __result, _Tp*) 
{
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (!(__value == *__first)) 
    {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}

// 版本一辅助函数，output_iterator_tag版
template <class _InputIter, class _OutputIter>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last, _OutputIter __result,  output_iterator_tag) 
{
  // 以下，output iterator有一些功能限制，所以必须先知道其value type。
  return __unique_copy(__first, __last, __result, __VALUE_TYPE(__first));
}

// 版本一辅助函数，forward_iterator_tag版
template <class _InputIter, class _ForwardIter>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, forward_iterator_tag) 
{
  *__result = *__first;
  while (++__first != __last)
    if (!(*__result == *__first))
      *++__result = *__first;
  return ++__result;
}

// 算法unique_copy可以从[first, last)中将元素复制到以result开头的区间上；
// 如果面对相邻重复元素群，指挥赋值其中第一个元素。
// 返回的迭代器指向以result开头的区间的尾端。
// 版本一使用简单的equality操作符测试。
template <class _InputIter, class _OutputIter>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last, _OutputIter __result) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type, _EqualityComparable);
  if (__first == __last) 
    return __result;
  return __unique_copy(__first, __last, __result, __ITERATOR_CATEGORY(__result));
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate,
          class _Tp>
_OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                          _OutputIter __result,
                          _BinaryPredicate __binary_pred, _Tp*) {
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool, _Tp, _Tp);
  _Tp __value = *__first;
  *__result = __value;
  while (++__first != __last)
    if (!__binary_pred(__value, *__first)) {
      __value = *__first;
      *++__result = __value;
    }
  return ++__result;
}

template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter __unique_copy(_InputIter __first, _InputIter __last,
                                 _OutputIter __result,
                                 _BinaryPredicate __binary_pred,
                                 output_iterator_tag) {
  return __unique_copy(__first, __last, __result, __binary_pred,
                       __VALUE_TYPE(__first));
}

template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_ForwardIter __unique_copy(_InputIter __first, _InputIter __last,
                           _ForwardIter __result, 
                           _BinaryPredicate __binary_pred,
                           forward_iterator_tag) {
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
     typename iterator_traits<_ForwardIter>::value_type,
     typename iterator_traits<_InputIter>::value_type);
  *__result = *__first;
  while (++__first != __last)
    if (!__binary_pred(*__result, *__first)) *++__result = *__first;
  return ++__result;
}

// 版本二使用一个Binary Predicate作为测试准则。
template <class _InputIter, class _OutputIter, class _BinaryPredicate>
inline _OutputIter unique_copy(_InputIter __first, _InputIter __last, _OutputIter __result, _BinaryPredicate __binary_pred) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  if (__first == __last) 
    return __result;
  return __unique_copy(__first, __last, __result, __binary_pred, __ITERATOR_CATEGORY(__result));
}

template <class _ForwardIter>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type, _EqualityComparable);
  __first = adjacent_find(__first, __last);
  return unique_copy(__first, __last, __first);
}

template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter unique(_ForwardIter __first, _ForwardIter __last,
                    _BinaryPredicate __binary_pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool, 
      typename iterator_traits<_ForwardIter>::value_type,
      typename iterator_traits<_ForwardIter>::value_type);
  __first = adjacent_find(__first, __last, __binary_pred);
  return unique_copy(__first, __last, __first, __binary_pred);
}

// reverse and reverse_copy, and their auxiliary functions

// bidirectional iterator版
template <class _BidirectionalIter>
void __reverse(_BidirectionalIter __first, _BidirectionalIter __last,  bidirectional_iterator_tag) 
{
  while (true)
    if (__first == __last || __first == --__last)
      return;
    else
      iter_swap(__first++, __last);
}

// random access iterator版
template <class _RandomAccessIter>
void __reverse(_RandomAccessIter __first, _RandomAccessIter __last, random_access_iterator_tag) 
{
  // 只有random access iterators才能做一下的first < last判断
  while (__first < __last)
    iter_swap(__first++, --__last);
}

// 将序列[first, last)的元素在原容器中颠倒重排。迭代器的双向或随机定位能力，影响了这个算法的效率，所以设计为双层架构
template <class _BidirectionalIter>
inline void reverse(_BidirectionalIter __first, _BidirectionalIter __last) 
{
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  __reverse(__first, __last, __ITERATOR_CATEGORY(__first));
}

// 行为类似reverse()，但产生出来的新序列会被置于以result指出的容器中。
// 返回值OutputIterator指向新产生的最后元素的下一个位置。原序列没有任何改变。
template <class _BidirectionalIter, class _OutputIter>
_OutputIter reverse_copy(_BidirectionalIter __first, _BidirectionalIter __last, _OutputIter __result) 
{
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  while (__first != __last) 
  {
    --__last;
    *__result = *__last;
    ++__result;
  }
  return __result;
}

// rotate and rotate_copy, and their auxiliary functions

// 最大公因数，辗转相除法，用除数除以余数直到余数为0
template <class _EuclideanRingElement>
_EuclideanRingElement __gcd(_EuclideanRingElement __m, _EuclideanRingElement __n)
{
  while (__n != 0) 
  {
    _EuclideanRingElement __t = __m % __n;
    __m = __n;
    __n = __t;
  }
  return __m;
}

// forward iterator版
template <class _ForwardIter, class _Distance>
_ForwardIter __rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last, _Distance*, forward_iterator_tag) 
{
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;

  _ForwardIter __first2 = __middle;
  // 此过程结束，从middle开始到last的部分以被从后段移到前段，且顺序保持不变
  // 但从first到middle - 1的部分虽然被移动到后段，但顺序可能不对
  // first指针用作遍历贯穿全程没有做修改，first2和middle做修改
  do {
    swap(*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
  } while (__first2 != __last);

  // 函数返回指针指向反转后原后半段尾端的位置
  _ForwardIter __new_middle = __first;

  __first2 = __middle;

  // 重排移动到后段的原前段顺序
  while (__first2 != __last) 
  {
    swap (*__first++, *__first2++);
    if (__first == __middle)
      __middle = __first2;
    else if (__first2 == __last)
      __first2 = __middle;
  }

  return __new_middle;
}

// bidirectional iterator版
template <class _BidirectionalIter, class _Distance>
_BidirectionalIter __rotate(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last, _Distance*, bidirectional_iterator_tag) 
{
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  if (__first == __middle)
    return __last;
  if (__last  == __middle)
    return __first;
  // 分别反转前后段内容
  __reverse(__first,  __middle, bidirectional_iterator_tag());
  __reverse(__middle, __last,   bidirectional_iterator_tag());

  // 头尾交换直到前段或后段耗尽
  while (__first != __middle && __middle != __last)
    swap (*__first++, *--__last);

  // 把剩余部分顺序反转回来
  if (__first == __middle) 
  {
    __reverse(__middle, __last,   bidirectional_iterator_tag());
    return __last;
  }
  else 
  {
    __reverse(__first,  __middle, bidirectional_iterator_tag());
    return __first;
  }
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __rotate(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Distance *, _Tp *) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  _Distance __n = __last   - __first;
  _Distance __k = __middle - __first;
  _Distance __l = __n - __k;
  _RandomAccessIter __result = __first + (__last - __middle);

  if (__k == 0)
    return __last;
  else if (__k == __l) 
  {
    swap_ranges(__first, __middle, __middle);
    return __result;
  }

  _Distance __d = __gcd(__n, __k);

  for (_Distance __i = 0; __i < __d; __i++) 
  {
    _Tp __tmp = *__first;
    _RandomAccessIter __p = __first;

    // 前段小于后段
    if (__k < __l) 
    {
      // 循环移位
      for (_Distance __j = 0; __j < __l / __d; __j++) 
      {
        // 将前段对应位前进后段的长度
        if (__p > __first + __l) 
        {
          *__p = *(__p - __l);
          __p -= __l;
        }
        // 将后段对应位后退前段的长度
        *__p = *(__p + __k);
        __p += __k;
      }
    }
    // 后段小于等于前段
    else 
    {
      for (_Distance __j = 0; __j < __k / __d - 1; __j ++) 
      {
        // 后段相应位后退前段的长度
        if (__p < __last - __k) 
        {
          *__p = *(__p + __k);
          __p += __k;
        }

        // 前段相应位前进后段长度
        *__p = * (__p - __l);
        __p -= __l;
      }
    }

    *__p = __tmp;
    ++__first;
  }

  return __result;
}

template <class _ForwardIter>
inline _ForwardIter rotate(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  return __rotate(__first, __middle, __last, __DISTANCE_TYPE(__first), __ITERATOR_CATEGORY(__first));
}

// 行为类似rotate()，但产生出来的新序列会被置于result所指出的容器中。返回值OutputIterator指向新产生的最后元素的下一位置。原序列没有任何改变。
// 由于不需要就地(in-place)在原容器中调整内容，所以只要把后段复制到新容器的前段，再把前段接续赋值到新容器即可。
template <class _ForwardIter, class _OutputIter>
_OutputIter rotate_copy(_ForwardIter __first, _ForwardIter __middle, _ForwardIter __last, _OutputIter __result) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  return copy(__first, __middle, copy(__middle, __last, __result));
}

// Return a random number in the range [0, __n).  This function encapsulates
// whether we're using rand (part of the standard C library) or lrand48
// (not standard, but a much better choice whenever it's available).

template <class _Distance>
inline _Distance __random_number(_Distance __n) 
{
#ifdef __STL_NO_DRAND48
  return rand() % __n;
#else
  return lrand48() % __n;
#endif
}

// random_shuffle

// 这个算法将[first, last)的元素次序随机重排。也就是说，在N!种可能的元素排列顺序中随机选出一种，此处N为last-first。
// 版本一使用内部随机数产生器。
template <class _RandomAccessIter>
inline void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) 
    return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __random_number((__i - __first) + 1));
}

// 版本二使用一个会产生随机随机数的仿函数。
// 特别注意的是这个仿函数的传递方式by reference而非一般的by value，这是因为随机随机数产生器有一个重要性质：
// 它拥有局部状态(local state)，每次被调用时都会有所改变，并因此保障产生出来的随机数能够随机。
template <class _RandomAccessIter, class _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last, _RandomNumberGenerator& __rand) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __rand((__i - __first) + 1));
}

// random_sample and random_sample_n (extensions, not part of the standard).

template <class _ForwardIter, class _OutputIter, class _Distance>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__random_number(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _ForwardIter, class _OutputIter, class _Distance,
          class _RandomNumberGenerator>
_OutputIter random_sample_n(_ForwardIter __first, _ForwardIter __last,
                            _OutputIter __out, const _Distance __n,
                            _RandomNumberGenerator& __rand)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, _Distance, _Distance);
  _Distance __remaining = 0;
  distance(__first, __last, __remaining);
  _Distance __m = min(__n, __remaining);

  while (__m > 0) {
    if (__rand(__remaining) < __m) {
      *__out = *__first;
      ++__out;
      --__m;
    }

    --__remaining;
    ++__first;
  }
  return __out;
}

template <class _InputIter, class _RandomAccessIter, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  const _Distance __n)
{
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first) 
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __random_number(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter,
          class _RandomNumberGenerator, class _Distance>
_RandomAccessIter __random_sample(_InputIter __first, _InputIter __last,
                                  _RandomAccessIter __out,
                                  _RandomNumberGenerator& __rand,
                                  const _Distance __n)
{
  __STL_UNARY_FUNCTION_CHECK(_RandomNumberGenerator, _Distance, _Distance);
  _Distance __m = 0;
  _Distance __t = __n;
  for ( ; __first != __last && __m < __n; ++__m, ++__first)
    __out[__m] = *__first;

  while (__first != __last) {
    ++__t;
    _Distance __M = __rand(__t);
    if (__M < __n)
      __out[__M] = *__first;
    ++__first;
  }

  return __out + __m;
}

template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  return __random_sample(__first, __last,
                         __out_first, __out_last - __out_first);
}


template <class _InputIter, class _RandomAccessIter, 
          class _RandomNumberGenerator>
inline _RandomAccessIter
random_sample(_InputIter __first, _InputIter __last,
              _RandomAccessIter __out_first, _RandomAccessIter __out_last,
              _RandomNumberGenerator& __rand) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  return __random_sample(__first, __last,
                         __out_first, __rand,
                         __out_last - __out_first);
}

// partition, stable_partition, and their auxiliary functions

template <class _ForwardIter, class _Predicate>
_ForwardIter __partition(_ForwardIter __first, _ForwardIter __last, _Predicate   __pred, forward_iterator_tag) 
{
  if (__first == __last) 
    return __first;

  // 碰到不符合pred条件的，跳出
  while (__pred(*__first))
    if (++__first == __last) 
      return __first;

  _ForwardIter __next = __first;
  
  while (++__next != __last)
    // 碰到符合pred条件的，进行交换
    if (__pred(*__next)) 
    {
      swap(*__first, *__next);
      ++__first;
    }

  return __first;
}

template <class _BidirectionalIter, class _Predicate>
_BidirectionalIter __partition(_BidirectionalIter __first, _BidirectionalIter __last, _Predicate __pred, bidirectional_iterator_tag) 
{
  while (true)
  {
    while (true)
      if (__first == __last)
        return __first;
      else if (__pred(*__first))
        ++__first;
      // 头，碰到不符合pred条件的，准备进行交换
      else
        break;

    --__last;
    while (true)
      if (__first == __last)
        return __first;
      else if (!__pred(*__last))
        --__last;
      // 尾，碰到符合条件的，准备进行交换。
      else
        break;
    iter_swap(__first, __last);
    // 头指针前进1，准备下一个外循环迭代。
    ++__first;
  }
}

// 快排的切分函数
// partition会将区间[first, last)中的元素重新排列。
// 所有被一元条件运算pred判定为true的元素，都会被放在区间的前端，被判定为false的元素，都会被放在区间的后端。
// 这个算法并不保证保留元素的原始相对位置。如果需要保留原始相对位置，应使用stable_partition。
template <class _ForwardIter, class _Predicate>
inline _ForwardIter partition(_ForwardIter __first, _ForwardIter __last, _Predicate   __pred) 
{
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool, typename iterator_traits<_ForwardIter>::value_type);
  return __partition(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}


template <class _ForwardIter, class _Predicate, class _Distance>
_ForwardIter __inplace_stable_partition(_ForwardIter __first,
                                        _ForwardIter __last,
                                        _Predicate __pred, _Distance __len) {
  if (__len == 1)
    return __pred(*__first) ? __last : __first;
  _ForwardIter __middle = __first;
  advance(__middle, __len / 2);
  return rotate(__inplace_stable_partition(__first, __middle, __pred, 
                                           __len / 2),
                __middle,
                __inplace_stable_partition(__middle, __last, __pred,
                                           __len - __len / 2));
}

template <class _ForwardIter, class _Pointer, class _Predicate, 
          class _Distance>
_ForwardIter __stable_partition_adaptive(_ForwardIter __first,
                                         _ForwardIter __last,
                                         _Predicate __pred, _Distance __len,
                                         _Pointer __buffer,
                                         _Distance __buffer_size) 
{
  if (__len <= __buffer_size) {
    _ForwardIter __result1 = __first;
    _Pointer __result2 = __buffer;
    for ( ; __first != __last ; ++__first)
      if (__pred(*__first)) {
        *__result1 = *__first;
        ++__result1;
      }
      else {
        *__result2 = *__first;
        ++__result2;
      }
    copy(__buffer, __result2, __result1);
    return __result1;
  }
  else {
    _ForwardIter __middle = __first;
    advance(__middle, __len / 2);
    return rotate(__stable_partition_adaptive(
                          __first, __middle, __pred,
                          __len / 2, __buffer, __buffer_size),
                    __middle,
                    __stable_partition_adaptive(
                          __middle, __last, __pred,
                          __len - __len / 2, __buffer, __buffer_size));
  }
}

template <class _ForwardIter, class _Predicate, class _Tp, class _Distance>
inline _ForwardIter
__stable_partition_aux(_ForwardIter __first, _ForwardIter __last, 
                       _Predicate __pred, _Tp*, _Distance*)
{
  _Temporary_buffer<_ForwardIter, _Tp> __buf(__first, __last);
  if (__buf.size() > 0)
    return __stable_partition_adaptive(__first, __last, __pred,
                                       _Distance(__buf.requested_size()),
                                       __buf.begin(), __buf.size());
  else
    return __inplace_stable_partition(__first, __last, __pred, 
                                      _Distance(__buf.requested_size()));
}

template <class _ForwardIter, class _Predicate>
inline _ForwardIter stable_partition(_ForwardIter __first,
                                     _ForwardIter __last, 
                                     _Predicate __pred) {
  __STL_REQUIRES(_ForwardIter, _Mutable_ForwardIterator);
  __STL_UNARY_FUNCTION_CHECK(_Predicate, bool,
      typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return __first;
  else
    return __stable_partition_aux(__first, __last, __pred,
                                  __VALUE_TYPE(__first),
                                  __DISTANCE_TYPE(__first));
}

// 忽略边界条件的检验的分割方法。
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot) 
{
  while (true) 
  {
    // 令头端迭代器first想尾部移动，当*first大于或等于枢轴时就停下来
    while (*__first < __pivot)
      ++__first;
    --__last;
    // 尾端迭代器last向头部移动，当*last小于或等于枢轴时也停下来
    while (__pivot < *__last)
      --__last;
    // 检查两个迭代器是否交错。
    if (!(__first < __last))
      // 如果发现两个迭代器交错了，表示整个序列已经调整完毕，以此时的first为轴，将序列分成左右两半，
      // 做半部分的元素都小于或等于枢轴，右半部分所有的元素值都大于或等于枢轴。
      return __first;
    // 如果first仍在last左，就将两者交换，然后各自调整一个位置（向中央逼近），再继续相同的行为。
    iter_swap(__first, __last);
    ++__first;
  }
}    

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, 
                                        _RandomAccessIter __last, 
                                        _Tp __pivot, _Compare __comp) 
{
  while (true) {
    while (__comp(*__first, __pivot))
      ++__first;
    --__last;
    while (__comp(__pivot, *__last))
      --__last;
    if (!(__first < __last))
      return __first;
    iter_swap(__first, __last);
    ++__first;
  }
}

const int __stl_threshold = 16;

// sort() and its auxiliary functions. 

// 版本一辅助函数
// 上述函数之所以命名为unguarded_x是因为，一般的Insertion Sort在内循环原本需要做两次判断，
// 判断是否相邻的两元素是“逆转对”，同时也判断循环的行进是否超过边界。
// 但由于上述所示的源代码会导致最小值必然在内循环子区间的最边缘(if (__val < *__first))，所以两个判断可合并为一个判断，所以称为unguarded。
template <class _RandomAccessIter, class _Tp>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val) 
{
  _RandomAccessIter __next = __last;
  --__next;
  // insertion sort的内循环
  // 内循环遍历子区间，将子区间内的每一个“逆转对(inversion)”倒转过来。
  while (__val < *__next) 
  {
    *__last = *__next;
    __last = __next;
    --__next;
  }
  // value的正确落脚处
  *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, 
                               _Compare __comp) {
  _RandomAccessIter __next = __last;
  --__next;  
  while (__comp(__val, *__next)) {
    *__last = *__next;
    __last = __next;
    --__next;
  }
  *__last = __val;
}

// 版本一辅助函数
template <class _RandomAccessIter, class _Tp>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*) 
{
  // 记录尾元素
  _Tp __val = *__last;
  // 尾比头小（注意，头端必为最小元素）
  if (__val < *__first) 
  {
    // 那么就别一个个比较了，将整个区间向右移一个位置
    copy_backward(__first, __last, __last + 1);
    // 令头元素等于原先的尾元素值
    *__first = __val;
  }
  else
    // 尾元素不小于头
    __unguarded_linear_insert(__last, __val);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Tp*, _Compare __comp) {
  _Tp __val = *__last;
  if (__comp(__val, *__first)) {
    copy_backward(__first, __last, __last + 1);
    *__first = __val;
  }
  else
    __unguarded_linear_insert(__last, __val, __comp);
}

/*
// 插入排序，复杂度为O(N^2)，说起来并不理想，但是当数据量很少时，却有不错的效果，原因是实现上有一些技巧，
// 并且不像其它较为复杂的排序算法有着诸如递归调用等操作带来的额外负荷。
*/
// 版本一使用以渐增爱抚那个是排序。
template <class _RandomAccessIter>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last) 
{
  if (__first == __last) 
    return; 
  // Insertion Sort以双层循环的形式进行。
  // 外循环遍历整个序列，每次迭代决定出一个子区间；
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    __linear_insert(__first, __i, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) 
{
  if (__first == __last) 
    return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    __linear_insert(__first, __i, __VALUE_TYPE(__first), __comp);
}

// 版本一
template <class _RandomAccessIter, class _Tp>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*) 
{
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i));
}

// 版本一
template <class _RandomAccessIter>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last) 
{
  __unguarded_insertion_sort_aux(__first, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, 
                                    _RandomAccessIter __last,
                                    _Tp*, _Compare __comp) {
  for (_RandomAccessIter __i = __first; __i != __last; ++__i)
    __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, 
                                       _RandomAccessIter __last,
                                       _Compare __comp) {
  __unguarded_insertion_sort_aux(__first, __last, __VALUE_TYPE(__first),
                                 __comp);
}

template <class _RandomAccessIter>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last) 
{
  // 首先判断元素个数是否大于16
  if (__last - __first > __stl_threshold) 
  {
    // 如果答案为是，就将[first, last)分割为长度16的一段子序列，和另一段剩余子序列，再针对两个子序列分别调用__insertion_sort()和__unguarded_insertion_sort().
    __insertion_sort(__first, __first + __stl_threshold);
    __unguarded_insertion_sort(__first + __stl_threshold, __last);
  }
  else
    // 如果答案为否，就调用__insertion_sort()加以处理。
    __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first > __stl_threshold) {
    __insertion_sort(__first, __first + __stl_threshold, __comp);
    __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
  }
  else
    __insertion_sort(__first, __last, __comp);
}

// 找出2^k <= n的最大值k。
template <class _Size>
inline _Size __lg(_Size __n) 
{
  _Size __k;
  for (__k = 0; __n != 1; __n >>= 1) 
    ++__k;
  return __k;
}

// 版本一内省排序
// 版函数内的许多迭代器运算操作，都只适用于RandomAccessIterators
template <class _RandomAccessIter, class _Tp, class _Size>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit)
{
  // __stl_threshold是个全局函数，稍早定义为16
  while (__last - __first > __stl_threshold) 
  {
    // 至此，分割恶化（分割层次超过指定值）
    if (__depth_limit == 0) 
    {
      // 改用heap sort
      partial_sort(__first, __last, __last);
      return;
    }
    --__depth_limit;
    // 以下是median-of-3 partition，选择一个后好的枢轴并决定分割点。分割点将落在迭代器cut身上。
    /*
    // 任何一个元素都可以被选来当作枢轴(pivot)，但是其合适与否却会影响Quick Sort的效率。
    // 为避免“元素当初输入时不够随机”所带来的恶化效应，最理想最稳当的方式是取整个序列的头、尾、中央三个位置的元素，以其中值(median)作为枢轴。
    */
    _RandomAccessIter __cut = __unguarded_partition(__first, __last, _Tp(__median(*__first, *(__first + (__last - __first)/2), *(__last - 1))));
    // 对右半段递归进行sort。
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit);
    // 现在回到while循环，准备对左半段递归进行sort
    __last = __cut;
  }
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first,
                      _RandomAccessIter __last, _Tp*,
                      _Size __depth_limit, _Compare __comp)
{
  while (__last - __first > __stl_threshold) {
    if (__depth_limit == 0) {
      partial_sort(__first, __last, __last, __comp);
      return;
    }
    --__depth_limit;
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2),
                                         *(__last - 1), __comp)),
       __comp);
    __introsort_loop(__cut, __last, (_Tp*) 0, __depth_limit, __comp);
    __last = __cut;
  }
}

// 版本一使用operator<排列。
// 千万注意：sort()只适用于RandomAccessIterator
template <class _RandomAccessIter>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type, _LessThanComparable);
  if (__first != __last) 
  {
    // 其中log()用来控制分割恶化的情况。eg. 当元素个数为40时，__introsort_loop()的最后的一个参数将是5*2，意思是最多允许分割10层。
    __introsort_loop(__first, __last, __VALUE_TYPE(__first), __lg(__last - __first) * 2);
    // 当__introsort_loop结束，[first, last)内有多个“元素个数少于16”的子序列，每个子序列都有相当程度的排序，
    // 但尚未完全排序（因为元素个数一旦小于__stl_threshold，就被终止进一步的排序操作了）。回到母函数sort()，再进行__final_insertion_sort();
    __final_insertion_sort(__first, __last);
  }
}

// 版本二允许用户指定一个仿函数comp作为排序标准。
template <class _RandomAccessIter, class _Compare>
inline void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_RandomAccessIter>::value_type, typename iterator_traits<_RandomAccessIter>::value_type);
  if (__first != __last) 
  {
    __introsort_loop(__first, __last, __VALUE_TYPE(__first), __lg(__last - __first) * 2, __comp);
    __final_insertion_sort(__first, __last, __comp);
  }
}

// stable_sort() and its auxiliary functions.

template <class _RandomAccessIter>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle);
  __inplace_stable_sort(__middle, __last);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle);
}

template <class _RandomAccessIter, class _Compare>
void __inplace_stable_sort(_RandomAccessIter __first,
                           _RandomAccessIter __last, _Compare __comp) {
  if (__last - __first < 15) {
    __insertion_sort(__first, __last, __comp);
    return;
  }
  _RandomAccessIter __middle = __first + (__last - __first) / 2;
  __inplace_stable_sort(__first, __middle, __comp);
  __inplace_stable_sort(__middle, __last, __comp);
  __merge_without_buffer(__first, __middle, __last,
                         __middle - __first,
                         __last - __middle,
                         __comp);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result);
    __first += __two_step;
  }

  __step_size = min(_Distance(__last - __first), __step_size);
  merge(__first, __first + __step_size, __first + __step_size, __last,
        __result);
}

template <class _RandomAccessIter1, class _RandomAccessIter2,
          class _Distance, class _Compare>
void __merge_sort_loop(_RandomAccessIter1 __first,
                       _RandomAccessIter1 __last, 
                       _RandomAccessIter2 __result, _Distance __step_size,
                       _Compare __comp) {
  _Distance __two_step = 2 * __step_size;

  while (__last - __first >= __two_step) {
    __result = merge(__first, __first + __step_size,
                     __first + __step_size, __first + __two_step,
                     __result,
                     __comp);
    __first += __two_step;
  }
  __step_size = min(_Distance(__last - __first), __step_size);

  merge(__first, __first + __step_size,
        __first + __step_size, __last,
        __result,
        __comp);
}

const int __stl_chunk_size = 7;
        
template <class _RandomAccessIter, class _Distance>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Distance __chunk_size)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last);
}

template <class _RandomAccessIter, class _Distance, class _Compare>
void __chunk_insertion_sort(_RandomAccessIter __first, 
                            _RandomAccessIter __last,
                            _Distance __chunk_size, _Compare __comp)
{
  while (__last - __first >= __chunk_size) {
    __insertion_sort(__first, __first + __chunk_size, __comp);
    __first += __chunk_size;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last,
                              _Pointer __buffer, _Distance*) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance,
          class _Compare>
void __merge_sort_with_buffer(_RandomAccessIter __first, 
                              _RandomAccessIter __last, _Pointer __buffer,
                              _Distance*, _Compare __comp) {
  _Distance __len = __last - __first;
  _Pointer __buffer_last = __buffer + __len;

  _Distance __step_size = __stl_chunk_size;
  __chunk_insertion_sort(__first, __last, __step_size, __comp);

  while (__step_size < __len) {
    __merge_sort_loop(__first, __last, __buffer, __step_size, __comp);
    __step_size *= 2;
    __merge_sort_loop(__buffer, __buffer_last, __first, __step_size, __comp);
    __step_size *= 2;
  }
}

template <class _RandomAccessIter, class _Pointer, class _Distance>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size);
}

template <class _RandomAccessIter, class _Pointer, class _Distance, 
          class _Compare>
void __stable_sort_adaptive(_RandomAccessIter __first, 
                            _RandomAccessIter __last, _Pointer __buffer,
                            _Distance __buffer_size, _Compare __comp) {
  _Distance __len = (__last - __first + 1) / 2;
  _RandomAccessIter __middle = __first + __len;
  if (__len > __buffer_size) {
    __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, 
                           __comp);
    __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, 
                           __comp);
  }
  else {
    __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0,
                               __comp);
    __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0,
                               __comp);
  }
  __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), 
                   _Distance(__last - __middle), __buffer, __buffer_size,
                   __comp);
}

template <class _RandomAccessIter, class _Tp, class _Distance>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()));
}

template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
inline void __stable_sort_aux(_RandomAccessIter __first,
                              _RandomAccessIter __last, _Tp*, _Distance*,
                              _Compare __comp) {
  _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
  if (buf.begin() == 0)
    __inplace_stable_sort(__first, __last, __comp);
  else 
    __stable_sort_adaptive(__first, __last, buf.begin(),
                           _Distance(buf.size()),
                           __comp);
}

template <class _RandomAccessIter>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  __stable_sort_aux(__first, __last,
                    __VALUE_TYPE(__first),
                    __DISTANCE_TYPE(__first));
}

template <class _RandomAccessIter, class _Compare>
inline void stable_sort(_RandomAccessIter __first,
                        _RandomAccessIter __last, _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_RandomAccessIter>::value_type,
       typename iterator_traits<_RandomAccessIter>::value_type);
  __stable_sort_aux(__first, __last,
                    __VALUE_TYPE(__first),
                    __DISTANCE_TYPE(__first), 
                    __comp);
}

// partial_sort, partial_sort_copy, and auxiliary functions.

template <class _RandomAccessIter, class _Tp>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*) 
{
  // partial_sort的任务是找出middle-first个最小元素，因此，首先界定出区间[first, middle)，并利用make_heap()将它组织成一个max-heap。
  make_heap(__first, __middle);
  // 
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (*__i < *__first) 
      // 调用的是内部_pop_heap()，与通常意义的pop_heap()不同，不在同一区间进行操作
      // 这里是把[first, middle)中最大的值与位置i进行交换，i位于[middle, last)，并重新保持max-heap的状态。
      // 如此一来，当我们走遍整个[middle, last)时，较大的元素都已经被抽离出[first, middle)
      __pop_heap(__first, __middle, __i, _Tp(*__i), __DISTANCE_TYPE(__first));
  // 这时再以sort_heap()将[first, middle)做一次排序，即功德圆满。
  sort_heap(__first, __middle);
}

// 接受一个middle迭代器（位于序列[first, last)之内），然后重新安排[first, last)，使序列中的middle-first个最小元素以递增顺序排序，置于[first, middle)内。
// 其余last-middle个元素安置于[middle, last)中，不保证任何特定顺序。
// 版本一使用less-than操作符。
template <class _RandomAccessIter>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type, _LessThanComparable);
  __partial_sort(__first, __middle, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                    _RandomAccessIter __last, _Tp*, _Compare __comp) {
  make_heap(__first, __middle, __comp);
  for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
    if (__comp(*__i, *__first))
      __pop_heap(__first, __middle, __i, _Tp(*__i), __comp,
                 __DISTANCE_TYPE(__first));
  sort_heap(__first, __middle, __comp);
}

// 版本二使用仿函数cmp。
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_RandomAccessIter>::value_type, typename iterator_traits<_RandomAccessIter>::value_type);
  __partial_sort(__first, __middle, __last, __VALUE_TYPE(__first), __comp);
}

template <class _InputIter, class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first, _InputIter __last, _RandomAccessIter __result_first, _RandomAccessIter __result_last,  _Distance*, _Tp*) 
{
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) 
  {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last);
  while (__first != __last) 
  {
    if (*__first < *__result_first) 
      __adjust_heap(__result_first, _Distance(0), _Distance(__result_real_last - __result_first), _Tp(*__first));
    ++__first;
  }
  sort_heap(__result_first, __result_real_last);
  return __result_real_last;
}

// 与partial_sort行为逻辑完全相同，只不过后者将(last-first)个最小元素（或最大元素，视comp而定）排序后的所得结果置于[result_first, result_last)。
template <class _InputIter, class _RandomAccessIter>
inline _RandomAccessIter partial_sort_copy(_InputIter __first, _InputIter __last, _RandomAccessIter __result_first, _RandomAccessIter __result_last) 
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_InputIter>::value_type, typename iterator_traits<_RandomAccessIter>::value_type);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type, _LessThanComparable);
  __STL_REQUIRES(typename iterator_traits<_InputIter>::value_type, _LessThanComparable);
  return __partial_sort_copy(__first, __last, __result_first, __result_last,  __DISTANCE_TYPE(__result_first), __VALUE_TYPE(__first));
}

template <class _InputIter, class _RandomAccessIter, class _Compare,
          class _Distance, class _Tp>
_RandomAccessIter __partial_sort_copy(_InputIter __first,
                                         _InputIter __last,
                                         _RandomAccessIter __result_first,
                                         _RandomAccessIter __result_last,
                                         _Compare __comp, _Distance*, _Tp*) {
  if (__result_first == __result_last) return __result_last;
  _RandomAccessIter __result_real_last = __result_first;
  while(__first != __last && __result_real_last != __result_last) {
    *__result_real_last = *__first;
    ++__result_real_last;
    ++__first;
  }
  make_heap(__result_first, __result_real_last, __comp);
  while (__first != __last) {
    if (__comp(*__first, *__result_first))
      __adjust_heap(__result_first, _Distance(0),
                    _Distance(__result_real_last - __result_first),
                    _Tp(*__first),
                    __comp);
    ++__first;
  }
  sort_heap(__result_first, __result_real_last, __comp);
  return __result_real_last;
}

template <class _InputIter, class _RandomAccessIter, class _Compare>
inline _RandomAccessIter
partial_sort_copy(_InputIter __first, _InputIter __last,
                  _RandomAccessIter __result_first,
                  _RandomAccessIter __result_last, _Compare __comp) {
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_CONVERTIBLE(typename iterator_traits<_InputIter>::value_type,
                    typename iterator_traits<_RandomAccessIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
     typename iterator_traits<_RandomAccessIter>::value_type,
     typename iterator_traits<_RandomAccessIter>::value_type);  
  return __partial_sort_copy(__first, __last, __result_first, __result_last,
                             __comp,
                             __DISTANCE_TYPE(__result_first),
                             __VALUE_TYPE(__first));
}

// nth_element() and its auxiliary functions.  

// 版本一辅助函数
// 不断地以median-of-3 partition（以首、尾、中央三点中值为枢轴之分割法）将整个序列分割为更小的左(L)、右(R)子序列。
// 如果nth迭代器落在左子序列，就再对左子序列进行分割，否则就再对右子序列进行分割。
// 以此类推，知道分割后的子序列长度不大于3，便对最后这个带分割的子序列做Insertion Sort
template <class _RandomAccessIter, class _Tp>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth, _RandomAccessIter __last, _Tp*) 
{
  while (__last - __first > 3) 
  {
    _RandomAccessIter __cut = __unguarded_partition(__first, __last, _Tp(__median(*__first, *(__first + (__last - __first)/2), *(__last - 1))));
    if (__cut <= __nth)
      __first = __cut;
    else 
      __last = __cut;
  }
  __insertion_sort(__first, __last);
}

// 这个算法会重新排列[first, last)，使迭代器nth所指的元素，与“整个[first, last)完整排序后，同一位置的元素”同值。
// 此外并保证[nth, last)内没有任何一个元素不大于[first, nth)内的元素，但是对于[nth, last)和[first, nth)两个子区间内的元素次序则无任何保证。
// 比较近似partition
// 版本一
template <class _RandomAccessIter>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth, _RandomAccessIter __last) 
{
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type, _LessThanComparable);
  __nth_element(__first, __nth, __last, __VALUE_TYPE(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                   _RandomAccessIter __last, _Tp*, _Compare __comp) {
  while (__last - __first > 3) {
    _RandomAccessIter __cut =
      __unguarded_partition(__first, __last,
                            _Tp(__median(*__first,
                                         *(__first + (__last - __first)/2), 
                                         *(__last - 1),
                                         __comp)),
                            __comp);
    if (__cut <= __nth)
      __first = __cut;
    else 
      __last = __cut;
  }
  __insertion_sort(__first, __last, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void nth_element(_RandomAccessIter __first, _RandomAccessIter __nth,
                        _RandomAccessIter __last, _Compare __comp) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
     typename iterator_traits<_RandomAccessIter>::value_type,
     typename iterator_traits<_RandomAccessIter>::value_type);
  __nth_element(__first, __nth, __last, __VALUE_TYPE(__first), __comp);
}


// Binary search (lower_bound, upper_bound, equal_range, binary_search).
// 版本一辅助函数
template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Distance*) 
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  // 二分查找
  while (__len > 0) 
  {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (*__middle < __val) 
    {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else
      __len = __half;
  }
  return __first;
  
}

/*
// 试图在已排序的[first, last)中寻找元素value。如果[first, last)具有与value相等的元素(s)，便返回一个迭代器，指向其中第一个元素。
// 如果没有这样的元素存在，便返回“假设这样的元素存在时应该在序列中的位置”。
// 也就是说，它会返回一个迭代器，指向第一个“不小于value”的元素。
// 如果value大于[first, last)内的任何一个元素，则返回last。
*/
// 版本一采用operator<进行比较。
template <class _ForwardIter, class _Tp>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __lower_bound(__first, __last, __val, __DISTANCE_TYPE(__first));
}

// 版本二辅助函数
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) 
  {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(*__middle, __val)) 
    {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else
      __len = __half;
  }
  return __first;
}

// 版本二采用仿函数comp。
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __lower_bound(__first, __last, __val, __comp, __DISTANCE_TYPE(__first));
}

// 版本一辅助函数
template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) 
  {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    // 与lower_bound中分位置不同
    if (__val < *__middle)
      __len = __half;
    else 
    {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
  }
  return __first;
}

/*
// 试图在已排序的[first, last)中寻找元素value。如果[first, last)具有与value相等的元素(s)，便返回一个迭代器，指向其中第一个value的下一个位置。
// 如果没有这样的元素存在，便返回“假设这样的元素存在时应该在序列中的位置”。
// 也就是说，它会返回一个迭代器，指向第一个“大于value”的元素。
// 如果value大于[first, last)内的任何一个元素，则返回last。
*/
// 版本一采用operator<进行比较。
template <class _ForwardIter, class _Tp>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __upper_bound(__first, __last, __val, __DISTANCE_TYPE(__first));
}

// 版本二辅助函数
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __upper_bound(_ForwardIter __first, _ForwardIter __last,
                           const _Tp& __val, _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(__val, *__middle))
      __len = __half;
    else {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
  }
  return __first;
}

// 版本二采用仿函数comp。
template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter upper_bound(_ForwardIter __first, _ForwardIter __last,
                                const _Tp& __val, _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp,
      typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __upper_bound(__first, __last, __val, __comp,
                       __DISTANCE_TYPE(__first));
}

// 版本一的_ForwardIter版本
template <class _ForwardIter, class _Tp, class _Distance>
pair<_ForwardIter, _ForwardIter> __equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) 
  {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (*__middle < __val) 
    {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else if (__val < *__middle)
      __len = __half;
    else 
    // 如果中央元素等于指定值
    {
      // 在前半段找lower_bound
      __left = lower_bound(__first, __middle, __val);
      advance(__first, __len);
      // 在后半段找upper_bound
      __right = upper_bound(++__middle, __first, __val);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}

/*
// equal_range是二分查找法的一个版本，试图在已排序的[first, last)中寻找value。
// 它返回一对迭代器i和j，其中i是在不破坏次序的前提下，value可插入的第一个位置（奕即lower_bound），
// j则是在不破坏次序的前提下，value可插入的最后一个位置（奕即upper_bound）。
*/ 
// 版本一采用operator<进行比较
template <class _ForwardIter, class _Tp>
inline pair<_ForwardIter, _ForwardIter> equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  return __equal_range(__first, __last, __val, __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
pair<_ForwardIter, _ForwardIter>
__equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
              _Compare __comp, _Distance*)
{
  _Distance __len = 0;
  distance(__first, __last, __len);
  _Distance __half;
  _ForwardIter __middle, __left, __right;

  while (__len > 0) {
    __half = __len >> 1;
    __middle = __first;
    advance(__middle, __half);
    if (__comp(*__middle, __val)) {
      __first = __middle;
      ++__first;
      __len = __len - __half - 1;
    }
    else if (__comp(__val, *__middle))
      __len = __half;
    else {
      __left = lower_bound(__first, __middle, __val, __comp);
      advance(__first, __len);
      __right = upper_bound(++__middle, __first, __val, __comp);
      return pair<_ForwardIter, _ForwardIter>(__left, __right);
    }
  }
  return pair<_ForwardIter, _ForwardIter>(__first, __first);
}           

template <class _ForwardIter, class _Tp, class _Compare>
inline pair<_ForwardIter, _ForwardIter>
equal_range(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
            _Compare __comp) {
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, 
       typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  return __equal_range(__first, __last, __val, __comp,
                       __DISTANCE_TYPE(__first));
} 

// 二分查找，试图爱已排序的[first, last)中寻找元素value。如果[first, last)内有等同于value的元素，便返回true，否则返回false。
// 事实上binary_search利用lower_bound先找出“假设value存在的话，应该出现的位置”，然后对比该位置上的值是否为我们要查找的目标，并返回对比结果。
// 版本一使用equality操作符
template <class _ForwardIter, class _Tp>
bool binary_search(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_REQUIRES(_Tp, _LessThanComparable);
  _ForwardIter __i = lower_bound(__first, __last, __val);
  return __i != __last && !(__val < *__i);
}
// 版本二使用comp

template <class _ForwardIter, class _Tp, class _Compare>
bool binary_search(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_SAME_TYPE(_Tp, typename iterator_traits<_ForwardIter>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
  _ForwardIter __i = lower_bound(__first, __last, __val, __comp);
  return __i != __last && !__comp(__val, *__i);
}

// merge, with and without an explicitly supplied comparison function.
// 将两个经过排序的集合S1和S2，合并起来置于另一段空间。所得结果也是一个有序(sorted)序列。
// 返回一个迭代器，指向最后结果序列的最后一个元素的下一个位置。
// 与set_union不同merge不会过滤掉多余的相同元素
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2) 
  {
    if (*__first2 < *__first1) 
    {
      *__result = *__first2;
      ++__first2;
    }
    else 
    {
      *__result = *__first1;
      ++__first1;
    }
    ++__result;
  }
  // 最后剩下元素以copy复制到目的端。以下两个序列一定至少有一个为空。
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result, _Compare __comp) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter1>::value_type);
  while (__first1 != __last1 && __first2 != __last2) 
  {
    if (__comp(*__first2, *__first1)) 
    {
      *__result = *__first2;
      ++__first2;
    }
    else 
    {
      *__result = *__first1;
      ++__first1;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

// inplace_merge and its auxiliary functions. 

template <class _BidirectionalIter, class _Distance>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (*__middle < *__first)
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle,
                         __len11, __len22);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22);
}

template <class _BidirectionalIter, class _Distance, class _Compare>
void __merge_without_buffer(_BidirectionalIter __first,
                            _BidirectionalIter __middle,
                            _BidirectionalIter __last,
                            _Distance __len1, _Distance __len2,
                            _Compare __comp) {
  if (__len1 == 0 || __len2 == 0)
    return;
  if (__len1 + __len2 == 2) {
    if (__comp(*__middle, *__first))
      iter_swap(__first, __middle);
    return;
  }
  _BidirectionalIter __first_cut = __first;
  _BidirectionalIter __second_cut = __middle;
  _Distance __len11 = 0;
  _Distance __len22 = 0;
  if (__len1 > __len2) {
    __len11 = __len1 / 2;
    advance(__first_cut, __len11);
    __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
    distance(__middle, __second_cut, __len22);
  }
  else {
    __len22 = __len2 / 2;
    advance(__second_cut, __len22);
    __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
    distance(__first, __first_cut, __len11);
  }
  _BidirectionalIter __new_middle
    = rotate(__first_cut, __middle, __second_cut);
  __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22,
                         __comp);
  __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11,
                         __len2 - __len22, __comp);
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter1 __rotate_adaptive(_BidirectionalIter1 __first, _BidirectionalIter1 __middle, _BidirectionalIter1 __last, _Distance __len1, _Distance __len2, _BidirectionalIter2 __buffer, _Distance __buffer_size) 
{
  _BidirectionalIter2 __buffer_end;
  // 缓冲区足够安置序列二
  if (__len1 > __len2 && __len2 <= __buffer_size) 
  {
    __buffer_end = copy(__middle, __last, __buffer);
    copy_backward(__first, __middle, __last);
    return copy(__buffer, __buffer_end, __first);
  }
  // 缓冲区足够安置序列一
  else if (__len1 <= __buffer_size) 
  {
    __buffer_end = copy(__first, __middle, __buffer);
    copy(__middle, __last, __first);
    return copy_backward(__buffer, __buffer_end, __last);
  }
  // 缓冲区仍不足
  else
    return rotate(__first, __middle, __last);
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (*__last2 < *__last1) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __merge_backward(_BidirectionalIter1 __first1,
                                     _BidirectionalIter1 __last1,
                                     _BidirectionalIter2 __first2,
                                     _BidirectionalIter2 __last2,
                                     _BidirectionalIter3 __result,
                                     _Compare __comp) {
  if (__first1 == __last1)
    return copy_backward(__first2, __last2, __result);
  if (__first2 == __last2)
    return copy_backward(__first1, __last1, __result);
  --__last1;
  --__last2;
  while (true) {
    if (__comp(*__last2, *__last1)) {
      *--__result = *__last1;
      if (__first1 == __last1)
        return copy_backward(__first2, ++__last2, __result);
      --__last1;
    }
    else {
      *--__result = *__last2;
      if (__first2 == __last2)
        return copy_backward(__first1, ++__last1, __result);
      --__last2;
    }
  }
}

// 版本一辅助函数，有缓冲区的情况
template <class _BidirectionalIter, class _Distance, class _Pointer>
void __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last, _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size) 
{
  // 首先判断缓冲区是否足以容纳inplace_merge所接受的两个序列中的任何一个。
  // 如果空间充裕，则把两个序列中的某一个copy到缓冲区中，再使用merge完成其余工作。
  if (__len1 <= __len2 && __len1 <= __buffer_size) 
  {
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
    merge(__buffer, __buffer_end, __middle, __last, __first);
  }
  else if (__len2 <= __buffer_size) 
  {
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last);
  }
  // 但是当缓冲区不足以容纳任何一个序列时，我们的处理原则是：以递归分割(recursive partitioning)的方式，让处理长度减半，看看能否容纳于缓冲区中。
  else 
  {
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    // 拿较长的序列开刀
    if (__len1 > __len2) 
    {
      __len11 = __len1 / 2;
      advance(__first_cut, __len11);
      __second_cut = lower_bound(__middle, __last, *__first_cut);
      distance(__middle, __second_cut, __len22); 
    }
    else 
    {
      // 计算序列二的长度的一半
      __len22 = __len2 / 2;
      advance(__second_cut, __len22);
      __first_cut = upper_bound(__first, __middle, *__second_cut);
      distance(__first, __first_cut, __len11);
    }
    // 这个__rotate_adaptive函数的功效和STL算法rotate并没有什么不同，只是它针对缓冲区的存在，做了优化。万一缓冲区不足，最终还是交给STL算法rotate去执行。
    _BidirectionalIter __new_middle = __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11, __len22, __buffer, __buffer_size);
    // 现在可以分段处理了。首先针对左段[first, first_cut, new_middle)做递归调用。
    __merge_adaptive(__first, __first_cut, __new_middle, __len11, __len22, __buffer, __buffer_size);
    // 再针对右段[new_middle, second_cut, last)做递归调用。
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __buffer, __buffer_size);
  }
}

template <class _BidirectionalIter, class _Distance, class _Pointer,
          class _Compare>
void __merge_adaptive(_BidirectionalIter __first, 
                      _BidirectionalIter __middle, 
                      _BidirectionalIter __last,
                      _Distance __len1, _Distance __len2,
                      _Pointer __buffer, _Distance __buffer_size,
                      _Compare __comp) {
  if (__len1 <= __len2 && __len1 <= __buffer_size) {
    _Pointer __buffer_end = copy(__first, __middle, __buffer);
    merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
  }
  else if (__len2 <= __buffer_size) {
    _Pointer __buffer_end = copy(__middle, __last, __buffer);
    __merge_backward(__first, __middle, __buffer, __buffer_end, __last,
                     __comp);
  }
  else {
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
      __len11 = __len1 / 2;
      advance(__first_cut, __len11);
      __second_cut = lower_bound(__middle, __last, *__first_cut, __comp);
      distance(__middle, __second_cut, __len22);   
    }
    else {
      __len22 = __len2 / 2;
      advance(__second_cut, __len22);
      __first_cut = upper_bound(__first, __middle, *__second_cut, __comp);
      distance(__first, __first_cut, __len11);
    }
    _BidirectionalIter __new_middle =
      __rotate_adaptive(__first_cut, __middle, __second_cut, __len1 - __len11,
                        __len22, __buffer, __buffer_size);
    __merge_adaptive(__first, __first_cut, __new_middle, __len11,
                     __len22, __buffer, __buffer_size, __comp);
    __merge_adaptive(__new_middle, __second_cut, __last, __len1 - __len11,
                     __len2 - __len22, __buffer, __buffer_size, __comp);
  }
}

// 版本一辅助函数
template <class _BidirectionalIter, class _Tp, class _Distance>
inline void __inplace_merge_aux(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last, _Tp*, _Distance*) 
{
  // len1表示序列一的长度
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);
  // len2表示序列二的长度
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);

  // 本算法会使用额外的内存空间（暂时缓冲区）
  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  // 内存配置失败
  if (__buf.begin() == 0)
    __merge_without_buffer(__first, __middle, __last, __len1, __len2);
  // 在有暂时缓冲区的情况下进行，这个算法如果有额外的内存（缓冲区）辅助，效率会好许多。但是在没有缓冲区或缓冲区不足的情况下，也可以运作。
  else
    __merge_adaptive(__first, __middle, __last, __len1, __len2, __buf.begin(), _Distance(__buf.size()));
}

template <class _BidirectionalIter, class _Tp, 
          class _Distance, class _Compare>
inline void __inplace_merge_aux(_BidirectionalIter __first,
                                _BidirectionalIter __middle,
                                _BidirectionalIter __last, _Tp*, _Distance*,
                                _Compare __comp) {
  _Distance __len1 = 0;
  distance(__first, __middle, __len1);
  _Distance __len2 = 0;
  distance(__middle, __last, __len2);

  _Temporary_buffer<_BidirectionalIter, _Tp> __buf(__first, __last);
  if (__buf.begin() == 0)
    __merge_without_buffer(__first, __middle, __last, __len1, __len2, __comp);
  else
    __merge_adaptive(__first, __middle, __last, __len1, __len2,
                     __buf.begin(), _Distance(__buf.size()),
                     __comp);
}

/*
// 如果两个连接在一起的序列[first, middle)和[middle, last)都已排序，那么inplace_merge可将它们结合成单一一个序列，并仍保有序性(sorted)。
// 和merge一样，inplace_merge也是一种稳定(stable)操作。每个作为数据来源的子序列中的元素相对次序都不会变动；
// 如果两个子序列有等同的元素，第一序列的元素会被排在第二序列元素之前。
*/
// 版本一使用operator<进行比较
template <class _BidirectionalIter>
inline void inplace_merge(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last) 
{
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type, _LessThanComparable);
  if (__first == __middle || __middle == __last)
    return;
  __inplace_merge_aux(__first, __middle, __last, __VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
}

// 版本二使用仿函数comp进行比较
template <class _BidirectionalIter, class _Compare>
inline void inplace_merge(_BidirectionalIter __first,
                          _BidirectionalIter __middle,
                          _BidirectionalIter __last, _Compare __comp) {
  __STL_REQUIRES(_BidirectionalIter, _Mutable_BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
           typename iterator_traits<_BidirectionalIter>::value_type,
           typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __middle || __middle == __last)
    return;
  __inplace_merge_aux(__first, __middle, __last,
                      __VALUE_TYPE(__first), __DISTANCE_TYPE(__first),
                      __comp);
}

// Set algorithms: includes, set_union, set_intersection, set_difference,
// set_symmetric_difference.  All of these algorithms have the precondition
// that their input ranges are sorted and the postcondition that their output
// ranges are sorted.

// 判断是否序列二S2的每一个元素都在S1中出现。
// 注意，S1或S2内的元素都可以重复，
// 这种情况下所谓“S1 内含一个S2子集合”的定义是：假设某元素在S2出现n次，在S1出现m次，那么如果m < n，次算法会返回false。
// 序列一和序列二都是sorted range
template <class _InputIter1, class _InputIter2>
bool includes(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first2 < *__first1)
      return false;
    else if(*__first1 < *__first2) 
      ++__first1;
    else
      ++__first1, ++__first2;

  return __first2 == __last2;
}

// 版本二。判断序列一内是否有个子序列（不一定连续），其与序列二的每个对应元素都满足二元运算compare
template <class _InputIter1, class _InputIter2, class _Compare>
bool includes(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _Compare __comp) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES_SAME_TYPE( typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first2, *__first1))
      return false;
    else if(__comp(*__first1, *__first2)) 
      ++__first1;
    else
      ++__first1, ++__first2;

  return __first2 == __last2;
}

/*
// 算法set_union可构造S1、S2的并集。也就是说，它能构造出集合S1 ∪ S2，此集合中含有S1或S2内的每一个元素。
// 返回值为一个迭代器，指向输出区间的尾端。
// 由于S1和S2内的元素都不需要唯一，因此，如果某个值在S1出现n次，在S2出现m次，那么该值在输出区间中会出现max(m, n)次。
// set_union是一种稳定(stable)操作，意思是输入区间的每个元素的相对顺序都不会改变。
// set是一种sorted range。这是以下算法的前提。
*/
// 版本一使用operator<进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE( typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2) 
  {
    if (*__first1 < *__first2) 
    {
      *__result = *__first1;
      ++__first1;
    }
    else if (*__first2 < *__first1) 
    {
      *__result = *__first2;
      ++__first2;
    }
    else 
    {
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;
  }
  // 将尚未到达尾端的区间的所有剩余元素拷贝到目的端，[first1, last1),[first2, last2)。
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

// 版本二使用_Compare进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_union(_InputIter1 __first1, _InputIter1 __last1,
                      _InputIter2 __first2, _InputIter2 __last2,
                      _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2) {
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
    }
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
    }
    ++__result;
  }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

/*
// 算法set_intersection可构造S1、S2的交集。也就是说，它能构造出集合S1 ∩ S2，此集合中含有同时出现于S1和S2内的每一个元素。
// 返回值为一个迭代器，指向输出区间的尾端。
// 由于S1和S2内的元素都不需要唯一，因此，如果某个值在S1出现n次，在S2出现m次，那么该值在输出区间中会出现min(m, n)次。
// set_intersection是一种稳定(stable)操作，意思是输入区间的每个元素的相对顺序都不会改变。
// set是一种sorted range。这是以下算法的前提。
*/
// 版本一使用operator<进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2) 
    if (*__first1 < *__first2) 
      ++__first1;
    else if (*__first2 < *__first1) 
      ++__first2;
    else 
    {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}

// 版本二使用_Compare进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter set_intersection(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);

  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2))
      ++__first1;
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      *__result = *__first1;
      ++__first1;
      ++__first2;
      ++__result;
    }
  return __result;
}

/*
// 算法set_difference可构造S1、S2的差集。也就是说，它能构造出集合S1 - S2，此集合中含有出现于S1但不出现于S2的每一个元素。
// 返回值为一个迭代器，指向输出区间的尾端。
// 由于S1和S2内的元素都不需要唯一，因此，如果某个值在S1出现n次，在S2出现m次，那么该值在输出区间中会出现max(n - m, 0)次。
// set_difference是一种稳定(stable)操作，意思是输入区间的每个元素的相对顺序都不会改变。
// set是一种sorted range。这是以下算法的前提。
*/
// 版本一使用operator<进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first1 < *__first2) 
    {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (*__first2 < *__first1)
      ++__first2;
    else 
    {
      ++__first1;
      ++__first2;
    }
  return copy(__first1, __last1, __result);
}

// 版本二使用_Compare进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter, 
          class _Compare>
_OutputIter set_difference(_InputIter1 __first1, _InputIter1 __last1,
                           _InputIter2 __first2, _InputIter2 __last2, 
                           _OutputIter __result, _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);

  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1))
      ++__first2;
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first1, __last1, __result);
}

/*
// 算法set_symmetric_difference可构造S1、S2的对称差集。也就是说，它能构造出集合(S1 - S2) ∪ (S2 - S1)，此集合中含有出现于S1但不出现于S2的每一个元素以及出现于S2但不出现于S1的每一个元素。
// 返回值为一个迭代器，指向输出区间的尾端。
// 由于S1和S2内的元素都不需要唯一，因此，如果某个值在S1出现n次，在S2出现m次，那么该值在输出区间中会出现|n - m|次。
// set_symmetric_difference是一种稳定(stable)操作，意思是输入区间的每个元素的相对顺序都不会改变。
// set是一种sorted range。这是以下算法的前提。
*/
// 版本一使用operator<进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter>
_OutputIter set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2, _OutputIter __result) 
{
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(typename iterator_traits<_InputIter1>::value_type, typename iterator_traits<_InputIter2>::value_type);
  __STL_REQUIRES(typename iterator_traits<_InputIter1>::value_type, _LessThanComparable);
  while (__first1 != __last1 && __first2 != __last2)
    if (*__first1 < *__first2) 
    {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (*__first2 < *__first1) 
    {
      *__result = *__first2;
      ++__first2;
      ++__result;
    }
    else 
    {
      ++__first1;
      ++__first2;
    }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

// 版本二使用_Compare进行比较
template <class _InputIter1, class _InputIter2, class _OutputIter,
          class _Compare>
_OutputIter 
set_symmetric_difference(_InputIter1 __first1, _InputIter1 __last1,
                         _InputIter2 __first2, _InputIter2 __last2,
                         _OutputIter __result,
                         _Compare __comp) {
  __STL_REQUIRES(_InputIter1, _InputIterator);
  __STL_REQUIRES(_InputIter2, _InputIterator);
  __STL_REQUIRES(_OutputIter, _OutputIterator);
  __STL_REQUIRES_SAME_TYPE(
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool,
       typename iterator_traits<_InputIter1>::value_type,
       typename iterator_traits<_InputIter2>::value_type);
  while (__first1 != __last1 && __first2 != __last2)
    if (__comp(*__first1, *__first2)) {
      *__result = *__first1;
      ++__first1;
      ++__result;
    }
    else if (__comp(*__first2, *__first1)) {
      *__result = *__first2;
      ++__first2;
      ++__result;
    }
    else {
      ++__first1;
      ++__first2;
    }
  return copy(__first2, __last2, copy(__first1, __last1, __result));
}

// min_element and max_element, with and without an explicitly supplied
// comparison function.

// 返回一个迭代器，指向序列之中数值最大的元素
template <class _ForwardIter>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type, _LessThanComparable);
  if (__first == __last) 
    return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (*__result < *__first)
      __result = __first;
  return __result;
}

template <class _ForwardIter, class _Compare>
_ForwardIter max_element(_ForwardIter __first, _ForwardIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_ForwardIter>::value_type, typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__result, *__first)) 
      __result = __first;
  return __result;
}

// 返回一个迭代器，指向序列之中数值最小的元素
template <class _ForwardIter>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type, _LessThanComparable);
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (*__first < *__result)
      __result = __first;
  return __result;
}

template <class _ForwardIter, class _Compare>
_ForwardIter min_element(_ForwardIter __first, _ForwardIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_ForwardIter>::value_type, typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last) return __first;
  _ForwardIter __result = __first;
  while (++__first != __last) 
    if (__comp(*__first, *__result))
      __result = __first;
  return __result;
}

// next_permutation and prev_permutation, with and without an explicitly 
// supplied comparison function.

// 版本一使用元素型别提供的less-than操作符来决定下一个排列组合。
// next_permutation()会取得[first, last)所标示的序列的下一个排列组合。这些排列组合根据less-than操作符做字典顺序(lexicongraphical)的排序。
template <class _BidirectionalIter>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last) 
{
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type, _LessThanComparable);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) 
  {
    // 从尾端开始往前寻找两个相邻元素，令第一个元素为*i，第二个元素为*ii，且满足*i < *ii。
    _BidirectionalIter __ii = __i;
    --__i;
    if (*__i < *__ii) 
    {
      // 找到这样一组相邻元素后，再从尾端开始往前检查，找出第一个大于*i的元素，令其为*j。
      _BidirectionalIter __j = __last;
      while (!(*__i < *--__j))
      {

      }
      // 将i，j元素对调
      iter_swap(__i, __j);
      // 再将ii之后的所有元素颠倒排列。既得下一个排列组合。
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) 
    {
      reverse(__first, __last);
      return false;
    }
  }
}

// 版本二以仿函数comp决定下一个排列组合。
template <class _BidirectionalIter, class _Compare>
bool next_permutation(_BidirectionalIter __first, _BidirectionalIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_BidirectionalIter>::value_type, typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__i, *__ii)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*__i, *--__j))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    // 进行到最前面了
    if (__i == __first) 
    {
      // 全部逆向重排
      reverse(__first, __last);
      return false;
    }
  }
}

// next_permutation()会取得[first, last)所标示的序列的上一个排列组合。这些排列组合根据less-than操作符做字典顺序(lexicongraphical)的排序。
// 版本一使用元素型别提供的less-than操作符来决定上一个排列组合。
template <class _BidirectionalIter>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last) 
{
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_REQUIRES(typename iterator_traits<_BidirectionalIter>::value_type, _LessThanComparable);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) 
  {
    // 从尾端开始往前寻找两个相邻元素，令第一个元素为*i，第二个元素为*ii，且满足*ii < *i。
    _BidirectionalIter __ii = __i;
    --__i;
    if (*__ii < *__i)
    {
      _BidirectionalIter __j = __last;
      // 找到这样一组相邻元素后，再从尾端开始往前检查，找出第一个小于*i的元素，令其为*j。
      while (!(*--__j < *__i))
      {

      }
      // 将i，j元素对调
      iter_swap(__i, __j);
      // 再将ii之后的所有元素颠倒排列。既得下一个排列组合。
      reverse(__ii, __last);
      return true;
    }
    // 进行到最前面了
    if (__i == __first) 
    {
      // 全部逆向重排
      reverse(__first, __last);
      return false;
    }
  }
}

// 版本二以仿函数comp决定下一个排列组合。
template <class _BidirectionalIter, class _Compare>
bool prev_permutation(_BidirectionalIter __first, _BidirectionalIter __last, _Compare __comp) 
{
  __STL_REQUIRES(_BidirectionalIter, _BidirectionalIterator);
  __STL_BINARY_FUNCTION_CHECK(_Compare, bool, typename iterator_traits<_BidirectionalIter>::value_type, typename iterator_traits<_BidirectionalIter>::value_type);
  if (__first == __last)
    return false;
  _BidirectionalIter __i = __first;
  ++__i;
  if (__i == __last)
    return false;
  __i = __last;
  --__i;

  for(;;) {
    _BidirectionalIter __ii = __i;
    --__i;
    if (__comp(*__ii, *__i)) {
      _BidirectionalIter __j = __last;
      while (!__comp(*--__j, *__i))
        {}
      iter_swap(__i, __j);
      reverse(__ii, __last);
      return true;
    }
    if (__i == __first) {
      reverse(__first, __last);
      return false;
    }
  }
}

// find_first_of, with and without an explicitly supplied comparison function.

// 在序列一中寻找序列二中任意一个元素首次出现于序列一的位置
// 如果第一序列并未内含第二序列的任何元素，返回的将是last1
// 版本一使用元素型别提供的equality操作符。
template <class _InputIter, class _ForwardIter>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_InputIter>::value_type, typename iterator_traits<_ForwardIter>::value_type);

  for ( ; __first1 != __last1; ++__first1) 
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (*__first1 == *__iter)
        return __first1;
  return __last1;
}

// 版本二允许用户指定一个二元运算pred。
template <class _InputIter, class _ForwardIter, class _BinaryPredicate>
_InputIter find_first_of(_InputIter __first1, _InputIter __last1,
                         _ForwardIter __first2, _ForwardIter __last2,
                         _BinaryPredicate __comp)
{
  __STL_REQUIRES(_InputIter, _InputIterator);
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
     typename iterator_traits<_InputIter>::value_type,
     typename iterator_traits<_ForwardIter>::value_type);

  for ( ; __first1 != __last1; ++__first1) 
    for (_ForwardIter __iter = __first2; __iter != __last2; ++__iter)
      if (__comp(*__first1, *__iter))
        return __first1;
  return __last1;
}


// find_end, with and without an explicitly supplied comparison function.
// Search [first2, last2) as a subsequence in [first1, last1), and return
// the *last* possible match.  Note that find_end for bidirectional iterators
// is much faster than for forward iterators.

// find_end for forward iterators. 
// forward iterator版
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2, forward_iterator_tag, forward_iterator_tag)
{
  // 如果查找目标是空的，返回last1，表示该“空子序列”的最后出现点。
  if (__first2 == __last2)
    return __last1;
  else 
  {
    _ForwardIter1 __result = __last1;
    while (1) 
    {
      // 利用search()查找某个子序列的首次出现点。找不到的话返回last1。
      _ForwardIter1 __new_result = search(__first1, __last1, __first2, __last2);
      if (__new_result == __last1)
        return __result;
      // 因为find_end()是查找最后一次出现地点，因此准备下一个查找行动
      else 
      {
        // 在下一个查找行动开始之前记录已找到的位置
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
      }
    }
  }
}

template <class _ForwardIter1, class _ForwardIter2,
          class _BinaryPredicate>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,
                         _ForwardIter2 __first2, _ForwardIter2 __last2,
                         forward_iterator_tag, forward_iterator_tag,
                         _BinaryPredicate __comp)
{
  if (__first2 == __last2)
    return __last1;
  else {
    _ForwardIter1 __result = __last1;
    while (1) {
      _ForwardIter1 __new_result
        = search(__first1, __last1, __first2, __last2, __comp);
      if (__new_result == __last1)
        return __result;
      else {
        __result = __new_result;
        __first1 = __new_result;
        ++__first1;
      }
    }
  }
}

// find_end for bidirectional iterators.  Requires partial specialization.
#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

// bidirectional iterators版（可逆向查找）
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter1 __find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1, _BidirectionalIter2 __first2, _BidirectionalIter2 __last2, bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  __STL_REQUIRES(_BidirectionalIter1, _BidirectionalIterator);
  __STL_REQUIRES(_BidirectionalIter2, _BidirectionalIterator);
  // 由于查找的是“最后出现地点”，因此反向查找比较快，利用reverse_iterator。
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  // 查找时，将序列一和序列二统统逆转方向。
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1, _RevIter2(__last2), __rlast2);

  if (__rresult == __rlast1)
    return __last1;
  else 
  {
    // 转回正常（非逆向迭代器）
    _BidirectionalIter1 __result = __rresult.base();
    // 调整回到子序列的起头处
    advance(__result, -distance(__first2, __last2));
    return __result;
  }
}

template <class _BidirectionalIter1, class _BidirectionalIter2,
          class _BinaryPredicate>
_BidirectionalIter1
__find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
           _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag, 
           _BinaryPredicate __comp)
{
  __STL_REQUIRES(_BidirectionalIter1, _BidirectionalIterator);
  __STL_REQUIRES(_BidirectionalIter2, _BidirectionalIterator);
  typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
  typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

  _RevIter1 __rlast1(__first1);
  _RevIter2 __rlast2(__first2);
  _RevIter1 __rresult = search(_RevIter1(__last1), __rlast1,
                               _RevIter2(__last2), __rlast2,
                               __comp);

  if (__rresult == __rlast1)
    return __last1;
  else {
    _BidirectionalIter1 __result = __rresult.base();
    advance(__result, -distance(__first2, __last2));
    return __result;
  }
}
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

// Dispatching functions for find_end.

/*
// 在序列一[first1, last1)所涵盖的区间中，查找序列二[first2, last2)的最后一次出现点。
// 如果序列一之内不存在“完全匹配序列二”的子序列，便返回迭代器last1。
*/
// 逆向查找的关键在于迭代器的双向移动能力，因此，SGI将算法设计为双层架构，
// 一般称呼此种上层函数为dispatch function（分派函数、派送函数）。
template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter1 find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2)
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_REQUIRES_BINARY_OP(_OP_EQUAL, bool, typename iterator_traits<_ForwardIter1>::value_type, typename iterator_traits<_ForwardIter2>::value_type);
  // 根据两个区间的类属，调用不同的下层函数。
  // 这是一种很常见的技巧，令函数传递调用过程中产生迭代器类型(iterator category)的临时对象，
  // 再利用编译器的推导机制(argument deduction)，自动调用某个对应的函数。此例中对应函数有两个候选者。
  return __find_end(__first1, __last1, __first2, __last2, __ITERATOR_CATEGORY(__first1), __ITERATOR_CATEGORY(__first2));
}

template <class _ForwardIter1, class _ForwardIter2, 
          class _BinaryPredicate>
inline _ForwardIter1 
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, 
         _ForwardIter2 __first2, _ForwardIter2 __last2,
         _BinaryPredicate __comp)
{
  __STL_REQUIRES(_ForwardIter1, _ForwardIterator);
  __STL_REQUIRES(_ForwardIter2, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_BinaryPredicate, bool,
   typename iterator_traits<_ForwardIter1>::value_type,
   typename iterator_traits<_ForwardIter2>::value_type);

  return __find_end(__first1, __last1, __first2, __last2,
                    __ITERATOR_CATEGORY(__first1),
                    __ITERATOR_CATEGORY(__first2),
                    __comp);
}

// is_heap, a predicate testing whether or not a range is
// a heap.  This function is an extension, not part of the C++
// standard.

template <class _RandomAccessIter, class _Distance>
bool __is_heap(_RandomAccessIter __first, _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__first[__parent] < __first[__child]) 
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter, class _Distance, class _StrictWeakOrdering>
bool __is_heap(_RandomAccessIter __first, _StrictWeakOrdering __comp,
               _Distance __n)
{
  _Distance __parent = 0;
  for (_Distance __child = 1; __child < __n; ++__child) {
    if (__comp(__first[__parent], __first[__child]))
      return false;
    if ((__child & 1) == 0)
      ++__parent;
  }
  return true;
}

template <class _RandomAccessIter>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last)
{
  __STL_REQUIRES(_RandomAccessIter, _RandomAccessIterator);
  __STL_REQUIRES(typename iterator_traits<_RandomAccessIter>::value_type,
                 _LessThanComparable);
  return __is_heap(__first, __last - __first);
}


template <class _RandomAccessIter, class _StrictWeakOrdering>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last,
                    _StrictWeakOrdering __comp)
{
  __STL_REQUIRES(_RandomAccessIter, _RandomAccessIterator);
  __STL_BINARY_FUNCTION_CHECK(_StrictWeakOrdering, bool, 
         typename iterator_traits<_RandomAccessIter>::value_type, 
         typename iterator_traits<_RandomAccessIter>::value_type);
  return __is_heap(__first, __comp, __last - __first);
}

// is_sorted, a predicated testing whether a range is sorted in
// nondescending order.  This is an extension, not part of the C++
// standard.

template <class _ForwardIter>
bool is_sorted(_ForwardIter __first, _ForwardIter __last)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_REQUIRES(typename iterator_traits<_ForwardIter>::value_type,
                 _LessThanComparable);
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (*__next < *__first)
      return false;
  }

  return true;
}

template <class _ForwardIter, class _StrictWeakOrdering>
bool is_sorted(_ForwardIter __first, _ForwardIter __last,
               _StrictWeakOrdering __comp)
{
  __STL_REQUIRES(_ForwardIter, _ForwardIterator);
  __STL_BINARY_FUNCTION_CHECK(_StrictWeakOrdering, bool, 
        typename iterator_traits<_ForwardIter>::value_type,
        typename iterator_traits<_ForwardIter>::value_type);
  if (__first == __last)
    return true;

  _ForwardIter __next = __first;
  for (++__next; __next != __last; __first = __next, ++__next) {
    if (__comp(*__next, *__first))
      return false;
  }

  return true;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1209
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALGO_H */

// Local Variables:
// mode:C++
// End:
