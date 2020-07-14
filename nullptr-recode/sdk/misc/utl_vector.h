#pragma once

#include <cstring>
#include "utl_memory.h"

template< class T, class A = c_utl_memory<T> >
class c_utl_vector {
    typedef A c_allocator;
public:
    typedef T elem_type_t;

    // constructor, destructor
    c_utl_vector(int growSize = 0, int initSize = 0);
    c_utl_vector(T* pMemory, int allocationCount, int numElements = 0);
    ~c_utl_vector();

    // Copy the array.
    c_utl_vector<T, A>& operator=(const c_utl_vector<T, A> &other);

    // element access
    T& operator[](int i);
    const T& operator[](int i) const;
    T& element(int i);
    const T& element(int i) const;
    T& head();
    const T& head() const;
    T& tail();
    const T& tail() const;

    // Gets the base address (can change when adding elements!)
    T* base() { return m_Memory.base(); }
    const T* base() const { return m_Memory.base(); }
    // Returns the number of elements in the vector
    int count() const;
    // Is element index valid?
    bool is_valid_index(int i) const;
    static int invalid_index();
    // Adds an element, uses default constructor
    int add_to_head();
    int add_to_tail();
    int insert_before(int elem);
    int insert_after(int elem);
    // Adds an element, uses copy constructor
    int add_to_head(const T& src);
    int add_to_tail(const T& src);
    int insert_before(int elem, const T& src);
    int insert_after(int elem, const T& src);
    // Adds multiple elements, uses default constructor
    int add_multiple_to_head(int num);
    int add_multiple_to_tail(int num);
    int add_multiple_to_tail(int num, const T *pToCopy);
    int insert_multiple_before(int elem, int num);
    int insert_multiple_before(int elem, int num, const T *pToCopy);
    int insert_multiple_after(int elem, int num);
    // Calls remove_all() then add_multiple_to_tail.
    void set_size(int size);
    void set_count(int count);
    void set_count_non_destructively(int count); //sets count by adding or removing elements to tail TODO: This should probably be the default behavior for set_count
    void copy_array(const T *pArray, int size); //Calls set_size and copies each element.
                                               // Fast swap
    void swap(c_utl_vector< T, A > &vec);
    // Add the specified array to the tail.
    int add_vector_to_tail(c_utl_vector<T, A> const &src);
    // Finds an element (element needs operator== defined)
    int get_offset(const T& src) const;
    void fill_with_value(const T& src);
    bool has_element(const T& src) const;
    // Makes sure we have enough memory allocated to store a requested # of elements
    void ensure_capacity(int num);
    // Makes sure we have at least this many elements
    void ensure_count(int num);
    // element removal
    void fast_remove(int elem);    // doesn't preserve order
    void remove(int elem);        // preserves order, shifts elements
    bool find_and_remove(const T& src);    // removes first occurrence of src, preserves order, shifts elements
    bool find_and_fast_remove(const T& src);    // removes first occurrence of src, doesn't preserve order
    void remove_multiple(int elem, int num);    // preserves order, shifts elements
    void remove_multiple_from_head(int num); // removes num elements from tail
    void remove_multiple_from_tail(int num); // removes num elements from tail
    void remove_all();                // doesn't deallocate memory
    void purge(); // Memory deallocation
                  // Purges the list and calls delete on each element in it.
    void purge_and_delete_elements();
    // Compacts the vector to the number of elements actually in use 
    void compact();
    // Set the size by which it grows when it needs to allocate more memory.
    void set_grow_size(int size) { m_Memory.set_grow_size(size); }
    int num_allocated() const;    // Only use this if you really know what you're doing!
    void sort(int(__cdecl *pfnCompare)(const T *, const T *));

protected:
    // Can't copy this unless we explicitly do it!
    c_utl_vector(c_utl_vector const& vec) { assert(0); }

    // Grows the vector
    void grow_vector(int num = 1);

    // Shifts elements....
    void shift_elements_right(int elem, int num = 1);
    void shift_elements_left(int elem, int num = 1);

public:
    c_allocator m_Memory;
    int m_Size;

    // For easier access to the elements through the debugger
    // it's in release builds so this can be used in libraries correctly
    T *m_pElements;

    void ResetDbgInfo() {
        m_pElements = base();
    }
};


//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template< typename T, class A >
c_utl_vector<T, A>::c_utl_vector(int growSize, int initSize) :
    m_Memory(growSize, initSize), m_Size(0) {
    ResetDbgInfo();
}

