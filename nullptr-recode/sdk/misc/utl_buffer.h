//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose: 
//
// $NoKeywords: $
//
// Serialization/unserialization buffer
//=============================================================================//
#pragma once

#include <stdarg.h>
#include "../../helpers/helpers.h"
#include "utl_memory.h"

#pragma warning(disable:4127) //conditional operation is constant
#define IsX360() (0)

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
struct characterset_t;
struct typedescription_t;
struct datamap_t;

class c_bytes_wap {
public:
    c_bytes_wap() {
        // Default behavior sets the target endian to match the machine native endian (no swap).
        set_target_big_endian(is_machine_big_endian());
    }

    //-----------------------------------------------------------------------------
    // Write a single field.
    //-----------------------------------------------------------------------------
    void swap_field_to_target_endian(void* pOutputBuffer, void *pData, typedescription_t *pField);

    //-----------------------------------------------------------------------------
    // Write a block of fields.  Works a bit like the saverestore code.  
    //-----------------------------------------------------------------------------
    void swap_fields_to_target_endian(void *pOutputBuffer, void *pBaseData, datamap_t *pDataMap);

    // Swaps fields for the templated type to the output buffer.
    template<typename T> void swap_fields_to_target_endian(T* pOutputBuffer, void *pBaseData, unsigned int objectCount = 1) {
        for(unsigned int i = 0; i < objectCount; ++i, ++pOutputBuffer) {
            swap_fields_to_target_endian((void*)pOutputBuffer, pBaseData, &T::m_DataMap);
            pBaseData = (uint8_t*)pBaseData + sizeof(T);
        }
    }

    // Swaps fields for the templated type in place.
    template<typename T> void swap_fields_to_target_endian(T* pOutputBuffer, unsigned int objectCount = 1)
    {
        swap_fields_to_target_endian<T>(pOutputBuffer, (void*)pOutputBuffer, objectCount);
    }

    //-----------------------------------------------------------------------------
    // True if the current machine is detected as big endian. 
    // (Endienness is effectively detected at compile time when optimizations are
    // enabled)
    //-----------------------------------------------------------------------------
    static bool is_machine_big_endian() {
        short nIsBigEndian = 1;

        // if we are big endian, the first uint8_t will be a 0, if little endian, it will be a one.
        return (bool)(0 == *(char *)&nIsBigEndian);
    }

    //-----------------------------------------------------------------------------
    // Sets the target uint8_t ordering we are swapping to or from.
    //
    // Braindead Endian Reference:
    //		x86 is LITTLE Endian
    //		PowerPC is BIG Endian
    //-----------------------------------------------------------------------------
    void set_target_big_endian(bool bigEndian) {
        m_bBigEndian = bigEndian;
        m_bSwapBytes = is_machine_big_endian() != bigEndian;
    }

    // Changes target endian
    void flip_target_endian(void) {
        m_bSwapBytes = !m_bSwapBytes;
        m_bBigEndian = !m_bBigEndian;
    }

    // Forces uint8_t swapping state, regardless of endianess
    void activate_byte_swapping(bool bActivate) {
        set_target_big_endian(is_machine_big_endian() != bActivate);
    }

    //-----------------------------------------------------------------------------
    // Returns true if the target machine is the same as this one in endianness.
    //
    // Used to determine when a byteswap needs to take place.
    //-----------------------------------------------------------------------------
    bool is_swapping_bytes(void) {
        return m_bSwapBytes;
    }

    bool is_target_big_endian(void) {
        return m_bBigEndian;
    }

    //-----------------------------------------------------------------------------
    // IsByteSwapped()
    //
    // When supplied with a chunk of input data and a constant or magic number
    // (in native format) determines the endienness of the current machine in
    // relation to the given input data.
    //
    // Returns:
    //		1  if input is the same as nativeConstant.
    //		0  if input is byteswapped relative to nativeConstant.
    //		-1 if input is not the same as nativeConstant and not byteswapped either.
    //
    // ( This is useful for detecting byteswapping in magic numbers in structure 
    // headers for example. )
    //-----------------------------------------------------------------------------
    template<typename T> int source_is_native_endian(T input, T nativeConstant) {
        // If it's the same, it isn't byteswapped:
        if(input == nativeConstant)
            return 1;

        int output;
        low_level_byte_swap<T>(&output, &input);
        if(output == nativeConstant)
            return 0;

        assert(0);		// if we Get here, input is neither a swapped nor unswapped version of nativeConstant.
        return -1;
    }

