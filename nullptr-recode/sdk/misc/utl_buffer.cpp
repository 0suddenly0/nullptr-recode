//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// $Header: $
// $NoKeywords: $
//
// Serialization buffer
//===========================================================================//

#pragma warning( disable : 4514 )

#include "..\sdk.h"
#include "utl_buffer.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include "characterset.h"

const char* V_strnchr( const char* pStr, char c, int n ) {
	char const* pLetter = pStr;
	char const* pLast = pStr + n;

	// Check the entire string
	while ( ( pLetter < pLast ) && ( *pLetter != 0 ) ) {
		if ( *pLetter == c )
			return pLetter;
		++pLetter;
	}
	return NULL;
}
//-----------------------------------------------------------------------------
// Finds a string in another string with a case insensitive test w/ length validation
//-----------------------------------------------------------------------------
char const* V_strnistr( char const* pStr, char const* pSearch, int n ) {
	if ( !pStr || !pSearch )
		return 0;

	char const* pLetter = pStr;

	// Check the entire string
	while ( *pLetter != 0 ) {
		if ( n <= 0 )
			return 0;

		// Skip over non-matches
		if ( tolower( *pLetter ) == tolower( *pSearch ) ) {
			int n1 = n - 1;

			// Check for match
			char const* pMatch = pLetter + 1;
			char const* pTest = pSearch + 1;
			while ( *pTest != 0 ) {
				if ( n1 <= 0 )
					return 0;

				// We've run off the end; don't bother.
				if ( *pMatch == 0 )
					return 0;

				if ( tolower( *pMatch ) != tolower( *pTest ) )
					break;

				++pMatch;
				++pTest;
				--n1;
			}

			// Found a match!
			if ( *pTest == 0 )
				return pLetter;
		}

		++pLetter;
		--n;
	}

	return 0;
}
//-----------------------------------------------------------------------------
// Character conversions for C strings
//-----------------------------------------------------------------------------
class c_utl_cstring_conversion : public c_utl_char_conversion {
public:
	c_utl_cstring_conversion( char nEscapeChar, const char* pDelimiter, int nCount, conversion_array_t* pArray );

	// Finds a conversion for the passed-in string, returns length
	virtual char find_conversion( const char* pString, int* pLength );

private:
	char m_pConversion[255];
};

//-----------------------------------------------------------------------------
// Character conversions for no-escape sequence strings
//-----------------------------------------------------------------------------
class c_utl_no_esc_conversion : public c_utl_char_conversion {
public:
	c_utl_no_esc_conversion( char nEscapeChar, const char* pDelimiter, int nCount, conversion_array_t* pArray ) :
		c_utl_char_conversion( nEscapeChar, pDelimiter, nCount, pArray ) {
	}

	// Finds a conversion for the passed-in string, returns length
	virtual char find_conversion( const char* pString, int* pLength ) {
		*pLength = 0;
		return 0;
	}
};

