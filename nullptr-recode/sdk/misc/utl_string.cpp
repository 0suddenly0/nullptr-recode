#include "..\sdk.h"
#include "utl_string.h"

#include <stdio.h>
#include <cstdarg>

//-----------------------------------------------------------------------------
// base class, containing simple memory management
//-----------------------------------------------------------------------------
c_utl_binary_block::c_utl_binary_block( int growSize, int initSize ) :
	m_Memory( growSize, initSize ) {
	m_nActualLength = 0;
}

c_utl_binary_block::c_utl_binary_block( void* pMemory, int nSizeInBytes, int nInitialLength ) :
	m_Memory( (unsigned char*)pMemory, nSizeInBytes ) {
	m_nActualLength = nInitialLength;
}

c_utl_binary_block::c_utl_binary_block( const void* pMemory, int nSizeInBytes ) :
	m_Memory( (const unsigned char*)pMemory, nSizeInBytes ) {
	m_nActualLength = nSizeInBytes;
}

c_utl_binary_block::c_utl_binary_block( const c_utl_binary_block& src ) {
	set( src.get(), src.length() );
}

void c_utl_binary_block::get( void* pValue, int nLen ) const {
	assert( nLen > 0 );
	if ( m_nActualLength < nLen ) {
		nLen = m_nActualLength;
	}

	if ( nLen > 0 ) {
		memcpy( pValue, m_Memory.base(), nLen );
	}
}

void c_utl_binary_block::set_length( int nLength ) {
	assert( !m_Memory.is_read_only() );

	m_nActualLength = nLength;
	if ( nLength > m_Memory.num_allocated() ) {
		int nOverFlow = nLength - m_Memory.num_allocated();
		m_Memory.grow( nOverFlow );

		// If the reallocation failed, clamp length
		if ( nLength > m_Memory.num_allocated() ) {
			m_nActualLength = m_Memory.num_allocated();
		}
	}

#ifdef _DEBUG
	if ( m_Memory.num_allocated() > m_nActualLength ) {
		memset( ( (char*)m_Memory.base() ) + m_nActualLength, 0xEB, m_Memory.num_allocated() - m_nActualLength );
	}
#endif
}

void c_utl_binary_block::set( const void* pValue, int nLen ) {
	assert( !m_Memory.is_read_only() );

	if ( !pValue ) {
		nLen = 0;
	}

	set_length( nLen );

	if ( m_nActualLength ) {
		if ( ( (const char*)m_Memory.base() ) >= ( (const char*)pValue ) + nLen ||
			( (const char*)m_Memory.base() ) + m_nActualLength <= ( (const char*)pValue ) ) {
			memcpy( m_Memory.base(), pValue, m_nActualLength );
		} else {
			memmove( m_Memory.base(), pValue, m_nActualLength );
		}
	}
}

c_utl_binary_block& c_utl_binary_block::operator=( const c_utl_binary_block& src ) {
	assert( !m_Memory.is_read_only() );
	set( src.get(), src.length() );
	return *this;
}

bool c_utl_binary_block::operator==( const c_utl_binary_block& src ) const {
	if ( src.length() != length() )
		return false;

	return !memcmp( src.get(), get(), length() );
}

//-----------------------------------------------------------------------------
// Simple string class.
//-----------------------------------------------------------------------------
c_utl_string::c_utl_string() {
}

c_utl_string::c_utl_string( const char* pString ) {
	set( pString );
}

c_utl_string::c_utl_string( const c_utl_string& string ) {
	set( string.get() );
}

// Attaches the string to external memory. Useful for avoiding a copy
c_utl_string::c_utl_string( void* pMemory, int nSizeInBytes, int nInitialLength ) :
	m_Storage( pMemory, nSizeInBytes, nInitialLength ) {
}

c_utl_string::c_utl_string( const void* pMemory, int nSizeInBytes ) :
	m_Storage( pMemory, nSizeInBytes ) {
}

void c_utl_string::set( const char* pValue ) {
	assert( !m_Storage.is_read_only() );
	int nLen = pValue ? strlen( pValue ) + 1 : 0;
	m_Storage.set( pValue, nLen );
}

// Returns strlen
int c_utl_string::length() const {
	return m_Storage.length() ? m_Storage.length() - 1 : 0;
}

// Sets the length (used to serialize into the buffer )
void c_utl_string::set_length( int nLen ) {
	assert( !m_Storage.is_read_only() );

	// Add 1 to account for the NULL
	m_Storage.set_length( nLen > 0 ? nLen + 1 : 0 );
}

const char* c_utl_string::get() const {
	if ( m_Storage.length() == 0 ) {
		return "";
	}

	return ( const char* )( m_Storage.get() );
}