    //-----------------------------------------------------------------------------
    // Swaps an input buffer full of type T into the given output buffer.
    //
    // Swaps [count] items from the inputBuffer to the outputBuffer.
    // If inputBuffer is omitted or NULL, then it is assumed to be the same as
    // outputBuffer - effectively swapping the contents of the buffer in place.
    //-----------------------------------------------------------------------------
    template<typename T> void swap_buffer(T* outputBuffer, T* inputBuffer = NULL, int count = 1) {
        assert(count >= 0);
        assert(outputBuffer);

        // Fail gracefully in release:
        if(count <= 0 || !outputBuffer)
            return;

        // Optimization for the case when we are swapping in place.
        if(inputBuffer == NULL) {
            inputBuffer = outputBuffer;
        }

        // swap everything in the buffer:
        for(int i = 0; i < count; i++) {
            low_level_byte_swap<T>(&outputBuffer[i], &inputBuffer[i]);
        }
    }

    //-----------------------------------------------------------------------------
    // Swaps an input buffer full of type T into the given output buffer.
    //
    // Swaps [count] items from the inputBuffer to the outputBuffer.
    // If inputBuffer is omitted or NULL, then it is assumed to be the same as
    // outputBuffer - effectively swapping the contents of the buffer in place.
    //-----------------------------------------------------------------------------
    template<typename T> void swap_buffer_to_target_endian(T* outputBuffer, T* inputBuffer = NULL, int count = 1) {
        assert(count >= 0);
        assert(outputBuffer);

        // Fail gracefully in release:
        if(count <= 0 || !outputBuffer)
            return;

        // Optimization for the case when we are swapping in place.
        if(inputBuffer == NULL) {
            inputBuffer = outputBuffer;
        }

        // Are we already the correct endienness? ( or are we swapping 1 uint8_t items? )
        if(!m_bSwapBytes || (sizeof(T) == 1)) {
            // If we were just going to swap in place then return.
            if(!inputBuffer)
                return;

            // Otherwise copy the inputBuffer to the outputBuffer:
            memcpy(outputBuffer, inputBuffer, count * sizeof(T));
            return;

        }

        // swap everything in the buffer:
        for(int i = 0; i < count; i++) {
            low_level_byte_swap<T>(&outputBuffer[i], &inputBuffer[i]);
        }
    }

private:
    //-----------------------------------------------------------------------------
    // The lowest level uint8_t swapping workhorse of doom.  output always contains the 
    // swapped version of input.  ( Doesn't compare machine to target endianness )
    //-----------------------------------------------------------------------------
    template<typename T> static void low_level_byte_swap(T *output, T *input) {
        T temp = *output;
#if defined( _X360 )
        // Intrinsics need the source type to be fixed-point
        DWORD* word = (DWORD*)input;
        switch(sizeof(T)) {
            case 8:
            {
                __storewordbytereverse(*word, 0, &temp);
                __storewordbytereverse(*(word + 1), 4, &temp);
            }
            break;

            case 4:
                __storewordbytereverse(*word, 0, &temp);
                break;

            case 2:
                __storeshortbytereverse(*input, 0, &temp);
                break;

            default:
                assert("Invalid size in c_bytes_wap::LowLevelByteSwap" && 0);
        }
#else
        for(int i = 0; i < sizeof(T); i++) {
            ((unsigned char*)&temp)[i] = ((unsigned char*)input)[sizeof(T) - (i + 1)];
        }
#endif
        memcpy(output, &temp, sizeof(T));
    }

    unsigned int m_bSwapBytes : 1;
    unsigned int m_bBigEndian : 1;
};

//-----------------------------------------------------------------------------
// Description of character conversions for string output
// Here's an example of how to use the macros to define a character conversion
// BEGIN_CHAR_CONVERSION( CStringConversion, '\\' )
//	{ '\n', "n" },
//	{ '\t', "t" }
// END_CHAR_CONVERSION( CStringConversion, '\\' )
//-----------------------------------------------------------------------------
class c_utl_char_conversion {
public:
    struct conversion_array_t {
        char m_nActualChar;
        char *m_pReplacementString;
    };

    c_utl_char_conversion(char nEscapeChar, const char *pDelimiter, int nCount, conversion_array_t *pArray);
    char get_escape_char() const;
    const char* get_delimiter() const;
    int get_delimiter_length() const;