//-----------------------------------------------------------------------------
// List of character conversions
//-----------------------------------------------------------------------------
BEGIN_CUSTOM_CHAR_CONVERSION( c_utl_cstring_conversion, s_StringCharConversion, "\"", '\\' ){
	'\n', "n"
},
	{ '\t', "t" },
	{ '\v', "v" },
	{ '\b', "b" },
	{ '\r', "r" },
	{ '\f', "f" },
	{ '\a', "a" },
	{ '\\', "\\" },
	{ '\?', "\?" },
	{ '\'', "\'" },
	{ '\"', "\"" },
		END_CUSTOM_CHAR_CONVERSION( c_utl_cstring_conversion, s_StringCharConversion, "\"", '\\' );

	c_utl_char_conversion* GetCStringCharConversion() {
		return &s_StringCharConversion;
	}

	BEGIN_CUSTOM_CHAR_CONVERSION( c_utl_no_esc_conversion, s_NoEscConversion, "\"", 0x7F ){
		0x7F, ""
	},
		END_CUSTOM_CHAR_CONVERSION( c_utl_no_esc_conversion, s_NoEscConversion, "\"", 0x7F );

		c_utl_char_conversion* GetNoEscCharConversion() {
			return &s_NoEscConversion;
		}

		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		c_utl_cstring_conversion::c_utl_cstring_conversion( char nEscapeChar, const char* pDelimiter, int nCount, conversion_array_t* pArray ) :
			c_utl_char_conversion( nEscapeChar, pDelimiter, nCount, pArray ) {
			memset( m_pConversion, 0x0, sizeof( m_pConversion ) );
			for ( int i = 0; i < nCount; ++i ) {
				m_pConversion[pArray[i].m_pReplacementString[0]] = pArray[i].m_nActualChar;
			}
		}

		// Finds a conversion for the passed-in string, returns length
		char c_utl_cstring_conversion::find_conversion( const char* pString, int* pLength ) {
			char c = m_pConversion[pString[0]];
			*pLength = ( c != '\0' ) ? 1 : 0;
			return c;
		}

		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		c_utl_char_conversion::c_utl_char_conversion( char nEscapeChar, const char* pDelimiter, int nCount, conversion_array_t* pArray ) {
			m_nEscapeChar = nEscapeChar;
			m_pDelimiter = pDelimiter;
			m_nCount = nCount;
			m_nDelimiterLength = strlen( pDelimiter );
			m_nMaxConversionLength = 0;

			memset( m_pReplacements, 0, sizeof( m_pReplacements ) );

			for ( int i = 0; i < nCount; ++i ) {
				m_pList[i] = pArray[i].m_nActualChar;
				conversion_info_t& info = m_pReplacements[m_pList[i]];
				assert( info.m_pReplacementString == 0 );
				info.m_pReplacementString = pArray[i].m_pReplacementString;
				info.m_nLength = strlen( info.m_pReplacementString );
				if ( info.m_nLength > m_nMaxConversionLength ) {
					m_nMaxConversionLength = info.m_nLength;
				}
			}
		}

		//-----------------------------------------------------------------------------
		// Escape character + delimiter
		//-----------------------------------------------------------------------------
		char c_utl_char_conversion::get_escape_char() const {
			return m_nEscapeChar;
		}

		const char* c_utl_char_conversion::get_delimiter() const {
			return m_pDelimiter;
		}

		int c_utl_char_conversion::get_delimiter_length() const {
			return m_nDelimiterLength;
		}

		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		const char* c_utl_char_conversion::get_conversion_string( char c ) const {
			return m_pReplacements[c].m_pReplacementString;
		}

		int c_utl_char_conversion::get_conversion_length( char c ) const {
			return m_pReplacements[c].m_nLength;
		}

		int c_utl_char_conversion::max_conversion_length() const {
			return m_nMaxConversionLength;
		}

		//-----------------------------------------------------------------------------
		// Finds a conversion for the passed-in string, returns length
		//-----------------------------------------------------------------------------
		char c_utl_char_conversion::find_conversion( const char* pString, int* pLength ) {
			for ( int i = 0; i < m_nCount; ++i ) {
				if ( !strcmp( pString, m_pReplacements[m_pList[i]].m_pReplacementString ) ) {
					*pLength = m_pReplacements[m_pList[i]].m_nLength;
					return m_pList[i];
				}
			}

			*pLength = 0;
			return '\0';
		}

		//-----------------------------------------------------------------------------
		// constructors
		//-----------------------------------------------------------------------------
		c_utl_buffer::c_utl_buffer( int growSize, int initSize, int nFlags ) :
			m_Memory( growSize, initSize ), m_Error( 0 ) {
			m_Get = 0;
			m_Put = 0;
			m_nTab = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			if ( ( initSize != 0 ) && !is_read_only() ) {
				m_nMaxPut = -1;
				add_null_termination();
			} else {
				m_nMaxPut = 0;
			}
			set_overflow_funcs( &c_utl_buffer::get_overflow, &c_utl_buffer::put_overflow );
		}

		c_utl_buffer::c_utl_buffer( const void* pBuffer, int nSize, int nFlags ) :
			m_Memory( (unsigned char*)pBuffer, nSize ), m_Error( 0 ) {
			assert( nSize != 0 );

			m_Get = 0;
			m_Put = 0;
			m_nTab = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			if ( is_read_only() ) {
				m_nMaxPut = nSize;
			} else {
				m_nMaxPut = -1;
				add_null_termination();
			}
			set_overflow_funcs( &c_utl_buffer::get_overflow, &c_utl_buffer::put_overflow );
		}

		//-----------------------------------------------------------------------------
		// Modifies the buffer to be binary or text; Blows away the buffer and the CONTAINS_CRLF value.
		//-----------------------------------------------------------------------------
		void c_utl_buffer::set_buffer_type( bool bIsText, bool bContainsCRLF ) {
		#ifdef _DEBUG
			// If the buffer is empty, there is no opportunity for this stuff to fail
			if ( tell_max_put() != 0 ) {
				if ( is_text() ) {
					if ( bIsText ) {
						assert( contains_crlf() == bContainsCRLF );
					} else {
						assert( contains_crlf() );
					}
				} else {
					if ( bIsText ) {
						assert( bContainsCRLF );
					}
				}
			}
		#endif

			if ( bIsText ) {
				m_Flags |= TEXT_BUFFER;
			} else {
				m_Flags &= ~TEXT_BUFFER;
			}
			if ( bContainsCRLF ) {
				m_Flags |= CONTAINS_CRLF;
			} else {
				m_Flags &= ~CONTAINS_CRLF;
			}
		}

		//-----------------------------------------------------------------------------
		// Attaches the buffer to external memory....
		//-----------------------------------------------------------------------------
		void c_utl_buffer::set_external_buffer( void* pMemory, int nSize, int nInitialPut, int nFlags ) {
			m_Memory.set_external_buffer( (unsigned char*)pMemory, nSize );

			// Reset all indices; we just changed memory
			m_Get = 0;
			m_Put = nInitialPut;
			m_nTab = 0;
			m_Error = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			m_nMaxPut = -1;
			add_null_termination();
		}

		//-----------------------------------------------------------------------------
		// Assumes an external buffer but manages its deletion
		//-----------------------------------------------------------------------------
		void c_utl_buffer::assume_memory( void* pMemory, int nSize, int nInitialPut, int nFlags ) {
			m_Memory.assume_memory( (unsigned char*)pMemory, nSize );

			// Reset all indices; we just changed memory
			m_Get = 0;
			m_Put = nInitialPut;
			m_nTab = 0;
			m_Error = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			m_nMaxPut = -1;
			add_null_termination();
		}

		//-----------------------------------------------------------------------------
		// Makes sure we've got at least this much memory
		//-----------------------------------------------------------------------------
		void c_utl_buffer::ensure_capacity( int num ) {
			// Add one extra for the null termination
			num += 1;
			if ( m_Memory.is_externally_allocated() ) {
				if ( is_growable() && ( m_Memory.num_allocated() < num ) ) {
					m_Memory.convert_to_growable_memory( 0 );
				} else {
					num -= 1;
				}
			}

			m_Memory.ensure_capacity( num );
		}

		//-----------------------------------------------------------------------------
		// base get method from which all others derive
		//-----------------------------------------------------------------------------
		void c_utl_buffer::get( void* pMem, int size ) {
			if ( check_get( size ) ) {
				memcpy( pMem, &m_Memory[m_Get - m_nOffset], size );
				m_Get += size;
			}
		}

		//-----------------------------------------------------------------------------
		// This will get at least 1 uint8_t and up to nSize bytes.
		// It will return the number of bytes actually read.
		//-----------------------------------------------------------------------------
		int c_utl_buffer::get_up_to( void* pMem, int nSize ) {
			if ( check_arbitrary_peek_get( 0, nSize ) ) {
				memcpy( pMem, &m_Memory[m_Get - m_nOffset], nSize );
				m_Get += nSize;
				return nSize;
			}
			return 0;
		}

		//-----------------------------------------------------------------------------
		// Eats whitespace
		//-----------------------------------------------------------------------------
		void c_utl_buffer::eat_white_space() {
			if ( is_text() && is_valid() ) {
				while ( check_get( sizeof( char ) ) ) {
					if ( !isspace( *(const unsigned char*)peek_get() ) )
						break;
					m_Get += sizeof( char );
				}
			}
		}

		//-----------------------------------------------------------------------------
		// Eats C++ style comments
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::eat_cpp_comment() {
			if ( is_text() && is_valid() ) {
				// If we don't have a a c++ style comment next, we're done
				const char* pPeek = (const char*)peek_get( 2 * sizeof( char ), 0 );
				if ( !pPeek || ( pPeek[0] != '/' ) || ( pPeek[1] != '/' ) )
					return false;

				// Deal with c++ style comments
				m_Get += 2;

				// read complete line
				for ( char c = get_char(); is_valid(); c = get_char() ) {
					if ( c == '\n' )
						break;
				}
				return true;
			}
			return false;
		}

		//-----------------------------------------------------------------------------
		// Peeks how much whitespace to eat
		//-----------------------------------------------------------------------------
		int c_utl_buffer::peek_white_space( int nOffset ) {
			if ( !is_text() || !is_valid() )
				return 0;

			while ( check_peek_get( nOffset, sizeof( char ) ) ) {
				if ( !isspace( *(unsigned char*)peek_get( nOffset ) ) )
					break;
				nOffset += sizeof( char );
			}

			return nOffset;
		}

		//-----------------------------------------------------------------------------
		// Peek size of sting to come, check memory bound
		//-----------------------------------------------------------------------------
		int c_utl_buffer::peek_string_length() {
			if ( !is_valid() )
				return 0;

			// Eat preceeding whitespace
			int nOffset = 0;
			if ( is_text() ) {
				nOffset = peek_white_space( nOffset );
			}

			int nStartingOffset = nOffset;

			do {
				int nPeekAmount = 128;

				// NOTE: Add 1 for the terminating zero!
				if ( !check_arbitrary_peek_get( nOffset, nPeekAmount ) ) {
					if ( nOffset == nStartingOffset )
						return 0;
					return nOffset - nStartingOffset + 1;
				}

				const char* pTest = (const char*)peek_get( nOffset );

				if ( !is_text() ) {
					for ( int i = 0; i < nPeekAmount; ++i ) {
						// The +1 here is so we eat the terminating 0
						if ( pTest[i] == 0 )
							return ( i + nOffset - nStartingOffset + 1 );
					}
				} else {
					for ( int i = 0; i < nPeekAmount; ++i ) {
						// The +1 here is so we eat the terminating 0
						if ( isspace( (unsigned char)pTest[i] ) || ( pTest[i] == 0 ) )
							return ( i + nOffset - nStartingOffset + 1 );
					}
				}

				nOffset += nPeekAmount;

			} while ( true );
		}

		//-----------------------------------------------------------------------------
		// Peek size of line to come, check memory bound
		//-----------------------------------------------------------------------------
		int c_utl_buffer::peek_line_length() {
			if ( !is_valid() )
				return 0;

			int nOffset = 0;
			int nStartingOffset = nOffset;

			do {
				int nPeekAmount = 128;

				// NOTE: Add 1 for the terminating zero!
				if ( !check_arbitrary_peek_get( nOffset, nPeekAmount ) ) {
					if ( nOffset == nStartingOffset )
						return 0;
					return nOffset - nStartingOffset + 1;
				}

				const char* pTest = (const char*)peek_get( nOffset );

				for ( int i = 0; i < nPeekAmount; ++i ) {
					// The +2 here is so we eat the terminating '\n' and 0
					if ( pTest[i] == '\n' || pTest[i] == '\r' )
						return ( i + nOffset - nStartingOffset + 2 );
					// The +1 here is so we eat the terminating 0
					if ( pTest[i] == 0 )
						return ( i + nOffset - nStartingOffset + 1 );
				}

				nOffset += nPeekAmount;

			} while ( true );
		}

		//-----------------------------------------------------------------------------
		// Does the next bytes of the buffer match a pattern?
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::peek_string_match( int nOffset, const char* pString, int nLen ) {
			if ( !check_peek_get( nOffset, nLen ) )
				return false;
			return !strncmp( (const char*)peek_get( nOffset ), pString, nLen );
		}

		//-----------------------------------------------------------------------------
		// This version of peek_string_length converts \" to \\ and " to \, etc.
		// It also reads a " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		int c_utl_buffer::peek_delimited_string_length( c_utl_char_conversion* pConv, bool bActualSize ) {
			if ( !is_text() || !pConv )
				return peek_string_length();

			// Eat preceeding whitespace
			int nOffset = 0;
			if ( is_text() ) {
				nOffset = peek_white_space( nOffset );
			}

			if ( !peek_string_match( nOffset, pConv->get_delimiter(), pConv->get_delimiter_length() ) )
				return 0;

			// Try to read ending ", but don't accept \"
			int nActualStart = nOffset;
			nOffset += pConv->get_delimiter_length();
			int nLen = 1; // Starts at 1 for the '\0' termination

			do {
				if ( peek_string_match( nOffset, pConv->get_delimiter(), pConv->get_delimiter_length() ) )
					break;

				if ( !check_peek_get( nOffset, 1 ) )
					break;

				char c = *(const char*)peek_get( nOffset );
				++nLen;
				++nOffset;
				if ( c == pConv->get_escape_char() ) {
					int nLength = pConv->max_conversion_length();
					if ( !check_arbitrary_peek_get( nOffset, nLength ) )
						break;

					pConv->find_conversion( (const char*)peek_get( nOffset ), &nLength );
					nOffset += nLength;
				}
			} while ( true );

			return bActualSize ? nLen : nOffset - nActualStart + pConv->get_delimiter_length() + 1;
		}

		//-----------------------------------------------------------------------------
		// Reads a null-terminated string
		//-----------------------------------------------------------------------------
		void c_utl_buffer::get_string( char* pString, int nMaxChars ) {
			if ( !is_valid() ) {
				*pString = 0;
				return;
			}

			if ( nMaxChars == 0 ) {
				nMaxChars = INT_MAX;
			}

			// Remember, this *includes* the null character
			// It will be 0, however, if the buffer is empty.
			int nLen = peek_string_length();

			if ( is_text() ) {
				eat_white_space();
			}

			if ( nLen == 0 ) {
				*pString = 0;
				m_Error |= GET_OVERFLOW;
				return;
			}

			// Strip off the terminating NULL
			if ( nLen <= nMaxChars ) {
				get( pString, nLen - 1 );
				pString[nLen - 1] = 0;
			} else {
				get( pString, nMaxChars - 1 );
				pString[nMaxChars - 1] = 0;
				seek_get( SEEK_CURRENT, nLen - 1 - nMaxChars );
			}

			// Read the terminating NULL in binary formats
			if ( !is_text() ) {
				assert( get_char() == 0 );
			}
		}

		//-----------------------------------------------------------------------------
		// Reads up to and including the first \n
		//-----------------------------------------------------------------------------
		void c_utl_buffer::get_line( char* pLine, int nMaxChars ) {
			assert( is_text() && !contains_crlf() );

			if ( !is_valid() ) {
				*pLine = 0;
				return;
			}

			if ( nMaxChars == 0 ) {
				nMaxChars = INT_MAX;
			}

			// Remember, this *includes* the null character
			// It will be 0, however, if the buffer is empty.
			int nLen = peek_line_length();
			if ( nLen == 0 ) {
				*pLine = 0;
				m_Error |= GET_OVERFLOW;
				return;
			}

			// Strip off the terminating NULL
			if ( nLen <= nMaxChars ) {
				get( pLine, nLen - 1 );
				pLine[nLen - 1] = 0;
			} else {
				get( pLine, nMaxChars - 1 );
				pLine[nMaxChars - 1] = 0;
				seek_get( SEEK_CURRENT, nLen - 1 - nMaxChars );
			}
		}

		//-----------------------------------------------------------------------------
		// This version of get_string converts \ to \\ and " to \", etc.
		// It also places " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		char c_utl_buffer::get_delimited_char_internal( c_utl_char_conversion* pConv ) {
			char c = get_char();
			if ( c == pConv->get_escape_char() ) {
				int nLength = pConv->max_conversion_length();
				if ( !check_arbitrary_peek_get( 0, nLength ) )
					return '\0';

				c = pConv->find_conversion( (const char*)peek_get(), &nLength );
				seek_get( SEEK_CURRENT, nLength );
			}

			return c;
		}

		char c_utl_buffer::get_delimited_char( c_utl_char_conversion* pConv ) {
			if ( !is_text() || !pConv )
				return get_char();
			return get_delimited_char_internal( pConv );
		}

		void c_utl_buffer::get_delimited_string( c_utl_char_conversion* pConv, char* pString, int nMaxChars ) {
			if ( !is_text() || !pConv ) {
				get_string( pString, nMaxChars );
				return;
			}

			if ( !is_valid() ) {
				*pString = 0;
				return;
			}

			if ( nMaxChars == 0 ) {
				nMaxChars = INT_MAX;
			}

			eat_white_space();
			if ( !peek_string_match( 0, pConv->get_delimiter(), pConv->get_delimiter_length() ) )
				return;

			// Pull off the starting delimiter
			seek_get( SEEK_CURRENT, pConv->get_delimiter_length() );

			int nRead = 0;
			while ( is_valid() ) {
				if ( peek_string_match( 0, pConv->get_delimiter(), pConv->get_delimiter_length() ) ) {
					seek_get( SEEK_CURRENT, pConv->get_delimiter_length() );
					break;
				}

				char c = get_delimited_char_internal( pConv );

				if ( nRead < nMaxChars ) {
					pString[nRead] = c;
					++nRead;
				}
			}

			if ( nRead >= nMaxChars ) {
				nRead = nMaxChars - 1;
			}
			pString[nRead] = '\0';
		}

		//-----------------------------------------------------------------------------
		// Checks if a get is ok
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::check_get( int nSize ) {
			if ( m_Error & GET_OVERFLOW )
				return false;

			if ( tell_max_put() < m_Get + nSize ) {
				m_Error |= GET_OVERFLOW;
				return false;
			}

			if ( ( m_Get < m_nOffset ) || ( m_Memory.num_allocated() < m_Get - m_nOffset + nSize ) ) {
				if ( !on_get_overflow( nSize ) ) {
					m_Error |= GET_OVERFLOW;
					return false;
				}
			}

			return true;
		}

		//-----------------------------------------------------------------------------
		// Checks if a peek get is ok
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::check_peek_get( int nOffset, int nSize ) {
			if ( m_Error & GET_OVERFLOW )
				return false;

			// Checking for peek can't Set the overflow flag
			bool bOk = check_get( nOffset + nSize );
			m_Error &= ~GET_OVERFLOW;
			return bOk;
		}

		//-----------------------------------------------------------------------------
		// Call this to peek arbitrarily long into memory. It doesn't fail unless
		// it can't read *anything* new
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::check_arbitrary_peek_get( int nOffset, int& nIncrement ) {
			if ( tell_get() + nOffset >= tell_max_put() ) {
				nIncrement = 0;
				return false;
			}

			if ( tell_get() + nOffset + nIncrement > tell_max_put() ) {
				nIncrement = tell_max_put() - tell_get() - nOffset;
			}

			// NOTE: check_peek_get could modify tell_max_put for streaming files
			// We have to call tell_max_put again here
			check_peek_get( nOffset, nIncrement );
			int nMaxGet = tell_max_put() - tell_get();
			if ( nMaxGet < nIncrement ) {
				nIncrement = nMaxGet;
			}
			return ( nIncrement != 0 );
		}

		//-----------------------------------------------------------------------------
		// Peek part of the butt
		//-----------------------------------------------------------------------------
		const void* c_utl_buffer::peek_get( int nMaxSize, int nOffset ) {
			if ( !check_peek_get( nOffset, nMaxSize ) )
				return NULL;
			return &m_Memory[m_Get + nOffset - m_nOffset];
		}

		//-----------------------------------------------------------------------------
		// Change where I'm reading
		//-----------------------------------------------------------------------------
		void c_utl_buffer::seek_get( seek_type_t type, int offset ) {
			switch ( type ) {
			case SEEK_HEAD:
				m_Get = offset;
				break;

			case SEEK_CURRENT:
				m_Get += offset;
				break;

			case SEEK_TAIL:
				m_Get = m_nMaxPut - offset;
				break;
			}

			if ( m_Get > m_nMaxPut ) {
				m_Error |= GET_OVERFLOW;
			} else {
				m_Error &= ~GET_OVERFLOW;
				if ( m_Get < m_nOffset || m_Get >= m_nOffset + size() ) {
					on_get_overflow( -1 );
				}
			}
		}

		//-----------------------------------------------------------------------------
		// Parse...
		//-----------------------------------------------------------------------------

	#pragma warning( disable : 4706 )

		int c_utl_buffer::vascanf( const char* pFmt, va_list list ) {
			assert( pFmt );
			if ( m_Error || !is_text() )
				return 0;

			int numScanned = 0;
			int nLength;
			char c;
			char* pEnd;
			while ( c = *pFmt++ ) {
				// Stop if we hit the end of the buffer
				if ( m_Get >= tell_max_put() ) {
					m_Error |= GET_OVERFLOW;
					break;
				}

				switch ( c ) {
				case ' ':
					// eat all whitespace
					eat_white_space();
					break;

				case '%': {
					// Conversion character... try to convert baby!
					char type = *pFmt++;
					if ( type == 0 )
						return numScanned;

					switch ( type ) {
					case 'c': {
						char* ch = va_arg( list, char* );
						if ( check_peek_get( 0, sizeof( char ) ) ) {
							*ch = *(const char*)peek_get();
							++m_Get;
						} else {
							*ch = 0;
							return numScanned;
						}
					} break;

					case 'i':
					case 'd': {
						int* i = va_arg( list, int* );

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if ( !check_arbitrary_peek_get( 0, nLength ) ) {
							*i = 0;
							return numScanned;
						}

						*i = strtol( (char*)peek_get(), &pEnd, 10 );
						int nBytesRead = (int)( pEnd - (char*)peek_get() );
						if ( nBytesRead == 0 )
							return numScanned;
						m_Get += nBytesRead;
					} break;

					case 'x': {
						int* i = va_arg( list, int* );

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if ( !check_arbitrary_peek_get( 0, nLength ) ) {
							*i = 0;
							return numScanned;
						}

						*i = strtol( (char*)peek_get(), &pEnd, 16 );
						int nBytesRead = (int)( pEnd - (char*)peek_get() );
						if ( nBytesRead == 0 )
							return numScanned;
						m_Get += nBytesRead;
					} break;

					case 'u': {
						unsigned int* u = va_arg( list, unsigned int* );

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if ( !check_arbitrary_peek_get( 0, nLength ) ) {
							*u = 0;
							return numScanned;
						}

						*u = strtoul( (char*)peek_get(), &pEnd, 10 );
						int nBytesRead = (int)( pEnd - (char*)peek_get() );
						if ( nBytesRead == 0 )
							return numScanned;
						m_Get += nBytesRead;
					} break;

					case 'f': {
						float* f = va_arg( list, float* );

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if ( !check_arbitrary_peek_get( 0, nLength ) ) {
							*f = 0.0f;
							return numScanned;
						}

						*f = (float)strtod( (char*)peek_get(), &pEnd );
						int nBytesRead = (int)( pEnd - (char*)peek_get() );
						if ( nBytesRead == 0 )
							return numScanned;
						m_Get += nBytesRead;
					} break;

					case 's': {
						char* s = va_arg( list, char* );
						get_string( s );
					} break;

					default: {
						// unimplemented scanf type
						assert( 0 );
						return numScanned;
					} break;
					}

					++numScanned;
				} break;

				default: {
					// Here we have to match the format string character
					// against what's in the buffer or we're done.
					if ( !check_peek_get( 0, sizeof( char ) ) )
						return numScanned;

					if ( c != *(const char*)peek_get() )
						return numScanned;

					++m_Get;
				}
				}
			}
			return numScanned;
		}

	#pragma warning( default : 4706 )

		int c_utl_buffer::scanf( const char* pFmt, ... ) {
			va_list args;

			va_start( args, pFmt );
			int count = vascanf( pFmt, args );
			va_end( args );

			return count;
		}

		//-----------------------------------------------------------------------------
		// Advance the get index until after the particular string is found
		// Do not eat whitespace before starting. Return false if it failed
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::get_token( const char* pToken ) {
			assert( pToken );

			// Look for the token
			int nLen = strlen( pToken );

			int nSizeToCheck = size() - tell_get() - m_nOffset;

			int nGet = tell_get();
			do {
				int nMaxSize = tell_max_put() - tell_get();
				if ( nMaxSize < nSizeToCheck ) {
					nSizeToCheck = nMaxSize;
				}
				if ( nLen > nSizeToCheck )
					break;

				if ( !check_peek_get( 0, nSizeToCheck ) )
					break;

				const char* pBufStart = (const char*)peek_get();
				const char* pFoundEnd = V_strnistr( pBufStart, pToken, nSizeToCheck );
				if ( pFoundEnd ) {
					size_t nOffset = (size_t)pFoundEnd - (size_t)pBufStart;
					seek_get( c_utl_buffer::SEEK_CURRENT, nOffset + nLen );
					return true;
				}

				seek_get( c_utl_buffer::SEEK_CURRENT, nSizeToCheck - nLen - 1 );
				nSizeToCheck = size() - ( nLen - 1 );

			} while ( true );

			seek_get( c_utl_buffer::SEEK_HEAD, nGet );
			return false;
		}

		//-----------------------------------------------------------------------------
		// (For text buffers only)
		// Parse a token from the buffer:
		// Grab all text that lies between a starting delimiter + ending delimiter
		// (skipping whitespace that leads + trails both delimiters).
		// Note the delimiter checks are case-insensitive.
		// If successful, the get index is advanced and the function returns true,
		// otherwise the index is not advanced and the function returns false.
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::parse_token( const char* pStartingDelim, const char* pEndingDelim, char* pString, int nMaxLen ) {
			int nCharsToCopy = 0;
			int nCurrentGet = 0;

			size_t nEndingDelimLen;

			// Starting delimiter is optional
			char emptyBuf = '\0';
			if ( !pStartingDelim ) {
				pStartingDelim = &emptyBuf;
			}

			// Ending delimiter is not
			assert( pEndingDelim && pEndingDelim[0] );
			nEndingDelimLen = strlen( pEndingDelim );

			int nStartGet = tell_get();
			char nCurrChar;
			int nTokenStart = -1;
			eat_white_space();
			while ( *pStartingDelim ) {
				nCurrChar = *pStartingDelim++;
				if ( !isspace( (unsigned char)nCurrChar ) ) {
					if ( tolower( get_char() ) != tolower( nCurrChar ) )
						goto parseFailed;
				} else {
					eat_white_space();
				}
			}

			eat_white_space();
			nTokenStart = tell_get();
			if ( !get_token( pEndingDelim ) )
				goto parseFailed;

			nCurrentGet = tell_get();
			nCharsToCopy = ( nCurrentGet - nEndingDelimLen ) - nTokenStart;
			if ( nCharsToCopy >= nMaxLen ) {
				nCharsToCopy = nMaxLen - 1;
			}

			if ( nCharsToCopy > 0 ) {
				seek_get( c_utl_buffer::SEEK_HEAD, nTokenStart );
				get( pString, nCharsToCopy );
				if ( !is_valid() )
					goto parseFailed;

				// Eat trailing whitespace
				for ( ; nCharsToCopy > 0; --nCharsToCopy ) {
					if ( !isspace( (unsigned char)pString[nCharsToCopy - 1] ) )
						break;
				}
			}
			pString[nCharsToCopy] = '\0';

			// Advance the get index
			seek_get( c_utl_buffer::SEEK_HEAD, nCurrentGet );
			return true;

		parseFailed:
			// Revert the get index
			seek_get( SEEK_HEAD, nStartGet );
			pString[0] = '\0';
			return false;
		}

		//-----------------------------------------------------------------------------
		// Parses the next token, given a Set of character breaks to stop at
		//-----------------------------------------------------------------------------
		int c_utl_buffer::parse_token( characterset_t* pBreaks, char* pTokenBuf, int nMaxLen, bool bParseComments ) {
			assert( nMaxLen > 0 );
			pTokenBuf[0] = 0;

			// skip whitespace + comments
			while ( true ) {
				if ( !is_valid() )
					return -1;
				eat_white_space();
				if ( bParseComments ) {
					if ( !eat_cpp_comment() )
						break;
				} else {
					break;
				}
			}

			char c = get_char();

			// End of buffer
			if ( c == 0 )
				return -1;

			// handle quoted strings specially
			if ( c == '\"' ) {
				int nLen = 0;
				while ( is_valid() ) {
					c = get_char();
					if ( c == '\"' || !c ) {
						pTokenBuf[nLen] = 0;
						return nLen;
					}
					pTokenBuf[nLen] = c;
					if ( ++nLen == nMaxLen ) {
						pTokenBuf[nLen - 1] = 0;
						return nMaxLen;
					}
				}

				// In this case, we hit the end of the buffer before hitting the end qoute
				pTokenBuf[nLen] = 0;
				return nLen;
			}

			// parse single characters
			if ( IN_CHARACTERSET( *pBreaks, c ) ) {
				pTokenBuf[0] = c;
				pTokenBuf[1] = 0;
				return 1;
			}

			// parse a regular word
			int nLen = 0;
			while ( true ) {
				pTokenBuf[nLen] = c;
				if ( ++nLen == nMaxLen ) {
					pTokenBuf[nLen - 1] = 0;
					return nMaxLen;
				}
				c = get_char();
				if ( !is_valid() )
					break;

				if ( IN_CHARACTERSET( *pBreaks, c ) || c == '\"' || c <= ' ' ) {
					seek_get( SEEK_CURRENT, -1 );
					break;
				}
			}

			pTokenBuf[nLen] = 0;
			return nLen;
		}

		//-----------------------------------------------------------------------------
		// Serialization
		//-----------------------------------------------------------------------------
		void c_utl_buffer::put( const void* pMem, int size ) {
			if ( size && check_put( size ) ) {
				memcpy( &m_Memory[m_Put - m_nOffset], pMem, size );
				m_Put += size;

				add_null_termination();
			}
		}

		//-----------------------------------------------------------------------------
		// Writes a null-terminated string
		//-----------------------------------------------------------------------------
		void c_utl_buffer::put_string( const char* pString ) {
			if ( !is_text() ) {
				if ( pString ) {
					// Not text? append a null at the end.
					size_t nLen = strlen( pString ) + 1;
					put( pString, nLen * sizeof( char ) );
					return;
				} else {
					put_type_bin< char >( 0 );
				}
			} else if ( pString ) {
				int nTabCount = ( m_Flags & AUTO_TABS_DISABLED ) ? 0 : m_nTab;
				if ( nTabCount > 0 ) {
					if ( was_last_character_cr() ) {
						put_tabs();
					}

					const char* pEndl = strchr( pString, '\n' );
					while ( pEndl ) {
						size_t nSize = (size_t)pEndl - (size_t)pString + sizeof( char );
						put( pString, nSize );
						pString = pEndl + 1;
						if ( *pString ) {
							put_tabs();
							pEndl = strchr( pString, '\n' );
						} else {
							pEndl = NULL;
						}
					}
				}
				size_t nLen = strlen( pString );
				if ( nLen ) {
					put( pString, nLen * sizeof( char ) );
				}
			}
		}

		//-----------------------------------------------------------------------------
		// This version of put_string converts \ to \\ and " to \", etc.
		// It also places " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		void c_utl_buffer::put_delimited_char_internal( c_utl_char_conversion* pConv, char c ) {
			int l = pConv->get_conversion_length( c );
			if ( l == 0 ) {
				put_char( c );
			} else {
				put_char( pConv->get_escape_char() );
				put( pConv->get_conversion_string( c ), l );
			}
		}

		void c_utl_buffer::put_delimited_char( c_utl_char_conversion* pConv, char c ) {
			if ( !is_text() || !pConv ) {
				put_char( c );
				return;
			}

			put_delimited_char_internal( pConv, c );
		}

		void c_utl_buffer::put_delimited_string( c_utl_char_conversion* pConv, const char* pString ) {
			if ( !is_text() || !pConv ) {
				put_string( pString );
				return;
			}

			if ( was_last_character_cr() ) {
				put_tabs();
			}
			put( pConv->get_delimiter(), pConv->get_delimiter_length() );

			int nLen = pString ? strlen( pString ) : 0;
			for ( int i = 0; i < nLen; ++i ) {
				put_delimited_char_internal( pConv, pString[i] );
			}

			if ( was_last_character_cr() ) {
				put_tabs();
			}
			put( pConv->get_delimiter(), pConv->get_delimiter_length() );
		}

		void c_utl_buffer::vaprintf( const char* pFmt, va_list list ) {
			char temp[2048];
			int nLen = vsnprintf( temp, sizeof( temp ), pFmt, list );
			assert( nLen < 2048 );
			put_string( temp );
		}

		void c_utl_buffer::printf( const char* pFmt, ... ) {
			va_list args;

			va_start( args, pFmt );
			vaprintf( pFmt, args );
			va_end( args );
		}

		//-----------------------------------------------------------------------------
		// Calls the overflow functions
		//-----------------------------------------------------------------------------
		void c_utl_buffer::set_overflow_funcs( utl_buffer_overflow_func_t getFunc, utl_buffer_overflow_func_t putFunc ) {
			m_GetOverflowFunc = getFunc;
			m_PutOverflowFunc = putFunc;
		}

		//-----------------------------------------------------------------------------
		// Calls the overflow functions
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::on_put_overflow( int nSize ) {
			return ( this->*m_PutOverflowFunc )( nSize );
		}

		bool c_utl_buffer::on_get_overflow( int nSize ) {
			return ( this->*m_GetOverflowFunc )( nSize );
		}

		//-----------------------------------------------------------------------------
		// Checks if a put is ok
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::put_overflow( int nSize ) {
			if ( m_Memory.is_externally_allocated() ) {
				if ( !is_growable() )
					return false;

				m_Memory.convert_to_growable_memory( 0 );
			}

			while ( size() < m_Put - m_nOffset + nSize ) {
				m_Memory.grow();
			}

			return true;
		}

		bool c_utl_buffer::get_overflow( int nSize ) {
			return false;
		}

		//-----------------------------------------------------------------------------
		// Checks if a put is ok
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::check_put( int nSize ) {
			if ( ( m_Error & PUT_OVERFLOW ) || is_read_only() )
				return false;

			if ( ( m_Put < m_nOffset ) || ( m_Memory.num_allocated() < m_Put - m_nOffset + nSize ) ) {
				if ( !on_put_overflow( nSize ) ) {
					m_Error |= PUT_OVERFLOW;
					return false;
				}
			}
			return true;
		}

		void c_utl_buffer::seek_put( seek_type_t type, int offset ) {
			int nNextPut = m_Put;
			switch ( type ) {
			case SEEK_HEAD:
				nNextPut = offset;
				break;

			case SEEK_CURRENT:
				nNextPut += offset;
				break;

			case SEEK_TAIL:
				nNextPut = m_nMaxPut - offset;
				break;
			}

			// Force a write of the data
			// FIXME: We could make this more optimal potentially by writing out
			// the entire buffer if you seek outside the current range

			// NOTE: This call will write and will also seek the file to nNextPut.
			on_put_overflow( -nNextPut - 1 );
			m_Put = nNextPut;

			add_null_termination();
		}

		void c_utl_buffer::activate_byte_swapping( bool bActivate ) {
			m_Byteswap.activate_byte_swapping( bActivate );
		}

		void c_utl_buffer::set_big_endian( bool bigEndian ) {
			m_Byteswap.set_target_big_endian( bigEndian );
		}

		bool c_utl_buffer::is_big_endian( void ) {
			return m_Byteswap.is_target_big_endian();
		}

		//-----------------------------------------------------------------------------
		// null terminate the buffer
		//-----------------------------------------------------------------------------
		void c_utl_buffer::add_null_termination( void ) {
			if ( m_Put > m_nMaxPut ) {
				if ( !is_read_only() && ( ( m_Error & PUT_OVERFLOW ) == 0 ) ) {
					// Add null termination value
					if ( check_put( 1 ) ) {
						m_Memory[m_Put - m_nOffset] = 0;
					} else {
						// Restore the overflow state, it was valid before...
						m_Error &= ~PUT_OVERFLOW;
					}
				}
				m_nMaxPut = m_Put;
			}
		}

		//-----------------------------------------------------------------------------
		// Converts a buffer from a CRLF buffer to a CR buffer (and back)
		// Returns false if no conversion was necessary (and outBuf is left untouched)
		// If the conversion occurs, outBuf will be cleared.
		//-----------------------------------------------------------------------------
		bool c_utl_buffer::convert_crlf( c_utl_buffer& outBuf ) {
			if ( !is_text() || !outBuf.is_text() )
				return false;

			if ( contains_crlf() == outBuf.contains_crlf() )
				return false;

			int nInCount = tell_max_put();

			outBuf.purge();
			outBuf.ensure_capacity( nInCount );

			bool bFromCRLF = contains_crlf();

			// Start reading from the beginning
			int nGet = tell_get();
			int nPut = tell_put();
			int nGetDelta = 0;
			int nPutDelta = 0;

			const char* pBase = (const char*)base();
			int nCurrGet = 0;
			while ( nCurrGet < nInCount ) {
				const char* pCurr = &pBase[nCurrGet];
				if ( bFromCRLF ) {
					const char* pNext = V_strnistr( pCurr, "\r\n", nInCount - nCurrGet );
					if ( !pNext ) {
						outBuf.put( pCurr, nInCount - nCurrGet );
						break;
					}

					int nBytes = (size_t)pNext - (size_t)pCurr;
					outBuf.put( pCurr, nBytes );
					outBuf.put_char( '\n' );
					nCurrGet += nBytes + 2;
					if ( nGet >= nCurrGet - 1 ) {
						--nGetDelta;
					}
					if ( nPut >= nCurrGet - 1 ) {
						--nPutDelta;
					}
				} else {
					const char* pNext = V_strnchr( pCurr, '\n', nInCount - nCurrGet );
					if ( !pNext ) {
						outBuf.put( pCurr, nInCount - nCurrGet );
						break;
					}

					int nBytes = (size_t)pNext - (size_t)pCurr;
					outBuf.put( pCurr, nBytes );
					outBuf.put_char( '\r' );
					outBuf.put_char( '\n' );
					nCurrGet += nBytes + 1;
					if ( nGet >= nCurrGet ) {
						++nGetDelta;
					}
					if ( nPut >= nCurrGet ) {
						++nPutDelta;
					}
				}
			}

			assert( nPut + nPutDelta <= outBuf.tell_max_put() );

			outBuf.seek_get( SEEK_HEAD, nGet + nGetDelta );
			outBuf.seek_put( SEEK_HEAD, nPut + nPutDelta );

			return true;
		}

		//---------------------------------------------------------------------------
		// Implementation of c_utl_inplace_buffer
		//---------------------------------------------------------------------------

		c_utl_inplace_buffer::c_utl_inplace_buffer( int growSize /* = 0 */, int initSize /* = 0 */, int nFlags /* = 0 */ ) :
			c_utl_buffer( growSize, initSize, nFlags ) {
			NULL;
		}

		bool c_utl_inplace_buffer::inplace_get_line_ptr( char** ppszInBufferPtr, int* pnLineLength ) {
			assert( is_text() && !contains_crlf() );

			int nLineLen = peek_line_length();
			if ( nLineLen <= 1 ) {
				seek_get( SEEK_TAIL, 0 );
				return false;
			}

			--nLineLen; // because it accounts for putting a terminating null-character

			char* pszLine = (char*)( void* )( peek_get() );
			seek_get( SEEK_CURRENT, nLineLen );

			// Set the out args
			if ( ppszInBufferPtr )
				*ppszInBufferPtr = pszLine;

			if ( pnLineLength )
				*pnLineLength = nLineLen;

			return true;
		}

		char* c_utl_inplace_buffer::inplace_get_line_ptr( void ) {
			char* pszLine = NULL;
			int nLineLen = 0;

			if ( inplace_get_line_ptr( &pszLine, &nLineLen ) ) {
				assert( nLineLen >= 1 );

				switch ( pszLine[nLineLen - 1] ) {
				case '\n':
				case '\r':
					pszLine[nLineLen - 1] = 0;
					if ( --nLineLen ) {
						switch ( pszLine[nLineLen - 1] ) {
						case '\n':
						case '\r':
							pszLine[nLineLen - 1] = 0;
							break;
						}
					}
					break;

				default:
					assert( pszLine[nLineLen] == 0 );
					break;
				}
			}
			return pszLine;
		}