template< typename T, class A >
c_utl_vector<T, A>::c_utl_vector(T* pMemory, int allocationCount, int numElements) :
    m_Memory(pMemory, allocationCount), m_Size(numElements) {
    ResetDbgInfo();
}

template< typename T, class A >
c_utl_vector<T, A>::~c_utl_vector() {
    purge();
}

template< typename T, class A >
c_utl_vector<T, A>& c_utl_vector<T, A>::operator=(const c_utl_vector<T, A> &other) {
    int nCount = other.count();
    set_size(nCount);
    for(int i = 0; i < nCount; i++) {
        (*this)[i] = other[i];
    }
    return *this;
}


//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template< typename T, class A >
T& c_utl_vector<T, A>::operator[](int i) {
    assert(i < m_Size);
    return m_Memory[i];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::operator[](int i) const {
    assert(i < m_Size);
    return m_Memory[i];
}

template< typename T, class A >
T& c_utl_vector<T, A>::element(int i) {
    assert(i < m_Size);
    return m_Memory[i];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::element(int i) const {
    assert(i < m_Size);
    return m_Memory[i];
}

template< typename T, class A >
T& c_utl_vector<T, A>::head() {
    assert(m_Size > 0);
    return m_Memory[0];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::head() const {
    assert(m_Size > 0);
    return m_Memory[0];
}

template< typename T, class A >
T& c_utl_vector<T, A>::tail() {
    assert(m_Size > 0);
    return m_Memory[m_Size - 1];
}

template< typename T, class A >
const T& c_utl_vector<T, A>::tail() const {
    assert(m_Size > 0);
    return m_Memory[m_Size - 1];
}


//-----------------------------------------------------------------------------
// count
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::count() const {
    return m_Size;
}


//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template< typename T, class A >
bool c_utl_vector<T, A>::is_valid_index(int i) const {
    return (i >= 0) && (i < m_Size);
}


//-----------------------------------------------------------------------------
// Returns in invalid index
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::invalid_index() {
    return -1;
}


//-----------------------------------------------------------------------------
// Grows the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::grow_vector(int num) {
    if(m_Size + num > m_Memory.num_allocated()) {
        m_Memory.grow(m_Size + num - m_Memory.num_allocated());
    }

    m_Size += num;
    ResetDbgInfo();
}


//-----------------------------------------------------------------------------
// Sorts the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::sort(int(__cdecl *pfnCompare)(const T *, const T *)) {
    typedef int(__cdecl *QSortCompareFunc_t)(const void *, const void *);
    if(count() <= 1)
        return;

    if(base()) {
        qsort(base(), count(), sizeof(T), (QSortCompareFunc_t)(pfnCompare));
    } else {
        assert(0);
        // this path is untested
        // if you want to sort vectors that use a non-sequential memory allocator,
        // you'll probably want to patch in a quicksort algorithm here
        // I just threw in this bubble sort to have something just in case...

        for(int i = m_Size - 1; i >= 0; --i) {
            for(int j = 1; j <= i; ++j) {
                if(pfnCompare(&element(j - 1), &element(j)) < 0) {
                    V_swap(element(j - 1), element(j));
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Makes sure we have enough memory allocated to store a requested # of elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::ensure_capacity(int num) {
    MEM_ALLOC_CREDIT_CLASS();
    m_Memory.ensure_capacity(num);
    ResetDbgInfo();
}


//-----------------------------------------------------------------------------
// Makes sure we have at least this many elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::ensure_count(int num) {
    if(count() < num) {
        add_multiple_to_tail(num - count());
    }
}


//-----------------------------------------------------------------------------
// Shifts elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::shift_elements_right(int elem, int num) {
    assert(is_valid_index(elem) || (m_Size == 0) || (num == 0));
    int numToMove = m_Size - elem - num;
    if((numToMove > 0) && (num > 0))
        memmove(&element(elem + num), &element(elem), numToMove * sizeof(T));
}

template< typename T, class A >
void c_utl_vector<T, A>::shift_elements_left(int elem, int num) {
    assert(is_valid_index(elem) || (m_Size == 0) || (num == 0));
    int numToMove = m_Size - elem - num;
    if((numToMove > 0) && (num > 0)) {
        memmove(&element(elem), &element(elem + num), numToMove * sizeof(T));

#ifdef _DEBUG
        memset(&element(m_Size - num), 0xDD, num * sizeof(T));
#endif
    }
}


//-----------------------------------------------------------------------------
// Adds an element, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_to_head() {
    return insert_before(0);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_to_tail() {
    return insert_before(m_Size);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_after(int elem) {
    return insert_before(elem + 1);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_before(int elem) {
    // Can insert at the end
    assert((elem == count()) || is_valid_index(elem));

    grow_vector();
    shift_elements_right(elem);
    Construct(&element(elem));
    return elem;
}


//-----------------------------------------------------------------------------
// Adds an element, uses copy constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_to_head(const T& src) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
    return insert_before(0, src);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_to_tail(const T& src) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
    return insert_before(m_Size, src);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_after(int elem, const T& src) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));
    return insert_before(elem + 1, src);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_before(int elem, const T& src) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || (&src < base()) || (&src >= (base() + count())));

    // Can insert at the end
    assert((elem == count()) || is_valid_index(elem));

    grow_vector();
    shift_elements_right(elem);
    CopyConstruct(&element(elem), src);
    return elem;
}


//-----------------------------------------------------------------------------
// Adds multiple elements, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_head(int num) {
    return insert_multiple_before(0, num);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_tail(int num) {
    return insert_multiple_before(m_Size, num);
}

template< typename T, class A >
int c_utl_vector<T, A>::add_multiple_to_tail(int num, const T *pToCopy) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || !pToCopy || (pToCopy + num <= base()) || (pToCopy >= (base() + count())));

    return insert_multiple_before(m_Size, num, pToCopy);
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_after(int elem, int num) {
    return insert_multiple_before(elem + 1, num);
}