    const char* get_conversion_string(char c) const;
    int get_conversion_length(char c) const;
    int max_conversion_length() const;

    // Finds a conversion for the passed-in string, returns length
    virtual char find_conversion(const char *pString, int *pLength);

protected:
    struct conversion_info_t {
        int m_nLength;
        char *m_pReplacementString;
    };

    char m_nEscapeChar;
    const char *m_pDelimiter;
    int m_nDelimiterLength;
    int m_nCount;
    int m_nMaxConversionLength;
    char m_pList[255];
    conversion_info_t m_pReplacements[255];
};

#define BEGIN_CHAR_CONVERSION( _name, _delimiter, _escapeChar )	\
	static c_utl_char_conversion::conversion_array_t s_pConversionArray ## _name[] = {

#define END_CHAR_CONVERSION( _name, _delimiter, _escapeChar ) \
	}; \
	c_utl_char_conversion _name( _escapeChar, _delimiter, sizeof( s_pConversionArray ## _name ) / sizeof( c_utl_char_conversion::conversion_array_t ), s_pConversionArray ## _name )

#define BEGIN_CUSTOM_CHAR_CONVERSION( _className, _name, _delimiter, _escapeChar ) \
	static c_utl_char_conversion::conversion_array_t s_pConversionArray ## _name[] = {

#define END_CUSTOM_CHAR_CONVERSION( _className, _name, _delimiter, _escapeChar ) \
	}; \
	_className _name( _escapeChar, _delimiter, sizeof( s_pConversionArray ## _name ) / sizeof( c_utl_char_conversion::conversion_array_t ), s_pConversionArray ## _name )

//-----------------------------------------------------------------------------
// Character conversions for C strings
//-----------------------------------------------------------------------------
c_utl_char_conversion *get_cstring_char_conversion();

//-----------------------------------------------------------------------------
// Character conversions for quoted strings, with no escape sequences
//-----------------------------------------------------------------------------
c_utl_char_conversion *get_no_esc_char_conversion();


//-----------------------------------------------------------------------------
// Macro to Set overflow functions easily
//-----------------------------------------------------------------------------
#define SetUtlBufferOverflowFuncs( _get, _put )	\
	set_overflow_funcs( (utl_buffer_overflow_func_t)( _get ), static_cast <utl_buffer_overflow_func_t>( _put ) )


//-----------------------------------------------------------------------------
// Command parsing..
//-----------------------------------------------------------------------------
class c_utl_buffer {
public:
    enum seek_type_t {
        SEEK_HEAD = 0,
        SEEK_CURRENT,
        SEEK_TAIL
    };

    // flags
    enum buffer_flags_t {
        TEXT_BUFFER = 0x1,			// Describes how Get + put work (as strings, or binary)
        EXTERNAL_GROWABLE = 0x2,	// This is used w/ external buffers and causes the utlbuf to switch to reallocatable memory if an overflow happens when Putting.
        CONTAINS_CRLF = 0x4,		// For text buffers only, does this contain \n or \n\r?
        READ_ONLY = 0x8,			// For external buffers; prevents null termination from happening.
        AUTO_TABS_DISABLED = 0x10,	// Used to disable/enable push/pop tabs
    };

    // Overflow functions when a Get or put overflows
    typedef bool (c_utl_buffer::*utl_buffer_overflow_func_t)(int nSize);

    // Constructors for growable + external buffers for serialization/unserialization
    c_utl_buffer(int growSize = 0, int initSize = 0, int nFlags = 0);
    c_utl_buffer(const void* pBuffer, int size, int nFlags = 0);
    // This one isn't actually defined so that we catch contructors that are trying to pass a bool in as the third param.
    c_utl_buffer(const void *pBuffer, int size, bool crap);

    unsigned char	get_flags() const;

    // NOTE: This will assert if you attempt to recast it in a way that
    // is not compatible. The only valid conversion is binary-> text w/CRLF
    void			set_buffer_type(bool bIsText, bool bContainsCRLF);

    // Makes sure we've got at least this much memory
    void			ensure_capacity(int num);

    // Attaches the buffer to external memory....
    void			set_external_buffer(void* pMemory, int nSize, int nInitialPut, int nFlags = 0);
    bool			is_externally_allocated() const;
    void			assume_memory(void *pMemory, int nSize, int nInitialPut, int nFlags = 0);

    __forceinline void activate_byte_swapping_if_big_endian(void) {
        if(IsX360())
            activate_byte_swapping(true);
    }


    // Controls endian-ness of binary utlbufs - default matches the current platform
    void			activate_byte_swapping(bool bActivate);
    void			set_big_endian(bool bigEndian);
    bool			is_big_endian(void);

    // Resets the buffer; but doesn't free memory
    void			clear();

    // Clears out the buffer; frees memory
    void			purge();

    // Read stuff out.
    // Binary mode: it'll just read the bits directly in, and characters will be
    //		read for strings until a null character is reached.
    // Text mode: it'll parse the file, turning text #s into real numbers.
    //		get_string will read a string until a space is reached
    char			get_char();
    unsigned char	get_unsigned_char();
    short			get_short();
    unsigned short	get_unsigned_short();
    int				get_int();
    int				get_int_hex();
    unsigned int	get_unsigned_int();
    float			get_float();
    double			get_double();
    void			get_string(char* pString, int nMaxChars = 0);
    void			get(void* pMem, int size);
    void			get_line(char* pLine, int nMaxChars = 0);

    // Used for getting objects that have a byteswap datadesc defined
    template <typename T> void get_objects(T *dest, int count = 1);

    // This will Get at least 1 uint8_t and up to nSize bytes. 
    // It will return the number of bytes actually read.
    int				get_up_to(void *pMem, int nSize);

    // This version of get_string converts \" to \\ and " to \, etc.
    // It also reads a " at the beginning and end of the string
    void			get_delimited_string(c_utl_char_conversion *pConv, char *pString, int nMaxChars = 0);
    char			get_delimited_char(c_utl_char_conversion *pConv);

    // This will return the # of characters of the string about to be read out
    // NOTE: The count will *include* the terminating 0!!
    // In binary mode, it's the number of characters until the next 0
    // In text mode, it's the number of characters until the next space.
    int				peek_string_length();

    // This version of PeekStringLength converts \" to \\ and " to \, etc.
    // It also reads a " at the beginning and end of the string
    // NOTE: The count will *include* the terminating 0!!
    // In binary mode, it's the number of characters until the next 0
    // In text mode, it's the number of characters between "s (checking for \")
    // Specifying false for bActualSize will return the pre-translated number of characters
    // including the delimiters and the escape characters. So, \n counts as 2 characters when bActualSize == false
    // and only 1 character when bActualSize == true
    int				peek_delimited_string_length(c_utl_char_conversion *pConv, bool bActualSize = true);

    // Just like scanf, but doesn't work in binary mode
    int				scanf(const char* pFmt, ...);
    int				vascanf(const char* pFmt, va_list list);

    // Eats white space, advances Get index
    void			eat_white_space();

    // Eats C++ style comments
    bool			eat_cpp_comment();

    // (For text buffers only)
    // Parse a token from the buffer:
    // Grab all text that lies between a starting delimiter + ending delimiter
    // (skipping whitespace that leads + trails both delimiters).
    // If successful, the Get index is advanced and the function returns true,
    // otherwise the index is not advanced and the function returns false.
    bool			parse_token(const char *pStartingDelim, const char *pEndingDelim, char* pString, int nMaxLen);

    // Advance the Get index until after the particular string is found
    // Do not eat whitespace before starting. Return false if it failed
    // String test is case-insensitive.
    bool			get_token(const char *pToken);

    // Parses the next token, given a Set of character breaks to stop at
    // Returns the length of the token parsed in bytes (-1 if none parsed)
    int				parse_token(characterset_t *pBreaks, char *pTokenBuf, int nMaxLen, bool bParseComments = true);

    // Write stuff in
    // Binary mode: it'll just write the bits directly in, and strings will be
    //		written with a null terminating character
    // Text mode: it'll convert the numbers to text versions
    //		put_string will not write a terminating character
    void			put_char(char c);
    void			put_unsigned_char(unsigned char uc);
    void			put_short(short s);
    void			put_unsigned_short(unsigned short us);
    void			put_int(int i);
    void			put_unsigned_int(unsigned int u);
    void			put_float(float f);
    void			put_double(double d);
    void			put_string(const char* pString);
    void			put(const void* pMem, int size);

    // Used for putting objects that have a byteswap datadesc defined
    template <typename T> void put_objects(T *src, int count = 1);

    // This version of put_string converts \ to \\ and " to \", etc.
    // It also places " at the beginning and end of the string
    void			put_delimited_string(c_utl_char_conversion *pConv, const char *pString);
    void			put_delimited_char(c_utl_char_conversion *pConv, char c);

    // Just like printf, writes a terminating zero in binary mode
    void			printf(const char* pFmt, ...);
    void			vaprintf(const char* pFmt, va_list list);

    // What am I writing (put)/reading (Get)?
    void* peek_put(int offset = 0);
    const void* peek_get(int offset = 0) const;
    const void* peek_get(int nMaxSize, int nOffset);

    // Where am I writing (put)/reading (Get)?
    int tell_put() const;
    int tell_get() const;

    // What's the most I've ever written?
    int tell_max_put() const;

    // How many bytes remain to be read?
    // NOTE: This is not accurate for streaming text files; it overshoots
    int get_bytes_remaining() const;

    // Change where I'm writing (put)/reading (Get)
    void seek_put(seek_type_t type, int offset);
    void seek_get(seek_type_t type, int offset);

    // Buffer base
    const void* base() const;
    void* base();

    // memory allocation size, does *not* reflect size written or read,
    //	use TellPut or tell_get for that
    int size() const;

    // Am I a text buffer?
    bool is_text() const;

    // Can I grow if I'm externally allocated?
    bool is_growable() const;

    // Am I valid? (overflow or underflow error), Once invalid it stays invalid
    bool is_valid() const;

    // Do I contain carriage return/linefeeds? 
    bool contains_crlf() const;

    // Am I read-only
    bool is_read_only() const;

    // Converts a buffer from a CRLF buffer to a CR buffer (and back)
    // Returns false if no conversion was necessary (and outBuf is left untouched)
    // If the conversion occurs, outBuf will be cleared.
    bool convert_crlf(c_utl_buffer &outBuf);

    // Push/pop pretty-printing tabs
    void push_tab();
    void pop_tab();

    // Temporarily disables pretty print
    void enable_tabs(bool bEnable);

protected:
    // error flags
    enum {
        PUT_OVERFLOW = 0x1,
        GET_OVERFLOW = 0x2,
        MAX_ERROR_FLAG = GET_OVERFLOW,
    };

    void set_overflow_funcs(utl_buffer_overflow_func_t getFunc, utl_buffer_overflow_func_t putFunc);

    bool on_put_overflow(int nSize);
    bool on_get_overflow(int nSize);

protected:
    // Checks if a Get/put is ok
    bool check_put(int size);
    bool check_get(int size);

    void add_null_termination();

    // Methods to help with pretty-printing
    bool was_last_character_cr();
    void put_tabs();

    // Help with delimited stuff
    char get_delimited_char_internal(c_utl_char_conversion *pConv);
    void put_delimited_char_internal(c_utl_char_conversion *pConv, char c);

    // Default overflow funcs
    bool put_overflow(int nSize);
    bool get_overflow(int nSize);

    // Does the next bytes of the buffer match a pattern?
    bool peek_string_match(int nOffset, const char *pString, int nLen);

    // Peek size of line to come, check memory bound
    int	peek_line_length();

    // How much whitespace should I skip?
    int peek_white_space(int nOffset);

    // Checks if a peek Get is ok
    bool check_peek_get(int nOffset, int nSize);

    // Call this to peek arbitrarily long into memory. It doesn't fail unless
    // it can't read *anything* new
    bool check_arbitrary_peek_get(int nOffset, int &nIncrement);

    template <typename T> void get_type(T& dest, const char *pszFmt);
    template <typename T> void get_type_bin(T& dest);
    template <typename T> void get_object(T *src);

    template <typename T> void put_type(T src, const char *pszFmt);
    template <typename T> void put_type_bin(T src);
    template <typename T> void put_object(T *src);

    c_utl_memory<unsigned char> m_Memory;
    int m_Get;
    int m_Put;

    unsigned char m_Error;
    unsigned char m_Flags;
    unsigned char m_Reserved;
#if defined( _X360 )
    unsigned char pad;
#endif

    int m_nTab;
    int m_nMaxPut;
    int m_nOffset;

    utl_buffer_overflow_func_t m_GetOverflowFunc;
    utl_buffer_overflow_func_t m_PutOverflowFunc;

    c_bytes_wap	m_Byteswap;
};


// Stream style output operators for c_utl_buffer
c_utl_buffer &operator<<(c_utl_buffer &b, char v) {
    b.put_char(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, unsigned char v) {
    b.put_unsigned_char(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, short v) {
    b.put_short(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, unsigned short v) {
    b.put_unsigned_short(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, int v) {
    b.put_int(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, unsigned int v) {
    b.put_unsigned_int(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, float v) {
    b.put_float(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, double v) {
    b.put_double(v);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, const char *pv) {
    b.put_string(pv);
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, const vec3 &v) {
    b << v.x << " " << v.y << " " << v.z;
    return b;
}

c_utl_buffer &operator<<(c_utl_buffer &b, const vec2 &v) {
    b << v.x << " " << v.y;
    return b;
}


class c_utl_inplace_buffer : public c_utl_buffer {
public:
    c_utl_inplace_buffer(int growSize = 0, int initSize = 0, int nFlags = 0);

    //
    // Routines returning buffer-inplace-pointers
    //
public:
    //
    // Upon success, determines the line length, fills out the pointer to the
    // beginning of the line and the line length, advances the "Get" pointer
    // offset by the line length and returns "true".
    //
    // If end of file is reached or upon error returns "false".
    //
    // Note:	the returned length of the line is at least one character because the
    //			trailing newline characters are also included as part of the line.
    //
    // Note:	the pointer returned points into the local memory of this buffer, in
    //			case the buffer gets relocated or destroyed the pointer becomes invalid.
    //
    // e.g.:	-------------
    //
    //			char *pszLine;
    //			int nLineLen;
    //			while ( pUtlInplaceBuffer->inplace_get_line_ptr( &pszLine, &nLineLen ) )
    //			{
    //				...
    //			}
    //
    //			-------------
    //
    // @param	ppszInBufferPtr		on return points into this buffer at start of line
    // @param	pnLineLength		on return holds num bytes accessible via (*ppszInBufferPtr)
    //
    // @returns	true				if line was successfully read
    //			false				when EOF is reached or error occurs
    //
    bool inplace_get_line_ptr( /* out */ char **ppszInBufferPtr, /* out */ int *pnLineLength);

    //
    // Determines the line length, advances the "Get" pointer offset by the line length,
    // replaces the newline character with null-terminator and returns the initial pointer
    // to now null-terminated line.
    //
    // If end of file is reached or upon error returns NULL.
    //
    // Note:	the pointer returned points into the local memory of this buffer, in
    //			case the buffer gets relocated or destroyed the pointer becomes invalid.
    //
    // e.g.:	-------------
    //
    //			while ( char *pszLine = pUtlInplaceBuffer->inplace_get_line_ptr() )
    //			{
    //				...
    //			}
    //
    //			-------------
    //
    // @returns	ptr-to-zero-terminated-line		if line was successfully read and buffer modified
    //			NULL							when EOF is reached or error occurs
    //
    char * inplace_get_line_ptr(void);
};


//-----------------------------------------------------------------------------
// Where am I reading?
//-----------------------------------------------------------------------------
int c_utl_buffer::tell_get() const {
    return m_Get;
}


//-----------------------------------------------------------------------------
// How many bytes remain to be read?
//-----------------------------------------------------------------------------
int c_utl_buffer::get_bytes_remaining() const {
    return m_nMaxPut - tell_get();
}


//-----------------------------------------------------------------------------
// What am I reading?
//-----------------------------------------------------------------------------
const void* c_utl_buffer::peek_get(int offset) const {
    return &m_Memory[m_Get + offset - m_nOffset];
}


//-----------------------------------------------------------------------------
// Unserialization
//-----------------------------------------------------------------------------

template <typename T>
void c_utl_buffer::get_object(T *dest) {
    if(check_get(sizeof(T))) {
        if(!m_Byteswap.is_swapping_bytes() || (sizeof(T) == 1)) {
            *dest = *(T *)PeekGet();
        } else {
            m_Byteswap.swap_fields_to_target_endian<T>(dest, (T*)peek_get());
        }
        m_Get += sizeof(T);
    } else {
        Q_memset(&dest, 0, sizeof(T));
    }
}


template <typename T>
void c_utl_buffer::get_objects(T *dest, int count) {
    for(int i = 0; i < count; ++i, ++dest) {
        GetObject<T>(dest);
    }
}


template <typename T>
void c_utl_buffer::get_type_bin(T &dest) {
    if(check_get(sizeof(T))) {
        if(!m_Byteswap.is_swapping_bytes() || (sizeof(T) == 1)) {
            dest = *(T *)peek_get();
        } else {
            m_Byteswap.swap_buffer_to_target_endian<T>(&dest, (T*)peek_get());
        }
        m_Get += sizeof(T);
    } else {
        dest = 0;
    }
}

template <>
void c_utl_buffer::get_type_bin< float >(float &dest) {
    if(check_get(sizeof(float))) {
        unsigned int pData = (unsigned int)peek_get();
        if(IsX360() && (pData & 0x03)) {
            // handle unaligned read
            ((unsigned char*)&dest)[0] = ((unsigned char*)pData)[0];
            ((unsigned char*)&dest)[1] = ((unsigned char*)pData)[1];
            ((unsigned char*)&dest)[2] = ((unsigned char*)pData)[2];
            ((unsigned char*)&dest)[3] = ((unsigned char*)pData)[3];
        } else {
            // aligned read
            dest = *(float *)pData;
        }
        if(m_Byteswap.is_swapping_bytes()) {
            m_Byteswap.swap_buffer_to_target_endian< float >(&dest, &dest);
        }
        m_Get += sizeof(float);
    } else {
        dest = 0;
    }
}

template <typename T>
void c_utl_buffer::get_type(T &dest, const char *pszFmt) {
    if(!is_text()) {
        get_type_bin(dest);
    } else {
        dest = 0;
        scanf(pszFmt, &dest);
    }
}

char c_utl_buffer::get_char() {
    char c;
    get_type(c, "%c");
    return c;
}

unsigned char c_utl_buffer::get_unsigned_char() {
    unsigned char c;
    get_type(c, "%u");
    return c;
}

short c_utl_buffer::get_short() {
    short s;
    get_type(s, "%d");
    return s;
}

unsigned short c_utl_buffer::get_unsigned_short() {
    unsigned short s;
    get_type(s, "%u");
    return s;
}

int c_utl_buffer::get_int() {
    int i;
    get_type(i, "%d");
    return i;
}

int c_utl_buffer::get_int_hex() {
    int i;
    get_type(i, "%x");
    return i;
}

unsigned int c_utl_buffer::get_unsigned_int() {
    unsigned int u;
    get_type(u, "%u");
    return u;
}

float c_utl_buffer::get_float() {
    float f;
    get_type(f, "%f");
    return f;
}

double c_utl_buffer::get_double() {
    double d;
    get_type(d, "%f");
    return d;
}


//-----------------------------------------------------------------------------
// Where am I writing?
//-----------------------------------------------------------------------------
unsigned char c_utl_buffer::get_flags() const {
    return m_Flags;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool c_utl_buffer::is_externally_allocated() const {
    return m_Memory.is_externally_allocated();
}


//-----------------------------------------------------------------------------
// Where am I writing?
//-----------------------------------------------------------------------------
int c_utl_buffer::tell_put() const {
    return m_Put;
}


//-----------------------------------------------------------------------------
// What's the most I've ever written?
//-----------------------------------------------------------------------------
int c_utl_buffer::tell_max_put() const {
    return m_nMaxPut;
}


//-----------------------------------------------------------------------------
// What am I reading?
//-----------------------------------------------------------------------------
void* c_utl_buffer::peek_put(int offset) {
    return &m_Memory[m_Put + offset - m_nOffset];
}


//-----------------------------------------------------------------------------
// Various put methods
//-----------------------------------------------------------------------------

template <typename T>
void c_utl_buffer::put_object(T *src) {
    if(check_put(sizeof(T))) {
        if(!m_Byteswap.is_swapping_bytes() || (sizeof(T) == 1)) {
            *(T *)peek_put() = *src;
        } else {
            m_Byteswap.swap_fields_to_target_endian<T>((T*)peek_put(), src);
        }
        m_Put += sizeof(T);
        add_null_termination();
    }
}


template <typename T>
void c_utl_buffer::put_objects(T *src, int count) {
    for(int i = 0; i < count; ++i, ++src) {
        put_object<T>(src);
    }
}


template <typename T>
void c_utl_buffer::put_type_bin(T src) {
    if(check_put(sizeof(T))) {
        if(!m_Byteswap.is_swapping_bytes() || (sizeof(T) == 1)) {
            *(T *)peek_put() = src;
        } else {
            m_Byteswap.swap_buffer_to_target_endian<T>((T*)peek_put(), &src);
        }
        m_Put += sizeof(T);
        add_null_termination();
    }
}

template <typename T>
void c_utl_buffer::put_type(T src, const char *pszFmt) {
    if(!is_text()) {
        put_type_bin(src);
    } else {
        printf(pszFmt, src);
    }
}

//-----------------------------------------------------------------------------
// Methods to help with pretty-printing
//-----------------------------------------------------------------------------
bool c_utl_buffer::was_last_character_cr() {
    if(!is_text() || (tell_put() == 0))
        return false;
    return (*(const char *)peek_put(-1) == '\n');
}

void c_utl_buffer::put_tabs() {
    int nTabCount = (m_Flags & AUTO_TABS_DISABLED) ? 0 : m_nTab;
    for(int i = nTabCount; --i >= 0; ) {
        put_type_bin<char>('\t');
    }
}


//-----------------------------------------------------------------------------
// Push/pop pretty-printing tabs
//-----------------------------------------------------------------------------
void c_utl_buffer::push_tab() {
    ++m_nTab;
}

void c_utl_buffer::pop_tab() {
    if(--m_nTab < 0) {
        m_nTab = 0;
    }
}


//-----------------------------------------------------------------------------
// Temporarily disables pretty print
//-----------------------------------------------------------------------------
void c_utl_buffer::enable_tabs(bool bEnable) {
    if(bEnable) {
        m_Flags &= ~AUTO_TABS_DISABLED;
    } else {
        m_Flags |= AUTO_TABS_DISABLED;
    }
}

void c_utl_buffer::put_char(char c) {
    if(was_last_character_cr()) {
        put_tabs();
    }

    put_type_bin(c);
}

void c_utl_buffer::put_unsigned_char(unsigned char c) {
    put_type(c, "%u");
}

void  c_utl_buffer::put_short(short s) {
    put_type(s, "%d");
}

void c_utl_buffer::put_unsigned_short(unsigned short s) {
    put_type(s, "%u");
}

void c_utl_buffer::put_int(int i ) {
    put_type(i, "%d");
}

void c_utl_buffer::put_unsigned_int(unsigned int u) {
    put_type(u, "%u");
}

void c_utl_buffer::put_float(float f) {
    put_type(f, "%f");
}

void c_utl_buffer::put_double(double d) {
    put_type(d, "%f");
}


//-----------------------------------------------------------------------------
// Am I a text buffer?
//-----------------------------------------------------------------------------
bool c_utl_buffer::is_text() const {
    return (m_Flags & TEXT_BUFFER) != 0;
}


//-----------------------------------------------------------------------------
// Can I grow if I'm externally allocated?
//-----------------------------------------------------------------------------
bool c_utl_buffer::is_growable() const {
    return (m_Flags & EXTERNAL_GROWABLE) != 0;
}


//-----------------------------------------------------------------------------
// Am I valid? (overflow or underflow error), Once invalid it stays invalid
//-----------------------------------------------------------------------------
bool c_utl_buffer::is_valid() const {
    return m_Error == 0;
}


//-----------------------------------------------------------------------------
// Do I contain carriage return/linefeeds? 
//-----------------------------------------------------------------------------
bool c_utl_buffer::contains_crlf() const {
    return is_text() && ((m_Flags & CONTAINS_CRLF) != 0);
}


//-----------------------------------------------------------------------------
// Am I read-only
//-----------------------------------------------------------------------------
bool c_utl_buffer::is_read_only() const {
    return (m_Flags & READ_ONLY) != 0;
}


//-----------------------------------------------------------------------------
// Buffer base and size
//-----------------------------------------------------------------------------
const void* c_utl_buffer::base() const {
    return m_Memory.base();
}

void* c_utl_buffer::base() {
    return m_Memory.base();
}

int c_utl_buffer::size() const {
    return m_Memory.num_allocated();
}


//-----------------------------------------------------------------------------
// Clears out the buffer; frees memory
//-----------------------------------------------------------------------------
void c_utl_buffer::clear() {
    m_Get = 0;
    m_Put = 0;
    m_Error = 0;
    m_nOffset = 0;
    m_nMaxPut = -1;
    add_null_termination();
}

void c_utl_buffer::purge() {
    m_Get = 0;
    m_Put = 0;
    m_nOffset = 0;
    m_nMaxPut = 0;
    m_Error = 0;
    m_Memory.purge();
}
#pragma warning(default:4127) //conditional operation is constant