// Converts to c-strings
c_utl_string::operator const char*() const {
	return get();
}

char* c_utl_string::get() {
	assert( !m_Storage.is_read_only() );

	if ( m_Storage.length() == 0 ) {
		// In general, we optimise away small mallocs for empty strings
		// but if you ask for the non-const bytes, they must be writable
		// so we can't return "" here, like we do for the const version - jd
		m_Storage.set_length( 1 );
		m_Storage[0] = '\0';
	}

	return ( char* )( m_Storage.get() );
}

c_utl_string& c_utl_string::operator=( const c_utl_string& src ) {
	assert( !m_Storage.is_read_only() );
	m_Storage = src.m_Storage;
	return *this;
}

c_utl_string& c_utl_string::operator=( const char* src ) {
	assert( !m_Storage.is_read_only() );
	set( src );
	return *this;
}

bool c_utl_string::operator==( const c_utl_string& src ) const {
	return m_Storage == src.m_Storage;
}

bool c_utl_string::operator==( const char* src ) const {
	return ( strcmp( get(), src ) == 0 );
}

c_utl_string& c_utl_string::operator+=( const c_utl_string& rhs ) {
	assert( !m_Storage.is_read_only() );

	const int lhsLength( length() );
	const int rhsLength( rhs.length() );
	const int requestedLength( lhsLength + rhsLength );

	set_length( requestedLength );
	const int allocatedLength( length() );
	const int copyLength( allocatedLength - lhsLength < rhsLength ? allocatedLength - lhsLength : rhsLength );
	memcpy( get() + lhsLength, rhs.get(), copyLength );
	m_Storage[allocatedLength] = '\0';

	return *this;
}

c_utl_string& c_utl_string::operator+=( const char* rhs ) {
	assert( !m_Storage.is_read_only() );

	const int lhsLength( length() );
	const int rhsLength( strlen( rhs ) );
	const int requestedLength( lhsLength + rhsLength );

	set_length( requestedLength );
	const int allocatedLength( length() );
	const int copyLength( allocatedLength - lhsLength < rhsLength ? allocatedLength - lhsLength : rhsLength );
	memcpy( get() + lhsLength, rhs, copyLength );
	m_Storage[allocatedLength] = '\0';

	return *this;
}

c_utl_string& c_utl_string::operator+=( char c ) {
	assert( !m_Storage.is_read_only() );

	int nLength = length();
	set_length( nLength + 1 );
	m_Storage[nLength] = c;
	m_Storage[nLength + 1] = '\0';
	return *this;
}

c_utl_string& c_utl_string::operator+=( int rhs ) {
	assert( !m_Storage.is_read_only() );
	assert( sizeof( rhs ) == 4 );

	char tmpBuf[12]; // Sufficient for a signed 32 bit integer [ -2147483648 to +2147483647 ]
	snprintf( tmpBuf, sizeof( tmpBuf ), "%d", rhs );
	tmpBuf[sizeof( tmpBuf ) - 1] = '\0';

	return operator+=( tmpBuf );
}

c_utl_string& c_utl_string::operator+=( double rhs ) {
	assert( !m_Storage.is_read_only() );

	char tmpBuf[256]; // How big can doubles be???  Dunno.
	snprintf( tmpBuf, sizeof( tmpBuf ), "%lg", rhs );
	tmpBuf[sizeof( tmpBuf ) - 1] = '\0';

	return operator+=( tmpBuf );
}

int c_utl_string::format( const char* pFormat, ... ) {
	assert( !m_Storage.is_read_only() );

	char tmpBuf[4096]; //< Nice big 4k buffer, as much memory as my first computer had, a Radio Shack Color Computer

	va_list marker;

	va_start( marker, pFormat );
	int len = _vsnprintf_s( tmpBuf, 4096, sizeof( tmpBuf ) - 1, pFormat, marker );
	va_end( marker );

	// Len < 0 represents an overflow
	if ( len < 0 ) {
		len = sizeof( tmpBuf ) - 1;
		tmpBuf[sizeof( tmpBuf ) - 1] = 0;
	}

	set( tmpBuf );

	return len;
}

//-----------------------------------------------------------------------------
// Strips the trailing slash
//-----------------------------------------------------------------------------
void c_utl_string::strip_trailing_slash() {
	if ( is_empty() )
		return;

	int nLastChar = length() - 1;
	char c = m_Storage[nLastChar];
	if ( c == '\\' || c == '/' ) {
		m_Storage[nLastChar] = 0;
		m_Storage.set_length( m_Storage.length() - 1 );
	}
}

