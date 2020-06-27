#pragma once

#include <assert.h>
#include "platform.h"

template< class T, class I = int >
class c_utl_memory
{
public:
	// constructor, destructor
	c_utl_memory(int nGrowSize = 0, int nInitSize = 0);
	c_utl_memory(T* pMemory, int numElements);
	c_utl_memory(const T* pMemory, int numElements);
	~c_utl_memory();

	// Set the size by which the memory grows
	void Init(int nGrowSize = 0, int nInitSize = 0);

	class Iterator_t
	{
	public:
		Iterator_t(I i) : index(i) {}
		I index;

		bool operator==(const Iterator_t it) const { return index == it.index; }
		bool operator!=(const Iterator_t it) const { return index != it.index; }
	};
	Iterator_t First() const { return Iterator_t(IsIdxValid(0) ? 0 : InvalidIndex()); }
	Iterator_t Next(const Iterator_t &it) const { return Iterator_t(IsIdxValid(it.index + 1) ? it.index + 1 : InvalidIndex()); }
	I GetIndex(const Iterator_t &it) const { return it.index; }
	bool IsIdxAfter(I i, const Iterator_t &it) const { return i > it.index; }
	bool IsValidIterator(const Iterator_t &it) const { return IsIdxValid(it.index); }
	Iterator_t InvalidIterator() const { return Iterator_t(InvalidIndex()); }

	// element access
	T& operator[](I i);
	const T& operator[](I i) const;
	T& Element(I i);
	const T& Element(I i) const;

	bool IsIdxValid(I i) const;

	static const I INVALID_INDEX = (I)-1; // For use with COMPILE_TIME_ASSERT
	static I InvalidIndex() { return INVALID_INDEX; }

	T* Base();
	const T* Base() const;

	void SetExternalBuffer(T* pMemory, int numElements);
	void SetExternalBuffer(const T* pMemory, int numElements);
	void AssumeMemory(T *pMemory, int nSize);
	T* Detach();
	void *DetachMemory();

	void Swap(c_utl_memory< T, I > &mem);
	void ConvertToGrowableMemory(int nGrowSize);
	int NumAllocated() const;
	int Count() const;
	void Grow(int num = 1);
	void EnsureCapacity(int num);
	void Purge();
	void Purge(int numElements);
	bool IsExternallyAllocated() const;
	bool IsReadOnly() const;
	void SetGrowSize(int size);

protected:
	void ValidateGrowSize()
	{

	}

