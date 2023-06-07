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


  /**
   * \brief Creates a new Vector with a size.
   * 
   * Creates a new Vector, allocating memory to it
   * and setting its size with the parameter received.
   * 
   * \param cp The size of the vector to be created.
   */
  explicit vector(size_type cp = 0) {
    m_storage = new T[cp];
    m_capacity = cp;
    m_end = cp; // Array começa vazio.
    /* for (size_type i{0}; i < m_end; ++i) {
      m_storage[i] = new T();
    } */
  }

  /**
   * \brief Class destructor.
   * 
   * This method frees the vector.
   */
  virtual ~vector(void) {
    if (m_storage)
      delete[] m_storage;
  }

  /**
   * \brief Copy constructor
   * 
   * Initializes the vector by copying the 
   * atributes of the object received by 
   * reference as a parameter.
   * 
   * \param other The vector that will be copied.
   */
  vector(const vector& other) {
    // Perform a deep copy of the elements

    m_capacity = other.m_capacity;
    m_end = other.m_end;
    m_storage = new T[m_capacity];

    // Copy each element from the source vector to the new vector
    for (size_type i = 0; i < m_end; ++i) {
      m_storage[i] = other.m_storage[i];
    }
  }

  /**
   * \brief Constructor from initialize list
   * 
   * Initializes the vector by copying the 
   * size of a list, and creating a new vector with this size.
   * Then, the new vector will receive all the elements of the 
   * list received as a parameter.
   * 
   * \param il The list whose elements will be copied.
   */
  vector(const std::initializer_list<T> &il) {
    m_capacity = il.size();
    m_storage = new T[m_capacity];
    m_end = m_capacity; // Array começa cheio.
    // Copy the elements from the il into the array.
    std::copy(il.begin(), il.end(), m_storage);
  }
  
  /**
   * \brief Constructor from range
   * 
   * Initializes the vector by copying a list by range, 
   * and creates a new vector, whose size will be difference
   * between the first and last pointer. Then, the elements of the list 
   * will be copied, using the ranges.
   * 
   * \param first The beggining of the list whose elements will be copied.
   * \param last The ending of the list whose elements will be copied.
   * 
   */
  template <typename InputItr>
  vector(InputItr first, InputItr last) {
    // Determine the size of the range
    size_type range_size = std::distance(first, last);

    // Allocate storage for the elements
    m_capacity = range_size;
    m_storage = new T[m_capacity];

    // Copy the elements from the range to the storage
    std::copy(first, last, m_storage);

    // Update the end iterator
    m_end = m_capacity;
  }

  /**
   * \brief Overloads the = operator.
   * 
   * Specifies the behavior of the = operator
   * when dealing with a vector. It is going to assign the atributes of
   * a vector to another one.
   * 
   * \param other The vector that will be assigned.
   * 
   * \return The vector where the assignment happened.
   */

  //vector &operator=(const vector &);
  vector& operator=(const vector& other) {
  if (this != &other) {  // Check for self-assignment
    // Deallocate current storage
    delete[] m_storage;

    // Perform a deep copy of the elements
    m_capacity = other.m_capacity;
    m_end = other.m_end;
    m_storage = new T[m_capacity];

    // Copy each element from the source vector to the target vector
    for (size_type i = 0; i < m_end; ++i) {
      m_storage[i] = other.m_storage[i];
    }
  }
  return *this;
}


  //=== [II] ITERATORS

  /**
   * \brief Funtion to get an iterator pointing to the first element of vector
   * 
   * This function returns the addres of the element that is
   * stored at the first position of the vector
   * 
   * \return The iterator that points to the first element of the vector.
   */
  iterator begin(void) {
     return iterator{&m_storage[0]}; 
  }

  /**
   * \brief Funtion to get an iterator pointing to the end of the vector
   * 
   * This function returns the addres of the element that is
   * stored at the last position of the vector
   * 
   * \return The iterator that points to the last element of the vector.
   */
  iterator end(void) {
    return iterator{&m_storage[m_end]};  // Points to one position past the last element
  }

  /**
   * \brief Funtion to get a constant iterator pointing to the first element of vector
   * 
   * This function returns the addres of the element that is
   * stored at the first position of the vector
   * 
   * \return The constant iterator that points to the first element of the vector.
   */
  const_iterator cbegin(void) const {
    return const_iterator{&m_storage[0]};  // Points to the beginning of the vector
  }

  /**
   * \brief Funtion to get a constant iterator pointing to the last element of vector
   * 
   * This function returns the addres of the element that is
   * stored at the last position of the vector
   * 
   * \return The constant iterator that points to the last element of the vector.
   */
  const_iterator cend(void) const {
    return const_iterator{&m_storage[m_end]};  // Points to one position past the last element
  }


  // [III] Capacity

  /**
   * \brief Funtion to get the size of a vector
   * 
   * This function returns the size of the vector based on the number of 
   * elements that are currently stored inside of it.
   * 
   * \return The last position right after the last element of the 
   * vector, which indicates the size.
   */
  size_type size(void) const { 
    return m_end; 
  }

  /**
   * \brief Funtion to get the capacity of a vector
   * 
   * This function returns the capacity of the vector based on 
   * its homonimous atribute.
   * 
   * \return The vector capacity.
   */
  size_type capacity(void) const { 
    return m_capacity; 
  }

  /**
   * \brief Funtion to verify if the vector is empty
   * 
   * This function verifies if there is any element inside of the vector.
   * 
   * \return A logical value for the emptiness of the vector.
   */
  bool empty(void) const { 
    return m_end == 0; 
  }

  // [IV] Modifiers
  void clear(void) {
    delete[] m_storage;  // Deallocate the current storage
    m_storage = nullptr; // Set the storage pointer to nullptr
    m_capacity = 0;      // Reset the capacity
    m_end = 0;           // Reset the size
  }
  void push_front(const_reference); //Not TODO
  void push_back(const_reference value) {
    if (m_end == m_capacity) {
      // Need to reallocate storage
      size_type new_capacity = (m_capacity == 0) ? 1 : 2 * m_capacity;
      pointer new_storage = new value_type[new_capacity];
      // Copy existing elements to the new storage
      for (size_type i = 0; i < m_end; ++i) {
        new_storage[i] = m_storage[i];
      }
      // Deallocate the old storage
      delete[] m_storage;
      // Update member variables
      m_storage = new_storage;
      m_capacity = new_capacity;
    }
    // Add the new element at the end
    m_storage[m_end] = value;
    ++m_end;
  }

  void pop_back(void) {
    if (m_end > 0) {
      --m_end;
    }
  }

  void pop_front(void); //not TODO

  iterator insert(iterator pos_, const_reference value_) {
    // Calculate the index of the insertion position
    size_type index = pos_ - begin();
    if (m_end == m_capacity) {
      // Need to reallocate storage
      size_type new_capacity = (m_capacity == 0) ? 1 : 2 * m_capacity;
      pointer new_storage = new value_type[new_capacity];
      // Copy existing elements to the new storage, up to the insertion position
      for (size_type i = 0; i < index; ++i) {
        new_storage[i] = m_storage[i];
      }
      // Insert the new element at the insertion position
      new_storage[index] = value_;
      // Copy the remaining elements to the new storage
      for (size_type i = index; i < m_end; ++i) {
        new_storage[i + 1] = m_storage[i];
      }
      // Deallocate the old storage
      delete[] m_storage;
      // Update member variables
      m_storage = new_storage;
      m_capacity = new_capacity;
      ++m_end;
      // Return the iterator pointing to the inserted element
      return begin() + index;
    } else {
      // Enough capacity, shift elements to make room for the new element
      for (size_type i = m_end; i > index; --i) {
        m_storage[i] = m_storage[i - 1];
      }
      // Insert the new element at the insertion position
      m_storage[index] = value_;
      ++m_end;
      // Return the iterator pointing to the inserted element
      return begin() + index;
    }
  }


  iterator insert(const_iterator pos_, const_reference value_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, value_);
  }


  template <typename InputItr>
  iterator insert(iterator pos_, InputItr first_, InputItr last_) {
    // Calculate the index of the insertion position
    size_type index = pos_ - begin();
    // Calculate the number of elements to insert
    size_type num_elements = std::distance(first_, last_);
    if (m_end + num_elements > m_capacity) {
      // Need to reallocate storage
      size_type new_capacity = m_capacity + num_elements;
      pointer new_storage = new value_type[new_capacity];
      // Copy existing elements to the new storage, up to the insertion position
      for (size_type i = 0; i < index; ++i) {
        new_storage[i] = m_storage[i];
      }
      // Insert the new elements at the insertion position
      std::copy(first_, last_, new_storage + index);
      // Copy the remaining elements to the new storage
      for (size_type i = index; i < m_end; ++i) {
        new_storage[i + num_elements] = m_storage[i];
      }
      // Deallocate the old storage
      delete[] m_storage;
      // Update member variables
      m_storage = new_storage;
      m_capacity = new_capacity;
      m_end += num_elements;
      // Return the iterator pointing to the first inserted element
      return begin() + index;
    } else {
      // Enough capacity, shift elements to make room for the new elements
      for (size_type i = m_end - 1; i >= index; --i) {
        m_storage[i + num_elements] = m_storage[i];
      }
      // Insert the new elements at the insertion position
      std::copy(first_, last_, m_storage + index);
      // Update m_end to reflect the inserted elements
      m_end += num_elements;
      // Return the iterator pointing to the first inserted element
      return begin() + index;
    }
  }

  template <typename InputItr>
  iterator insert(const_iterator pos_, InputItr first_, InputItr last_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, first_, last_);
  }

  iterator insert(iterator pos_,
                  const std::initializer_list<value_type> &ilist_) {
    return insert(pos_, ilist_.begin(), ilist_.end());
  }
  iterator insert(const_iterator pos_,
                  const std::initializer_list<value_type> &ilist_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, ilist_.begin(), ilist_.end());
  }


  void reserve(size_type new_capacity) {
    if (new_capacity > m_capacity) {
      // Allocate new storage with the desired capacity
      pointer new_storage = new value_type[new_capacity];
      // Copy existing elements to the new storage
      for (size_type i = 0; i < m_end; ++i) {
        new_storage[i] = m_storage[i];
      }
      // Deallocate the old storage
      delete[] m_storage;
      // Update member variables
      m_storage = new_storage;
      m_capacity = new_capacity;
    }
  }

  void shrink_to_fit(void) {
    if (m_end < m_capacity) {
      // Allocate new storage with the desired capacity
      pointer new_storage = new value_type[m_end];
      // Copy existing elements to the new storage
      for (size_type i = 0; i < m_end; ++i) {
        new_storage[i] = m_storage[i];
      }
      // Deallocate the old storage
      delete[] m_storage;
      // Update member variables
      m_storage = new_storage;
      m_capacity = m_end;
    }
  }

  void assign(size_type count_, const_reference value_) {
    // Resize the vector to the specified count
    resize(count_);
    // Fill the vector with the given value
    for (size_type i = 0; i < m_end; ++i) {
      m_storage[i] = value_;
    }
  }

  void assign(const std::initializer_list<T> &ilist) {
    // Resize the vector to the size of the initializer list
    resize(ilist.size());
    // Copy the elements from the initializer list to the vector
    size_type i = 0;
    for (const auto &value : ilist) {
      m_storage[i++] = value;
    }
  }

  template <typename InputItr>
  void assign(InputItr first, InputItr last) {
    // Calculate the number of elements in the range
    size_type count = std::distance(first, last);
    // Resize the vector to the specified count
    resize(count);
    // Copy the elements from the range to the vector
    size_type i = 0;
    for (InputItr it = first; it != last; ++it) {
      m_storage[i++] = *it;
    }
  }


  iterator erase(iterator first, iterator last) {
    // Calculate the number of elements to be removed
    difference_type numElementsToRemove = std::distance(first, last);
    // Move the elements after the range to be removed
    iterator newEnd = std::move(last, end(), first);
    // Update the size of the vector
    m_end -= numElementsToRemove;
    // Destroy the remaining elements (optional, depending on the element type)
    // Return an iterator pointing to the element that follows the last removed element
    return newEnd;
  }

iterator erase(const_iterator first, const_iterator last) {
  // Calculate the number of elements to be removed
  difference_type numElementsToRemove = std::distance(first, last);
  // Convert const iterators to non-const iterators
  iterator nonConstFirst = begin() + std::distance(cbegin(), first);
  iterator nonConstLast = nonConstFirst + numElementsToRemove;
  // Move the elements after the range to be removed
  iterator newEnd = std::move(nonConstLast, end(), nonConstFirst);
  // Update the size of the vector
  m_end -= numElementsToRemove;
  // Destroy the remaining elements (optional, depending on the element type)

  // Return an iterator pointing to the element that follows the last removed element
  return newEnd;
}


  iterator erase(const_iterator pos) {
    // Convert const iterator to non-const iterator
    iterator nonConstPos = begin() + std::distance(cbegin(), pos);
    // Move the elements after the erased element
    iterator newEnd = std::move(nonConstPos + 1, end(), nonConstPos);
    // Destroy the last element (optional, depending on the element type)
    // Update the size of the vector
    m_end--;
    // Return an iterator pointing to the element that follows the erased element
    return newEnd;
  }

  iterator erase(iterator pos) {
    // Move the elements after the erased element
    iterator newEnd = std::move(pos + 1, end(), pos);
    // Destroy the last element (optional, depending on the element type)
    // Update the size of the vector
    m_end--;
    // Return an iterator pointing to the element that follows the erased element
    return newEnd;
  }


  // [V] Element access
  const_reference back() const {
    assert(m_end > 0);
    return m_storage[m_end - 1];
  }

  const_reference front() const {
    assert(m_end > 0);
    return m_storage[0];
  }

  reference back() {
    assert(m_end > 0);
    return m_storage[m_end - 1];
  }
  reference front() {
    assert(m_end > 0);
    return m_storage[0];
  }
  const_reference operator[](size_type idx) const {
    assert(idx < m_end);
    return m_storage[idx];
  }

  reference operator[](size_type idx) {
    assert(idx < m_end);
    return m_storage[idx];
  }

  const_reference at(size_type idx) const {
    if (idx >= m_end) {
      throw std::out_of_range("Index out of range");
    }
    return m_storage[idx];
  }

  reference at(size_type idx) {
    if (idx >= m_end) {
      throw std::out_of_range("Index out of range");
    }
    return m_storage[idx];
  }

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
    // enable ADLResize
resize
Resize
resize
Resize
resize
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
  template <typename T>
  bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    for (typename vector<T>::size_type i = 0; i < lhs.size(); ++i) {
      if (lhs[i] != rhs[i]) {
        return false;
      }
    }
    return true;
  }

  template <typename T>
  bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs == rhs);
  }


} // namespace sc.

#endif
