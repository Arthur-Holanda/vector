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


  /**
   * \brief Iterator constructor.
   * 
   * Creates a new iterator and assign to nullptr.
   * 
   * \param pt The pointer.
   */
  MyForwardIterator(pointer pt = nullptr) : m_ptr(pt) { /* empty */
  }

  /**
   * \brief Overloads the = operator.
   * 
   * Specifies the behavior of the = operator
   * when dealing with an iterator. It is going to assign the atributes of
   * a iterator to another one.
   * 
   * \param iterator The iterator that will be assigned.
   * 
   * \return The iterator where the assignment happened.
   */
  iterator &operator=(const iterator &) = default;

  /**
   * \brief Copy constructor
   * 
   * Initializes iterator by copying another one.
   * 
   * \param iterator The iterator that will be copied.
   */
  MyForwardIterator(const iterator &) = default;
  
  /**
   * \brief Overloads the ++ operator.
   * 
   * Specifies the behavior of the ++ operator
   * when dealing with an iterator. It is going to increment the iterator.
   * 
   * \return The incremented iterator.
   */
  iterator operator++(void) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    ++m_ptr; // Increment the raw pointer to the next element

    return *this; // Return the updated iterator
  }

  /**
   * \brief Overloads the ++ operator.
   * 
   * Specifies the behavior of the ++ operator
   * when dealing with an iterator. It is going to post increment the iterator.
   * 
   * \return The post incremented iterator.
   */
  iterator operator++(int) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    iterator original(*this); // Create a copy of the original iterator

    ++m_ptr; // Increment the raw pointer to the next element

    return original; // Return the original iterator
  }

  /**
   * \brief Overloads the -- operator.
   * 
   * Specifies the behavior of the -- operator
   * when dealing with an iterator. It is going to post the iterator.
   * 
   * \return The decrement iterator.
   */
  iterator operator--(void) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    --m_ptr; // Decrement the raw pointer to the previous element

    return *this; // Return the updated iterator
  }

  /**
   * \brief Overloads the -- operator.
   * 
   * Specifies the behavior of the -- operator
   * when dealing with an iterator. It is going to post decrement the iterator.
   * 
   * \return The post decrement iterator.
   */
  iterator operator--(int) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    iterator original(*this); // Create a copy of the original iterator

    --m_ptr; // Decrement the raw pointer to the previous element

    return original; // Return the original iterator
  }

  /**
   * \brief Overloads the + operator.
   * 
   * Specifies the behavior of the + operator
   * when dealing with an iterator. It is going to add a number to an iterator.
   * 
   * \param  offset The number to be added.
   * \param it The iterator.
   * 
   * \return The updated iterator.
   */
  friend iterator operator+(difference_type offset, iterator it) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr += offset; // Advance the raw pointer by the given offset

    return it; // Return the updated iterator
  }

  /**
   * \brief Overloads the + operator.
   * 
   * Specifies the behavior of the + operator
   * when dealing with an iterator. It is going to add an iterator to a number.
   * 
   * \param it The iterator.
   * \param  offset The number to be added.
   * 
   * \return The updated iterator.
   */
  friend iterator operator+(iterator it, difference_type offset) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr += offset; // Advance the raw pointer by the given offset

    return it; // Return the updated iterator
  }

  /**
   * \brief Overloads the - operator.
   * 
   * Specifies the behavior of the - operator
   * when dealing with an iterator. It is going to subtract an iterator to a number.
   * 
   * \param it The iterator.
   * \param  offset The number to be subtracted.
   * 
   * \return The updated iterator.
   */
  friend iterator operator-(iterator it, difference_type offset) {
    assert(it.m_ptr != nullptr); // Check if the iterator is valid

    it.m_ptr -= offset; // Move the raw pointer backward by the given offset

    return it; // Return the updated iterator
  }

  /**
   * \brief Overloads the += operator.
   * 
   * Specifies the behavior of the += operator
   * when dealing with an iterator. It is going to increment the iterator
   * with the specified offset.
   * 
   * \param  offset The number to be incremented.
   * 
   * \return The updated iterator.
   */
  iterator &operator+=(difference_type offset) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    m_ptr += offset; // Advance the raw pointer by the given offset

    return *this; // Return the updated iterator
  }

  /**
   * \brief Overloads the -= operator.
   * 
   * Specifies the behavior of the -= operator
   * when dealing with an iterator. It is going to decrement the iterator
   * with the specified offset.
   * 
   * \param  offset The number to be decrement.
   * 
   * \return The updated iterator.
   */
  iterator &operator-=(difference_type offset) {
    assert(m_ptr != nullptr); // Check if the iterator is valid

    m_ptr -= offset; // Move the raw pointer backward by the given offset

    return *this; // Return the updated iterator
  }

  /**
   * \brief Overloads the - operator.
   * 
   * Specifies the behavior of the - operator
   * when dealing with an iterator. It is going to calculate
   * the difference between two operators.
   * 
   * \param rhs The iterator that will be subtract the left side iterator.
   * 
   * \return The updated iterator.
   */
  difference_type operator-(const iterator& rhs_) const {
    assert(m_ptr != nullptr && rhs_.m_ptr != nullptr); // Check if the iterators are valid

    return m_ptr - rhs_.m_ptr; // Calculate the difference between the raw pointers
  }

  /**
   * \brief Overloads the * operator.
   * 
   * Specifies the behavior of the * operator
   * when dealing with an iterator. It is going to dereference the pointer.
   * 
   * \return The content of the iterator.
   */
  reference operator*(void) const {
    assert(m_ptr != nullptr);
    return *m_ptr;
  }

  /**
   * \brief Overloads the == operator.
   * 
   * Specifies the behavior of the == operator
   * when dealing with an iterator. It is going return whether two iterator are equal.
   * It is going to be done by comparing the two pointers.
   * 
   * \param rhs The iterator of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
  bool operator==(const iterator& rhs_) const {
    return m_ptr == rhs_.m_ptr; // Compare the raw pointers of the iterators
  }

  /**
   * \brief Overloads the != operator.
   * 
   * Specifies the behavior of the != operator
   * when dealing with an iterator. It is going return whether two iterator are different.
   * It is going to be done by comparing the two pointers.
   * 
   * \param rhs The iterator of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
  bool operator!=(const iterator& rhs_) const {
    return !(*this == rhs_); // Utilize the already defined operator== to implement operator!=
  }

  /**
   * \brief Overloads the < operator.
   * 
   * Specifies the behavior of the < operator
   * when dealing with an iterator. It is going to return whether the
   * left side iterator is less than the right side iterator.
   * 
   * \param ita The iterator of the left side of the comparision.
   * \param itb The iterator of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
  friend bool operator<(const iterator& ita, const iterator& itb) {
    return ita.m_ptr < itb.m_ptr;
  }

  /**
   * \brief Overloads the > operator.
   * 
   * Specifies the behavior of the > operator
   * when dealing with an iterator. It is going to return whether the
   * left side iterator is greater than the right side iterator.
   * 
   * \param ita The iterator of the left side of the comparision.
   * \param itb The iterator of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
  friend bool operator>(const iterator& ita, const iterator& itb) {
    return itb < ita; // Utilize the already defined operator< to implement operator>
  }

  /// Overloaded `->` operator.
  pointer operator->(void) const {
    assert(m_ptr != nullptr);
    return m_ptr;
  }

  friend bool operator>=(const iterator& ita, const iterator& itb) {
    return !(ita < itb); // Utilize the already defined operator< to implement operator>=
  }

  friend bool operator<=(const iterator& ita, const iterator& itb) {
    return !(ita > itb); // Utilize the already defined operator> to implement operator<=
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

  /**
   * \brief Function to clear a vector
   * 
   * This function deallocate the memory of the vector, set its capacity and 
   * size to zero, and set the storage pounter to null .
   * 
   */
  void clear(void) {
    //delete[] m_storage;  
    //m_storage = nullptr; 
    //m_capacity = 0;     
    m_end = 0;           
  }

  void push_front(const_reference); //Not TODO

  /**
   * \brief This function inserts a value at the end of the vector.
   * 
   * The function receives a value, assigns it to the vector last element,
   * and increases the size of the vector.
   * 
   * \param value The data that shall be inserted in the vector.
   * 
   */
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

  /**
   * \brief This function removes the last value if the vector.
   * 
   * The function will verify if the vector has any element, and then it will
   * make the pointer to the last element decrement, in such a way that the last 
   * element will not be part of the vector anymore.
   * 
   */
  void pop_back(void) {
    if (m_end > 0) {
      --m_end;
    }
  }

  void pop_front(void); //not TODO

  /**
   * \brief This function inserts a value at a specific position.
   * 
   * The function receives a value and a position, and inserts the element
   * at the desired position by shifting the elements so the elements has 
   * its own space.
   * 
   * \param pos_ The position where the insertion must be done.
   * \param value The data that shall be inserted in the vector.
   * 
   * \return The iterator pointing to the position of the insertion
   */
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

  /**
   * \brief This function inserts a value at a specific position.
   * 
   * The function receives a value and a position, and inserts the element
   * at the desired position by shifting the elements so the elements has 
   * its own space.
   * 
   * \param pos_ The position where the insertion must be done.
   * \param value The data that shall be inserted in the vector.
   * 
   * \return The const iterator pointing to the position of the insertion
   */
  iterator insert(const_iterator pos_, const_reference value_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, value_);
  }

  /**
   * \brief This function inserts a list of values at a specific position.
   * 
   * The function receives a range and a position, and inserts the elements stored
   * between that range at the desired position by shifting the elements so the elements has 
   * its own space.
   * 
   * \param pos_ The iterator pointing to the position where the insertion must be done.
   * \param first The pointer to the first element to be inserted.
   * \param last The pointer to the last element to be inserted.
   * 
   * \return The iterator pointing to the position of the insertion
   */
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

  /**
   * \brief This function inserts a list of values at a specific position.
   * 
   * The function receives a range and a position, and inserts the elements stored
   * between that range at the desired position by shifting the elements so the elements has 
   * its own space.
   * 
   * \param pos_ The constiterator pointing to the position where the insertion must be done.
   * \param first The pointer to the first element to be inserted.
   * \param last The pointer to the last element to be inserted.
   * 
   * \return The const iterator pointing to the position of the insertion
   */
  template <typename InputItr>
  iterator insert(const_iterator pos_, InputItr first_, InputItr last_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, first_, last_);
  }

  /**
   * \brief This function inserts an initialiazer list of values at a specific position.
   * 
   * The function receives a initialize list and inserts its elements at the specified position.
   * 
   * \param pos_ The titerator pointing to the position where the insertion must be done.
   * \param ilist The list whose elements are to be inserted.
   * 
   * \return The iterator pointing to the the end of the list after the insertion
   */
  iterator insert(iterator pos_, const std::initializer_list<value_type> &ilist_) {
    return insert(pos_, ilist_.begin(), ilist_.end());
  }

  /**
   * \brief This function inserts an initialiazer list of values at a specific position.
   * 
   * The function receives a initialize list and inserts its elements at the specified position.
   * 
   * \param pos_ The const iterator pointing to the position where the insertion must be done.
   * \param ilist The const list whose elements are to be inserted.
   * 
   * \return The const iterator pointing to the the end of the list after the insertion
   */
  iterator insert(const_iterator pos_,
                  const std::initializer_list<value_type> &ilist_) {
    // Convert the const_iterator to an iterator
    iterator pos{const_cast<iterator>(pos_)};

    // Delegate the insert operation to the iterator-based insert function
    return insert(pos, ilist_.begin(), ilist_.end());
  }

  /**
   * \brief This function changes the capacity of the vector
   * 
   * The function receives the value of the new capacity, creates a new vector
   * with this capacity, deallocates the memory of the old storage, and assigns the curente
   * vector with the new vector. 
   * 
   * \param new_capacity The capacity that the vector must have.
   * 
   */
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

  /**
   * \brief This function reduces the vector capacity untill it is equal to the ocuppied size.
   * 
   * The function verifies if the capacity of the vector is grater than the current ocuppied size,
   * and decreases the capacity. 
   * 
   */
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

  /**
   * \brief This function assings n elements of the vector with a specified a value
   * 
   * The function receives the value to be assigned and the quantity of elements that
   * will receive this value. 
   * 
   * \param count_ The number of elements that will receive the value.
   * \param value_ The value to be assigned.
   */
  void assign(size_type count_, const_reference value_) {
      // Check if the vector needs to be resized
      if (count_ > m_capacity) {
          // Allocate new memory for the vector
          T* newStorage = new T[count_];
          // Copy the elements from the old storage to the new storage
          for (size_type i = 0; i < m_end; ++i) {
              newStorage[i] = m_storage[i];
          }
          // Deallocate the old storage
          delete[] m_storage;
          // Update the storage pointer and capacity
          m_storage = newStorage;
          m_capacity = count_;
      }
      // Update the size of the vector
      m_end = count_;
      // Fill the vector with the given value
      for (size_type i = 0; i < m_end; ++i) {
          m_storage[i] = value_;
      }
  }


  /**
   * \brief This function assings n elements of the vector with the elements of a list
   * 
   * The function receives the list whose elements are to be assigned . 
   * 
   * \param ilist The list with the elements to be assigned.
   */
  void assign(const std::initializer_list<T>& ilist) {
      // Check if the initializer list size exceeds the current capacity
      if (ilist.size() > m_capacity) {
          // Allocate new memory for the vector
          T* newStorage = new T[ilist.size()];
          // Deallocate the old storage
          delete[] m_storage;
          // Update the storage pointer and capacity
          m_storage = newStorage;
          m_capacity = ilist.size();
      }
      // Update the size of the vector
      m_end = ilist.size();
      // Copy the elements from the initializer list to the vector
      size_type i = 0;
      for (const auto& value : ilist) {
          m_storage[i++] = value;
      }
  }


  /**
   * \brief Template function to assign a range of elements to the vector
   * 
   * The function receives the range of elements that must be assigned to the
   * vector elements. 
   * 
   * \param first The iterator pointing to the first element to be assigned.
   * \param last The iterator pointing to the last element to be assigned.
   */
  template <typename InputItr>
  void assign(InputItr first, InputItr last) {
      // Calculate the number of elements in the range
      size_type count = std::distance(first, last);
      // Check if the range size exceeds the current capacity
      if (count > m_capacity) {
          // Allocate new memory for the vector
          T* newStorage = new T[count]; 
          // Deallocate the old storage
          delete[] m_storage;
          // Update the storage pointer and capacity
          m_storage = newStorage;
          m_capacity = count;
      }
      // Update the size of the vector
      m_end = count;
      // Copy the elements from the range to the vector
      size_type i = 0;
      for (InputItr it = first; it != last; ++it) {
          m_storage[i++] = *it;
      }
  }


  /**
   * \brief This function deletes a specific range of the vector elements.
   * 
   * The function receives the range to be deleted, calculates the distance between, the first and 
   * last elements of the range, and moves the elements of the vector in such a way that the range is removed.
   * 
   * \param first The pointer to the first element to be erased.
   * \param last The pointer to the last element to be erased.
   * 
   * \return The iterator pointing to the the end of the list after the delete
   */
  iterator erase(iterator first, iterator last) {
    // Calculate the number of elements to be removed
    auto numElementsToRemove = std::distance(first, last);
    // Move the elements after the range to be removed
    iterator newEnd = std::move(last, end(), first);
    // Update the size of the vector
    m_end -= numElementsToRemove;
    return newEnd;
  }

  /**
   * \brief This function deletes a specific range of the vector elements.
   * 
   * The function receives the range to be deleted, calculates the distance between, the first and 
   * last elements of the range, and moves the elements of the vector in such a way that the range is removed.
   * 
   * \param first The const iterator pointing to to the first element to be erased.
   * \param last The const iterator pointing to to the last element to be erased.
   * 
   * \return The const iterator pointing to the the end of the list after the delete
   */
  iterator erase(const_iterator first, const_iterator last) {
    // Calculate the number of elements to be removed
    auto numElementsToRemove = std::distance(first, last);
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

  /**
   * \brief This function deletes a specific of element of the vector.
   * 
   * The function receives the element that is located at the received position.
   * 
   * \param pos The const iterator pointing to position where the target is located.
   * 
   * \return The const iterator pointing to the the end of the list after the delete
   */
  iterator erase(const_iterator pos) {
    // Convert const iterator to non-const iterator
    iterator nonConstPos = begin() + std::distance(cbegin(), pos);
    // Move the elements after the erased element
    iterator newEnd = std::move(nonConstPos + 1, end(), nonConstPos);
    // Update the size of the vector
    m_end--;
    // Return an iterator pointing to the element that follows the erased element
    return newEnd;
  }

  /**
   * \brief This function deletes a specific of element of the vector.
   * 
   * The function receives the element that is located at the received position.
   * 
   * \param pos The iterator pointing to position where the target is located.
   * 
   * \return The iterator pointing to the the end of the list after the delete
   */
  iterator erase(iterator pos) {
    // Move the elements after the erased element
    iterator newEnd = std::move(pos + 1, end(), pos);
    // Update the size of the vector
    m_end--;
    // Return an iterator pointing to the element that follows the erased element
    return newEnd;
  }

  // [V] Element access

  /**
   * \brief This function returns the last element of the vector.
   * 
   * The function returns a const reference to the last element of the vector
   * by accessing the position right before the pointer to the end.
   * 
   * \return The const reference to the last element.
   */
  const_reference back() const {
    assert(m_end > 0);
    return m_storage[m_end - 1];
  }

  /**
   * \brief This function returns the first element of the vector.
   * 
   * The function returns a const reference to the first element of the vector
   * by accessing the position of index zero.
   * 
   * \return The const reference to the first element.
   */
  const_reference front() const {
    assert(m_end > 0);
    return m_storage[0];
  }

  /**
   * \brief This function returns the last element of the vector.
   * 
   * The function returns a reference to the last element of the vector
   * by accessing the position right before the pointer to the end.
   * 
   * \return The reference to the last element.
   */
  reference back() {
    assert(m_end > 0);
    return m_storage[m_end - 1];
  }

  /**
   * \brief This function returns the first element of the vector.
   * 
   * The function returns a reference to the first element of the vector
   * by accessing the position of index zero.
   * 
   * \return The reference to the first element.
   */
  reference front() {
    assert(m_end > 0);
    return m_storage[0];
  }

  /**
   * \brief Overloads the [] operator.
   * 
   * Specifies the behavior of the [] operator
   * when dealing with a vector. It is going return the value 
   * stored at the specified index.
   * 
   * \param idx The index to be searched.
   * 
   * \return The const reference to the value stored at the searched index.
   */
  const_reference operator[](size_type idx) const {
    assert(idx < m_end);
    return m_storage[idx];
  }

  /**
   * \brief Overloads the [] operator.
   * 
   * Specifies the behavior of the [] operator
   * when dealing with a vector. It is going return the value 
   * stored at the specified index.
   * 
   * \param idx The index to be searched.
   * 
   * \return The reference to the value stored at the searched index.
   */
  reference operator[](size_type idx) {
    assert(idx < m_end);
    return m_storage[idx];
  }

  /**
   * \brief Returns the value of a specific index.
   * 
   * It is going return the value stored at the specified index.
   * 
   * \param idx The index to be searched.
   * 
   * \return The const reference to the value stored at the searched index.
   */
  const_reference at(size_type idx) const {
    if (idx >= m_end) {
      throw std::out_of_range("Index out of range");
    }
    return m_storage[idx];
  }

  /**
   * \brief Returns the value of a specific index.
   * 
   * It is going return the value stored at the specified index.
   * 
   * \param idx The index to be searched.
   * 
   * \return The reference to the value stored at the searched index.
   */
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

  /**
   * \brief Overloads the == operator.
   * 
   * Specifies the behavior of the == operator
   * when dealing with a vector. It is going return whether two vector are equal.
   * It is going to be done by comparing the sizes. If the sizes are different, the functin will
   * end return false, otherwise, it will traverse the whole vector comparing it with each element
   * of the other vector.
   * 
   * \param lhs The vector of the left side of the comparision.
   * \param rhs The vector of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
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

  /**
   * \brief Overloads the != operator.
   * 
   * Specifies the behavior of the != operator
   * when dealing with a vector. It is going return whether two vector are different.
   * It is going to be done by calling the operator== function and returning the logical value of its return.
   * 
   * \param lhs The vector of the left side of the comparision.
   * \param rhs The vector of the right side of the comparision.
   * 
   * \return The logical value of the comparision.
   */
  template <typename T>
  bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
    return !(lhs == rhs);
  }


} // namespace sc.

#endif
  