	enum
	{
		EXTERNAL_BUFFER_MARKER = -1,
		EXTERNAL_CONST_BUFFER_MARKER = -2,
	};

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(int nGrowSize, int nInitAllocationCount) : m_pMemory(0),
m_nAllocationCount(nInitAllocationCount), m_nGrowSize(nGrowSize)
{
	ValidateGrowSize();
	assert(nGrowSize >= 0);
	if (m_nAllocationCount) {
		m_pMemory = (T*)new unsigned char[m_nAllocationCount * sizeof(T)];
		//m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(T* pMemory, int numElements) : m_pMemory(pMemory),
m_nAllocationCount(numElements)
{
	// Special marker indicating externally supplied modifyable memory
	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< class T, class I >
c_utl_memory<T, I>::c_utl_memory(const T* pMemory, int numElements) : m_pMemory((T*)pMemory),
m_nAllocationCount(numElements)
{
	// Special marker indicating externally supplied modifyable memory
	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< class T, class I >
c_utl_memory<T, I>::~c_utl_memory()
{
	Purge();
}

template< class T, class I >
void c_utl_memory<T, I>::Init(int nGrowSize /*= 0*/, int nInitSize /*= 0*/)
{
	Purge();

	m_nGrowSize = nGrowSize;
	m_nAllocationCount = nInitSize;
	ValidateGrowSize();
	assert(nGrowSize >= 0);
	if (m_nAllocationCount) {
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}

//-----------------------------------------------------------------------------
// Fast swap
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::Swap(c_utl_memory<T, I> &mem)
{
	V_swap(m_nGrowSize, mem.m_nGrowSize);
	V_swap(m_pMemory, mem.m_pMemory);
	V_swap(m_nAllocationCount, mem.m_nAllocationCount);
}


//-----------------------------------------------------------------------------
// Switches the buffer from an external memory buffer to a reallocatable buffer
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::ConvertToGrowableMemory(int nGrowSize)
{
	if (!IsExternallyAllocated())
		return;

	m_nGrowSize = nGrowSize;
	if (m_nAllocationCount) {
		int nNumBytes = m_nAllocationCount * sizeof(T);
		T *pMemory = (T*)malloc(nNumBytes);
		memcpy(pMemory, m_pMemory, nNumBytes);
		m_pMemory = pMemory;
	}
	else {
		m_pMemory = NULL;
	}
}


//-----------------------------------------------------------------------------
// Attaches the buffer to external memory....
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::SetExternalBuffer(T* pMemory, int numElements)
{
	// Blow away any existing allocated memory
	Purge();

	m_pMemory = pMemory;
	m_nAllocationCount = numElements;

	// Indicate that we don't own the memory
	m_nGrowSize = EXTERNAL_BUFFER_MARKER;
}

template< class T, class I >
void c_utl_memory<T, I>::SetExternalBuffer(const T* pMemory, int numElements)
{
	// Blow away any existing allocated memory
	Purge();

	m_pMemory = (T*)(pMemory);
	m_nAllocationCount = numElements;

	// Indicate that we don't own the memory
	m_nGrowSize = EXTERNAL_CONST_BUFFER_MARKER;
}

template< class T, class I >
void c_utl_memory<T, I>::AssumeMemory(T* pMemory, int numElements)
{
	// Blow away any existing allocated memory
	Purge();

	// Simply take the pointer but don't mark us as external
	m_pMemory = pMemory;
	m_nAllocationCount = numElements;
}

template< class T, class I >
void *c_utl_memory<T, I>::DetachMemory()
{
	if (IsExternallyAllocated())
		return NULL;

	void *pMemory = m_pMemory;
	m_pMemory = 0;
	m_nAllocationCount = 0;
	return pMemory;
}

template< class T, class I >
T* c_utl_memory<T, I>::Detach()
{
	return (T*)DetachMemory();
}


//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template< class T, class I >
T& c_utl_memory<T, I>::operator[](I i)
{
	assert(!IsReadOnly());
	assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
const T& c_utl_memory<T, I>::operator[](I i) const
{
	assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
T& c_utl_memory<T, I>::Element(I i)
{
	assert(!IsReadOnly());
	assert(IsIdxValid(i));
	return m_pMemory[i];
}

template< class T, class I >
const T& c_utl_memory<T, I>::Element(I i) const
{
	assert(IsIdxValid(i));
	return m_pMemory[i];
}


//-----------------------------------------------------------------------------
// is the memory externally allocated?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::IsExternallyAllocated() const
{
	return (m_nGrowSize < 0);
}


//-----------------------------------------------------------------------------
// is the memory read only?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::IsReadOnly() const
{
	return (m_nGrowSize == EXTERNAL_CONST_BUFFER_MARKER);
}


template< class T, class I >
void c_utl_memory<T, I>::SetGrowSize(int nSize)
{
	assert(!IsExternallyAllocated());
	assert(nSize >= 0);
	m_nGrowSize = nSize;
	ValidateGrowSize();
}


//-----------------------------------------------------------------------------
// Gets the base address (can change when adding elements!)
//-----------------------------------------------------------------------------
template< class T, class I >
T* c_utl_memory<T, I>::Base()
{
	assert(!IsReadOnly());
	return m_pMemory;
}

template< class T, class I >
const T *c_utl_memory<T, I>::Base() const
{
	return m_pMemory;
}


//-----------------------------------------------------------------------------
// Size
//-----------------------------------------------------------------------------
template< class T, class I >
int c_utl_memory<T, I>::NumAllocated() const
{
	return m_nAllocationCount;
}

template< class T, class I >
int c_utl_memory<T, I>::Count() const
{
	return m_nAllocationCount;
}


//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template< class T, class I >
bool c_utl_memory<T, I>::IsIdxValid(I i) const
{
	// GCC warns if I is an unsigned type and we do a ">= 0" against it (since the comparison is always 0).
	// We Get the warning even if we cast inside the expression. It only goes away if we assign to another variable.
	long x = i;
	return (x >= 0) && (x < m_nAllocationCount);
}

//-----------------------------------------------------------------------------
// Grows the memory
//-----------------------------------------------------------------------------
int UtlMemory_CalcNewAllocationCount(int nAllocationCount, int nGrowSize, int nNewSize, int nBytesItem)
{
	if (nGrowSize) {
		nAllocationCount = ((1 + ((nNewSize - 1) / nGrowSize)) * nGrowSize);
	}
	else {
		if (!nAllocationCount) {
			// Compute an allocation which is at least as big as a cache line...
			nAllocationCount = (31 + nBytesItem) / nBytesItem;
		}

		while (nAllocationCount < nNewSize) {
#ifndef _X360
			nAllocationCount *= 2;
#else
			int nNewAllocationCount = (nAllocationCount * 9) / 8; // 12.5 %
			if (nNewAllocationCount > nAllocationCount)
				nAllocationCount = nNewAllocationCount;
			else
				nAllocationCount *= 2;
#endif
		}
	}

	return nAllocationCount;
}

template< class T, class I >
void c_utl_memory<T, I>::Grow(int num)
{
	assert(num > 0);

	if (IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}


	auto oldAllocationCount = m_nAllocationCount;
	// Make sure we have at least numallocated + num allocations.
	// Use the grow rules specified for this memory (in m_nGrowSize)
	int nAllocationRequested = m_nAllocationCount + num;

	int nNewAllocationCount = UtlMemory_CalcNewAllocationCount(m_nAllocationCount, m_nGrowSize, nAllocationRequested, sizeof(T));

	// if m_nAllocationRequested wraps index type I, recalculate
	if ((int)(I)nNewAllocationCount < nAllocationRequested) {
		if ((int)(I)nNewAllocationCount == 0 && (int)(I)(nNewAllocationCount - 1) >= nAllocationRequested) {
			--nNewAllocationCount; // deal w/ the common case of m_nAllocationCount == MAX_USHORT + 1
		}
		else {
			if ((int)(I)nAllocationRequested != nAllocationRequested) {
				// we've been asked to grow memory to a size s.t. the index type can't address the requested amount of memory
				assert(0);
				return;
			}
			while ((int)(I)nNewAllocationCount < nAllocationRequested) {
				nNewAllocationCount = (nNewAllocationCount + nAllocationRequested) / 2;
			}
		}
	}

	m_nAllocationCount = nNewAllocationCount;

	if (m_pMemory) {
		auto ptr = new unsigned char[m_nAllocationCount * sizeof(T)];

		memcpy(ptr, m_pMemory, oldAllocationCount * sizeof(T));
		m_pMemory = (T*)ptr;
	}
	else {
		m_pMemory = (T*)new unsigned char[m_nAllocationCount * sizeof(T)];
	}
}


//-----------------------------------------------------------------------------
// Makes sure we've got at least this much memory
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::EnsureCapacity(int num)
{
	if (m_nAllocationCount >= num)
		return;

	if (IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}
	m_nAllocationCount = num;

	if (m_pMemory) {
		m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
	}
	else {
		m_pMemory = (T*)malloc(m_nAllocationCount * sizeof(T));
	}
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------
template< class T, class I >
void c_utl_memory<T, I>::Purge()
{
	if (!IsExternallyAllocated()) {
		if (m_pMemory) {
			free((void*)m_pMemory);
			m_pMemory = 0;
		}
		m_nAllocationCount = 0;
	}
}

template< class T, class I >
void c_utl_memory<T, I>::Purge(int numElements)
{
	assert(numElements >= 0);

	if (numElements > m_nAllocationCount) {
		// Ensure this isn't a grow request in disguise.
		assert(numElements <= m_nAllocationCount);
		return;
	}

	// If we have zero elements, simply do a purge:
	if (numElements == 0) {
		Purge();
		return;
	}

	if (IsExternallyAllocated()) {
		// Can't shrink a buffer whose memory was externally allocated, fail silently like purge 
		return;
	}

	// If the number of elements is the same as the allocation count, we are done.
	if (numElements == m_nAllocationCount) {
		return;
	}


	if (!m_pMemory) {
		// Allocation count is non zero, but memory is null.
		assert(m_pMemory);
		return;
	}
	m_nAllocationCount = numElements;
	m_pMemory = (T*)realloc(m_pMemory, m_nAllocationCount * sizeof(T));
}

//-----------------------------------------------------------------------------
// The c_utl_memory class:
// A growable memory class which doubles in size by default.
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
class c_utl_memoryAligned : public c_utl_memory<T>
{
public:
	// constructor, destructor
	c_utl_memoryAligned(int nGrowSize = 0, int nInitSize = 0);
	c_utl_memoryAligned(T* pMemory, int numElements);
	c_utl_memoryAligned(const T* pMemory, int numElements);
	~c_utl_memoryAligned();

	// Attaches the buffer to external memory....
	void SetExternalBuffer(T* pMemory, int numElements);
	void SetExternalBuffer(const T* pMemory, int numElements);

	// Grows the memory, so that at least allocated + num elements are allocated
	void Grow(int num = 1);

	// Makes sure we've got at least this much memory
	void EnsureCapacity(int num);

	// Memory deallocation
	void Purge();

	// Purge all but the given number of elements (NOT IMPLEMENTED IN c_utl_memoryAligned)
	void Purge(int numElements) { assert(0); }

private:
	void *Align(const void *pAddr);
};


//-----------------------------------------------------------------------------
// Aligns a pointer
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void *c_utl_memoryAligned<T, nAlignment>::Align(const void *pAddr)
{
	size_t nAlignmentMask = nAlignment - 1;
	return (void*)(((size_t)pAddr + nAlignmentMask) & (~nAlignmentMask));
}


//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
c_utl_memoryAligned<T, nAlignment>::c_utl_memoryAligned(int nGrowSize, int nInitAllocationCount)
{
	c_utl_memory<T>::m_pMemory = 0;
	c_utl_memory<T>::m_nAllocationCount = nInitAllocationCount;
	c_utl_memory<T>::m_nGrowSize = nGrowSize;
	this->ValidateGrowSize();

	// Alignment must be a power of two
	COMPILE_TIME_ASSERT((nAlignment & (nAlignment - 1)) == 0);
	assert((nGrowSize >= 0) && (nGrowSize != c_utl_memory<T>::EXTERNAL_BUFFER_MARKER));
	if (c_utl_memory<T>::m_nAllocationCount) {
		UTLMEMORY_TRACK_ALLOC();
		MEM_ALLOC_CREDIT_CLASS();
		c_utl_memory<T>::m_pMemory = (T*)_aligned_malloc(nInitAllocationCount * sizeof(T), nAlignment);
	}
}

template< class T, int nAlignment >
c_utl_memoryAligned<T, nAlignment>::c_utl_memoryAligned(T* pMemory, int numElements)
{
	// Special marker indicating externally supplied memory
	c_utl_memory<T>::m_nGrowSize = c_utl_memory<T>::EXTERNAL_BUFFER_MARKER;

	c_utl_memory<T>::m_pMemory = (T*)Align(pMemory);
	c_utl_memory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)c_utl_memory<T>::m_pMemory) / sizeof(T);
}

template< class T, int nAlignment >
c_utl_memoryAligned<T, nAlignment>::c_utl_memoryAligned(const T* pMemory, int numElements)
{
	// Special marker indicating externally supplied memory
	c_utl_memory<T>::m_nGrowSize = c_utl_memory<T>::EXTERNAL_CONST_BUFFER_MARKER;

	c_utl_memory<T>::m_pMemory = (T*)Align(pMemory);
	c_utl_memory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)c_utl_memory<T>::m_pMemory) / sizeof(T);
}

template< class T, int nAlignment >
c_utl_memoryAligned<T, nAlignment>::~c_utl_memoryAligned()
{
	Purge();
}


//-----------------------------------------------------------------------------
// Attaches the buffer to external memory....
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memoryAligned<T, nAlignment>::SetExternalBuffer(T* pMemory, int numElements)
{
	// Blow away any existing allocated memory
	Purge();

	c_utl_memory<T>::m_pMemory = (T*)Align(pMemory);
	c_utl_memory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)c_utl_memory<T>::m_pMemory) / sizeof(T);

	// Indicate that we don't own the memory
	c_utl_memory<T>::m_nGrowSize = c_utl_memory<T>::EXTERNAL_BUFFER_MARKER;
}

template< class T, int nAlignment >
void c_utl_memoryAligned<T, nAlignment>::SetExternalBuffer(const T* pMemory, int numElements)
{
	// Blow away any existing allocated memory
	Purge();

	c_utl_memory<T>::m_pMemory = (T*)Align(pMemory);
	c_utl_memory<T>::m_nAllocationCount = ((int)(pMemory + numElements) - (int)c_utl_memory<T>::m_pMemory) / sizeof(T);

	// Indicate that we don't own the memory
	c_utl_memory<T>::m_nGrowSize = c_utl_memory<T>::EXTERNAL_CONST_BUFFER_MARKER;
}


//-----------------------------------------------------------------------------
// Grows the memory
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memoryAligned<T, nAlignment>::Grow(int num)
{
	assert(num > 0);

	if (this->IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	// Make sure we have at least numallocated + num allocations.
	// Use the grow rules specified for this memory (in m_nGrowSize)
	int nAllocationRequested = c_utl_memory<T>::m_nAllocationCount + num;

	c_utl_memory<T>::m_nAllocationCount = UtlMemory_CalcNewAllocationCount(c_utl_memory<T>::m_nAllocationCount, c_utl_memory<T>::m_nGrowSize, nAllocationRequested, sizeof(T));

	UTLMEMORY_TRACK_ALLOC();

	if (c_utl_memory<T>::m_pMemory) {
		MEM_ALLOC_CREDIT_CLASS();
		c_utl_memory<T>::m_pMemory = (T*)MemAlloc_ReallocAligned(c_utl_memory<T>::m_pMemory, c_utl_memory<T>::m_nAllocationCount * sizeof(T), nAlignment);
		assert(c_utl_memory<T>::m_pMemory);
	}
	else {
		MEM_ALLOC_CREDIT_CLASS();
		c_utl_memory<T>::m_pMemory = (T*)MemAlloc_AllocAligned(c_utl_memory<T>::m_nAllocationCount * sizeof(T), nAlignment);
		assert(c_utl_memory<T>::m_pMemory);
	}
}


//-----------------------------------------------------------------------------
// Makes sure we've got at least this much memory
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memoryAligned<T, nAlignment>::EnsureCapacity(int num)
{
	if (c_utl_memory<T>::m_nAllocationCount >= num)
		return;

	if (this->IsExternallyAllocated()) {
		// Can't grow a buffer whose memory was externally allocated 
		assert(0);
		return;
	}

	UTLMEMORY_TRACK_FREE();

	c_utl_memory<T>::m_nAllocationCount = num;

	UTLMEMORY_TRACK_ALLOC();

	if (c_utl_memory<T>::m_pMemory) {
		MEM_ALLOC_CREDIT_CLASS();
		c_utl_memory<T>::m_pMemory = (T*)MemAlloc_ReallocAligned(c_utl_memory<T>::m_pMemory, c_utl_memory<T>::m_nAllocationCount * sizeof(T), nAlignment);
	}
	else {
		MEM_ALLOC_CREDIT_CLASS();
		c_utl_memory<T>::m_pMemory = (T*)MemAlloc_AllocAligned(c_utl_memory<T>::m_nAllocationCount * sizeof(T), nAlignment);
	}
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------
template< class T, int nAlignment >
void c_utl_memoryAligned<T, nAlignment>::Purge()
{
	if (!this->IsExternallyAllocated()) {
		if (c_utl_memory<T>::m_pMemory) {
			UTLMEMORY_TRACK_FREE();
			MemAlloc_FreeAligned(c_utl_memory<T>::m_pMemory);
			c_utl_memory<T>::m_pMemory = 0;
		}
		c_utl_memory<T>::m_nAllocationCount = 0;
	}
}