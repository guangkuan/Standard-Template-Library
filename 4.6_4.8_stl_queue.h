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
 * Copyright (c) 1996,1997
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

#ifndef __SGI_STL_INTERNAL_QUEUE_H
#define __SGI_STL_INTERNAL_QUEUE_H

#include <sequence_concepts.h>
#include <vector>

__STL_BEGIN_NAMESPACE

// Forward declarations of operators < and ==, needed for friend declaration.

//以deque为默认底部结构
template <class _Tp, class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(deque<_Tp>) > class queue;

template <class _Tp, class _Seq>
inline bool operator==(const queue<_Tp, _Seq>&, const queue<_Tp, _Seq>&);

template <class _Tp, class _Seq>
inline bool operator<(const queue<_Tp, _Seq>&, const queue<_Tp, _Seq>&);

/*
//queue是一种先进先出(First In First Out, FIFO)的数据结构。
//它有两个出口。queue允许新增元素、移除元素、从最底端加入元素、取得最顶端元素。
//除了最底端可以加入、最顶端可以取出外，没有任何其它办法可以存取queue的其它元素。
//queue所有元素的进出都必须符合“先进先出”的条件，只有queue顶端元素，才有机会被外界取用。
//queue不提供遍历功能，也不提供迭代器。
//queue也是一种适配器
*/
template <class _Tp, class _Sequence>
class queue {

  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_REQUIRES(_Sequence, _FrontInsertionSequence);
  __STL_CLASS_REQUIRES(_Sequence, _BackInsertionSequence);
  typedef typename _Sequence::value_type _Sequence_value_type;
  __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);


#ifdef __STL_MEMBER_TEMPLATES 
  template <class _Tp1, class _Seq1>
  friend bool operator== (const queue<_Tp1, _Seq1>&,
                          const queue<_Tp1, _Seq1>&);
  template <class _Tp1, class _Seq1>
  friend bool operator< (const queue<_Tp1, _Seq1>&,
                         const queue<_Tp1, _Seq1>&);
#else /* __STL_MEMBER_TEMPLATES */
  friend bool __STD_QUALIFIER
  operator== __STL_NULL_TMPL_ARGS (const queue&, const queue&);
  friend bool __STD_QUALIFIER
  operator<  __STL_NULL_TMPL_ARGS (const queue&, const queue&);
#endif /* __STL_MEMBER_TEMPLATES */

public:
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
protected:
  _Sequence c;
public:
  queue() : c() {}
  explicit queue(const _Sequence& __c) : c(__c) {}

  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  reference front() { return c.front(); }
  const_reference front() const { return c.front(); }
  reference back() { return c.back(); }
  const_reference back() const { return c.back(); }
  void push(const value_type& __x) { c.push_back(__x); }
  void pop() { c.pop_front(); }
};

template <class _Tp, class _Sequence>
bool 
operator==(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __x.c == __y.c;
}

template <class _Tp, class _Sequence>
bool
operator<(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __x.c < __y.c;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _Tp, class _Sequence>
bool
operator!=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__x == __y);
}

template <class _Tp, class _Sequence>
bool 
operator>(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return __y < __x;
}

template <class _Tp, class _Sequence>
bool 
operator<=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__y < __x);
}

template <class _Tp, class _Sequence>
bool 
operator>=(const queue<_Tp, _Sequence>& __x, const queue<_Tp, _Sequence>& __y)
{
  return !(__x < __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */
/*
//priority_queue是一个拥有权值观念的queue（自动依照元素的权值排列（通常权值以实值表示））。权值最高者，排在最前面。
//它允许加入新元素，移除旧元素、审视元素值等功能。
//由于这是一个queue，所以只允许在底端加入元素，并从顶端取出元素，除此之外别无其他存取元素的途径。
//默认情况priority_queue利用一个max-heap完成。因此它也是一个适配器。
*/
template <class _Tp, class _Sequence __STL_DEPENDENT_DEFAULT_TMPL(std::vector<_Tp>), class _Compare __STL_DEPENDENT_DEFAULT_TMPL(less<typename _Sequence::value_type>) >
class priority_queue {

  // requirements:

  __STL_CLASS_REQUIRES(_Tp, _Assignable);
  __STL_CLASS_REQUIRES(_Sequence, _Sequence);
  __STL_CLASS_REQUIRES(_Sequence, _RandomAccessContainer);
  typedef typename _Sequence::value_type _Sequence_value_type;
  __STL_CLASS_REQUIRES_SAME_TYPE(_Tp, _Sequence_value_type);
  __STL_CLASS_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);

public:
  typedef typename _Sequence::value_type      value_type;
  typedef typename _Sequence::size_type       size_type;
  typedef          _Sequence                  container_type;

  typedef typename _Sequence::reference       reference;
  typedef typename _Sequence::const_reference const_reference;
protected:
  //底层容器vector
  _Sequence c;
  _Compare comp;
public:
  priority_queue() : c() {}
  explicit priority_queue(const _Compare& __x) :  c(), comp(__x) {}
  priority_queue(const _Compare& __x, const _Sequence& __s) : c(__s), comp(__x) { make_heap(c.begin(), c.end(), comp); }

#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last) 
    : c(__first, __last) { make_heap(c.begin(), c.end(), comp); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, 
                 _InputIterator __last, const _Compare& __x)
    : c(__first, __last), comp(__x) 
    { make_heap(c.begin(), c.end(), comp); }

  template <class _InputIterator>
  priority_queue(_InputIterator __first, _InputIterator __last,
                 const _Compare& __x, const _Sequence& __s)
  : c(__s), comp(__x)
  { 
    c.insert(c.end(), __first, __last);
    make_heap(c.begin(), c.end(), comp);
  }

#else /* __STL_MEMBER_TEMPLATES */
  priority_queue(const value_type* __first, const value_type* __last) 
    : c(__first, __last) { make_heap(c.begin(), c.end(), comp); }

  priority_queue(const value_type* __first, const value_type* __last, 
                 const _Compare& __x) 
    : c(__first, __last), comp(__x)
    { make_heap(c.begin(), c.end(), comp); }

  priority_queue(const value_type* __first, const value_type* __last, 
                 const _Compare& __x, const _Sequence& __c)
    : c(__c), comp(__x) 
  { 
    c.insert(c.end(), __first, __last);
    make_heap(c.begin(), c.end(), comp);
  }
#endif /* __STL_MEMBER_TEMPLATES */

  bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }
  const_reference top() const { return c.front(); }
  void push(const value_type& __x) 
  {
    __STL_TRY 
    {
      //push_heap是泛型算法，先利用底层容器的push_back()将新元素推入尾端，再重排heap。
      c.push_back(__x); 
      push_heap(c.begin(), c.end(), comp);
    }
    __STL_UNWIND(c.clear());
  }
  void pop() 
  {
    __STL_TRY 
    {
      //pop_heap是泛型算法，先重排heap，再利用底层容器的pop_back()取得尾端被弹出的元素。
      pop_heap(c.begin(), c.end(), comp);
      c.pop_back();
    }
    __STL_UNWIND(c.clear());
  }
};

// no equality is provided

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_QUEUE_H */

// Local Variables:
// mode:C++
// End:
