#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>        // std::copy, std::equal, std::fill
#include <cassert>          // assert()
#include <cstddef>          // std::size_t
#include <exception>        // std::out_of_range
#include <initializer_list> // std::initializer_list
#include <iostream>         // std::cout, std::endl
#include <iterator> // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <limits> // std::numeric_limits<T>
#include <memory> // std::unique_ptr

/// Sequence container namespace.
namespace sc {
/// Implements tha infrastrcture to support a bidirectional iterator.
template <class T> class MyForwardIterator {
public:
  using iterator = MyForwardIterator; //!< Alias to iterator.
  // Below we have the iterator_traits common interface
  typedef std::ptrdiff_t
      difference_type;  //!< Difference type used to calculated distance between
                        //!< iterators.
  typedef T value_type; //!< Value type the iterator points to.
  typedef T *pointer;   //!< Pointer to the value type.
  typedef T &reference; //!< Reference to the value type.
  typedef const T &const_reference; //!< Reference to the value type.
  typedef std::bidirectional_iterator_tag
      iterator_category; //!< Iterator category.

  /*! Create an iterator around a raw pointer.
   * \param pt_ raw pointer to the container.
   */
  MyForwardIterator(pointer pt = nullptr) : m_ptr(pt) { /* empty */
  }

  /// Access the content the iterator points to.
  reference operator*(void) const {
    assert(m_ptr != nullptr);
    return *m_ptr;
  }

  /// Overloaded `->` operator.
  pointer operator->(void) const {
    assert(m_ptr != nullptr);
    return m_ptr;
  }

  /// Assignment operator.
  iterator &operator=(const iterator &) = default;
  /// Copy constructor.
  MyForwardIterator(const iterator &) = default;

  /// Pre-increment operator.
  iterator operator++(void) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    ++m_ptr; // Increment the raw pointer to the next element

    return *this; // Return the updated iterator
  }
  /// Post-increment operator.
  iterator operator++(int) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    iterator original(*this); // Create a copy of the original iterator

    ++m_ptr; // Increment the raw pointer to the next element

    return original; // Return the original iterator
  }

  /// Pre-decrement operator.
  iterator operator--(void) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    --m_ptr; // Decrement the raw pointer to the previous element

    return *this; // Return the updated iterator
  }

  /// Post-decrement operator.
  iterator operator--(int) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    iterator original(*this); // Create a copy of the original iterator

    --m_ptr; // Decrement the raw pointer to the previous element

    return original; // Return the original iterator
  }

  iterator &operator+=(difference_type offset) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    m_ptr += offset; // Advance the raw pointer by the given offset

    return *this; // Return the updated iterator
  }

  iterator &operator-=(difference_type offset) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    m_ptr -= offset; // Move the raw pointer backward by the given offset

    return *this; // Return the updated iterator
  }

  friend bool operator<(const iterator& ita, const iterator& itb) {
    return ita.m_ptr < itb.m_ptr;
  }
  friend bool operator>(const iterator& ita, const iterator& itb) {
    return itb < ita; // Utilize the already defined operator< to implement operator>
  }
  friend bool operator>=(const iterator& ita, const iterator& itb) {
    return !(ita < itb); // Utilize the already defined operator< to implement operator>=
  }
  friend bool operator<=(const iterator& ita, const iterator& itb) {
    return !(ita > itb); // Utilize the already defined operator> to implement operator<=
  }

  friend iterator operator+(difference_type offset, iterator it) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr += offset; // Advance the raw pointer by the given offset

    return it; // Return the updated iterator
  }
  friend iterator operator+(iterator it, difference_type offset) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr += offset; // Advance the raw pointer by the given offset

    return it; // Return the updated iterator
  }
  friend iterator operator-(iterator it, difference_type offset) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr -= offset; // Move the raw pointer backward by the given offset

    return it; // Return the updated iterator
  }

  /// Equality operator.
  bool operator==(const iterator& rhs_) const {
    return m_ptr == rhs_.m_ptr; // Compare the raw pointers of the iterators
  }

  /// Not equality operator.
  bool operator!=(const iterator& rhs_) const {
    return !(*this == rhs_); // Utilize the already defined operator== to implement operator!=
  }


  /// Returns the difference between two iterators.
  difference_type operator-(const iterator& rhs_) const {
    assert(m_ptr != nullptr && rhs_.m_ptr != nullptr); // Check if the iterators are valid

    return m_ptr - rhs_.m_ptr; // Calculate the difference between the raw pointers
  }


  /// Stream extractor operator.
  friend std::ostream &operator<<(std::ostream &os_,
                                  const MyForwardIterator &p_) {
    os_ << "[@ " << p_.m_ptr << ": " << *p_.m_ptr << " ]";
    return os_;
  }

private:
  pointer m_ptr; //!< The raw pointer.
};