template< typename T, class A >
void c_utl_vector<T, A>::set_count(int count) {
    remove_all();
    add_multiple_to_tail(count);
}

template< typename T, class A >
void c_utl_vector<T, A>::set_size(int size) {
    set_count(size);
}

template< typename T, class A >
void c_utl_vector<T, A>::set_count_non_destructively(int count) {
    int delta = count - m_Size;
    if(delta > 0) add_multiple_to_tail(delta);
    else if(delta < 0) remove_multiple_from_tail(-delta);
}

template< typename T, class A >
void c_utl_vector<T, A>::copy_array(const T *pArray, int size) {
    // Can't insert something that's in the list... reallocation may hose us
    assert((base() == NULL) || !pArray || (base() >= (pArray + size)) || (pArray >= (base() + count())));

    set_size(size);
    for(int i = 0; i < size; i++) {
        (*this)[i] = pArray[i];
    }
}

template< typename T, class A >
void c_utl_vector<T, A>::swap(c_utl_vector< T, A > &vec) {
    m_Memory.swap(vec.m_Memory);
    V_swap(m_Size, vec.m_Size);
#ifndef _X360
    V_swap(m_pElements, vec.m_pElements);
#endif
}

template< typename T, class A >
int c_utl_vector<T, A>::add_vector_to_tail(c_utl_vector const &src) {
    assert(&src != this);

    int base = count();

    // Make space.
    int nSrcCount = src.count();
    ensure_capacity(base + nSrcCount);

    // Copy the elements.	
    m_Size += nSrcCount;
    for(int i = 0; i < nSrcCount; i++) {
        CopyConstruct(&element(base + i), src[i]);
    }
    return base;
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_before(int elem, int num) {
    if(num == 0)
        return elem;

    // Can insert at the end
    assert((elem == count()) || is_valid_index(elem));

    grow_vector(num);
    shift_elements_right(elem, num);

    // Invoke default constructors
    for(int i = 0; i < num; ++i) {
        Construct(&element(elem + i));
    }

    return elem;
}

template< typename T, class A >
int c_utl_vector<T, A>::insert_multiple_before(int elem, int num, const T *pToInsert) {
    if(num == 0)
        return elem;

    // Can insert at the end
    assert((elem == count()) || is_valid_index(elem));

    grow_vector(num);
    shift_elements_right(elem, num);

    // Invoke default constructors
    if(!pToInsert) {
        for(int i = 0; i < num; ++i) {
            Construct(&element(elem + i));
        }
    } else {
        for(int i = 0; i < num; i++) {
            CopyConstruct(&element(elem + i), pToInsert[i]);
        }
    }

    return elem;
}


//-----------------------------------------------------------------------------
// Finds an element (element needs operator== defined)
//-----------------------------------------------------------------------------
template< typename T, class A >
int c_utl_vector<T, A>::get_offset(const T& src) const {
    for(int i = 0; i < count(); ++i) {
        if(element(i) == src)
            return i;
    }
    return -1;
}

template< typename T, class A >
void c_utl_vector<T, A>::fill_with_value(const T& src) {
    for(int i = 0; i < count(); i++) {
        element(i) = src;
    }
}

template< typename T, class A >
bool c_utl_vector<T, A>::has_element(const T& src) const {
    return (get_offset(src) >= 0);
}


//-----------------------------------------------------------------------------
// element removal
//-----------------------------------------------------------------------------
template< typename T, class A >
void c_utl_vector<T, A>::fast_remove(int elem) {
    assert(is_valid_index(elem));

    Destruct(&element(elem));
    if(m_Size > 0) {
        if(elem != m_Size - 1)
            memcpy(&element(elem), &element(m_Size - 1), sizeof(T));
        --m_Size;
    }
}

template< typename T, class A >
void c_utl_vector<T, A>::remove(int elem) {
    Destruct(&element(elem));
    shift_elements_left(elem);
    --m_Size;
}

template< typename T, class A >
bool c_utl_vector<T, A>::find_and_remove(const T& src) {
    int elem = get_offset(src);
    if(elem != -1) {
        remove(elem);
        return true;
    }
    return false;
}

template< typename T, class A >
bool c_utl_vector<T, A>::find_and_fast_remove(const T& src) {
    int elem = get_offset(src);
    if(elem != -1) {
        fast_remove(elem);
        return true;
    }
    return false;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple(int elem, int num) {
    assert(elem >= 0);
    assert(elem + num <= count());

    for(int i = elem + num; --i >= elem; )
        Destruct(&element(i));

    shift_elements_left(elem, num);
    m_Size -= num;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple_from_head(int num) {
    assert(num <= count());

    for(int i = num; --i >= 0; )
        Destruct(&element(i));

    shift_elements_left(0, num);
    m_Size -= num;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_multiple_from_tail(int num) {
    assert(num <= count());

    for(int i = m_Size - num; i < m_Size; i++)
        Destruct(&element(i));

    m_Size -= num;
}

template< typename T, class A >
void c_utl_vector<T, A>::remove_all() {
    for(int i = m_Size; --i >= 0; ) {
        Destruct(&element(i));
    }

    m_Size = 0;
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------

template< typename T, class A >
void c_utl_vector<T, A>::purge() {
    remove_all();
    m_Memory.purge();
    ResetDbgInfo();
}


template< typename T, class A >
void c_utl_vector<T, A>::purge_and_delete_elements() {
    for(int i = 0; i < m_Size; i++) {
        delete element(i);
    }
    purge();
}

template< typename T, class A >
void c_utl_vector<T, A>::compact() {
    m_Memory.purge(m_Size);
}

template< typename T, class A >
int c_utl_vector<T, A>::num_allocated() const {
    return m_Memory.num_allocated();
}


//-----------------------------------------------------------------------------
// Data and memory validation
//-----------------------------------------------------------------------------
#ifdef DBGFLAG_VALIDATE
template< typename T, class A >
void c_utl_vector<T, A>::Validate(CValidator &validator, char *pchName) {
    validator.Push(typeid(*this).name(), this, pchName);

    m_Memory.Validate(validator, "m_Memory");

    validator.Pop();
}
#endif // DBGFLAG_VALIDATE

// A vector class for storing pointers, so that the elements pointed to by the pointers are deleted
// on exit.
template<class T> class c_utl_vector_auto_purge : public c_utl_vector< T, c_utl_memory< T, int> > {
public:
    ~c_utl_vector_auto_purge(void) {
        this->purge_and_delete_elements();
    }
};

// easy string list class with dynamically allocated strings. For use with V_SplitString, etc.
// Frees the dynamic strings in destructor.
class c_utl_string_list : public c_utl_vector_auto_purge< char *> {
public:
    void CopyAndAddToTail(char const *pString) {
        char *pNewStr = new char[1 + strlen(pString)];
        strcpy_s(pNewStr, 1 + strlen(pString), pString);
        add_to_tail(pNewStr);
    }

    static int __cdecl SortFunc(char * const * sz1, char * const * sz2) {
        return strcmp(*sz1, *sz2);
    }

};