/// This class implements the ADT list with dynamic array.
/*!
 * sc::vector is a sequence container that encapsulates dynamic size arrays.
 *
 * The elements are stored contiguously, which means that elements can
 * be accessed not only through iterators, but also using offsets to
 * regular pointers to elements.
 * This means that a pointer to an element of a vector may be passed to
 * any function that expects a pointer to an element of an array.
 *
 * \tparam T The type of the elements.
 */
template <typename T> class vector {
  //=== Aliases
public:
  using size_type = unsigned long; //!< The size type.
  using value_type = T;            //!< The value type.
  using pointer = value_type *; //!< Pointer to a value stored in the container.
  using reference =
      value_type &; //!< Reference to a value stored in the container.
  using const_reference = const value_type &; //!< Const reference to a value
                                              //!< stored in the container.

  using iterator =
      MyForwardIterator<value_type>; //!< The iterator, instantiated from a
                                     //!< template class.
  using const_iterator =
      MyForwardIterator<const value_type>; //!< The const_iterator,
                                           //!< instantiated from a template
                                           //!< class.

public:
  //=== [I] SPECIAL MEMBERS (6 OF THEM)
  explicit vector(size_type cp = 0) {
    m_storage = new T[cp];
    m_capacity = cp;
    m_end = cp; // Array começa vazio.
    /* for (size_type i{0}; i < m_end; ++i) {
      m_storage[i] = new T();
    } */
  }
  virtual ~vector(void) {
    if (m_storage)
      delete[] m_storage;
  }
  vector(const vector &);
  vector(const std::initializer_list<T> &il) {
    m_capacity = il.size();
    m_storage = new T[m_capacity];
    m_end = m_capacity; // Array começa cheio.
    // Copy the elements from the il into the array.
    std::copy(il.begin(), il.end(), m_storage);
  }
  template <typename InputItr> vector(InputItr, InputItr);

  vector &operator=(const vector &);

  //=== [II] ITERATORS
  iterator begin(void) { return iterator{&m_storage[0]}; }
  iterator end(void);
  const_iterator cbegin(void) const;
  const_iterator cend(void) const;

  // [III] Capacity
  size_type size(void) const { return m_end; }
  size_type capacity(void) const { return m_capacity; }
  bool empty(void) const { return m_end == 0; }

  // [IV] Modifiers
  void clear(void);
  void push_front(const_reference);
  void push_back(const_reference);
  void pop_back(void);
  void pop_front(void);

  iterator insert(iterator pos_, const_reference value_);
  iterator insert(const_iterator pos_, const_reference value_);

  template <typename InputItr>
  iterator insert(iterator pos_, InputItr first_, InputItr last_);
  template <typename InputItr>
  iterator insert(const_iterator pos_, InputItr first_, InputItr last_);

  iterator insert(iterator pos_,
                  const std::initializer_list<value_type> &ilist_);
  iterator insert(const_iterator pos_,
                  const std::initializer_list<value_type> &ilist_);

  void reserve(size_type);
  void shrink_to_fit(void);

  void assign(size_type count_, const_reference value_);
  void assign(const std::initializer_list<T> &ilist);
  template <typename InputItr> void assign(InputItr first, InputItr last);

  iterator erase(iterator first, iterator last);
  iterator erase(const_iterator first, const_iterator last);

  iterator erase(const_iterator pos);
  iterator erase(iterator pos);

  // [V] Element access
  const_reference back(void) const;
  const_reference front(void) const;
  reference back(void);
  reference front(void);
  const_reference operator[](size_type idx) const { return m_storage[idx]; }
  reference operator[](size_type idx) { return m_storage[idx]; }
  const_reference at(size_type) const;
  reference at(size_type);
  pointer data(void);
  const_reference data(void) const;

  // [VII] Friend functions.
  friend std::ostream &operator<<(std::ostream &os_, const vector<T> &v_) {
    // O que eu quero imprimir???
    os_ << "{ ";
    for (auto i{0u}; i < v_.m_capacity; ++i) {
      if (i == v_.m_end)
        os_ << "| ";
      os_ << v_.m_storage[i] << " ";
    }
    os_ << "}, m_end=" << v_.m_end << ", m_capacity=" << v_.m_capacity;

    return os_;
  }
  friend void swap(vector<T> &first_, vector<T> &second_) {
    // enable ADL
    using std::swap;

    // Swap each member of the class.
    swap(first_.m_end, second_.m_end);
    swap(first_.m_capacity, second_.m_capacity);
    swap(first_.m_storage, second_.m_storage);
  }

private:
  bool full(void) const;

  size_type
      m_end; //!< The list's current size (or index past-last valid element).
  size_type m_capacity; //!< The list's storage capacity.
  T *m_storage;         //!< The list's data storage area.
};

// [VI] Operators
template <typename T> bool operator==(const vector<T> &, const vector<T> &);
template <typename T> bool operator!=(const vector<T> &, const vector<T> &);

} // namespace sc.

#endif
