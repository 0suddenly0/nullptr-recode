#pragma once

#include <cstdint>
#include <cstring>

#include "utl_memory.h"

class c_utl_binary_block {
public:
    c_utl_binary_block(int growSize = 0, int initSize = 0);

    // NOTE: nInitialLength indicates how much of the buffer starts full
    c_utl_binary_block(void* pMemory, int nSizeInBytes, int nInitialLength);
    c_utl_binary_block(const void* pMemory, int nSizeInBytes);
    c_utl_binary_block(const c_utl_binary_block& src);

    void        get(void *pValue, int nMaxLen) const;
    void        set(const void *pValue, int nLen);
    const void  *get() const;
    void        *get();

    unsigned char& operator[](int i);
    const unsigned char& operator[](int i) const;

    int         length() const;
    void        set_length(int nLength);    // Undefined memory will result
    bool        is_empty() const;
    void        clear();
    void        purge();

    bool        is_read_only() const;

    c_utl_binary_block &operator=(const c_utl_binary_block &src);

    // Test for equality
    bool operator==(const c_utl_binary_block &src) const;

private:
    c_utl_memory<unsigned char> m_Memory;
    int m_nActualLength;
};


//-----------------------------------------------------------------------------
// class inlines
//-----------------------------------------------------------------------------
const void *c_utl_binary_block::get() const {
    return m_Memory.base();
}

void *c_utl_binary_block::get() {
    return m_Memory.base();
}

int c_utl_binary_block::length() const {
    return m_nActualLength;
}

unsigned char& c_utl_binary_block::operator[](int i) {
    return m_Memory[i];
}

const unsigned char& c_utl_binary_block::operator[](int i) const {
    return m_Memory[i];
}

bool c_utl_binary_block::is_read_only() const {
    return m_Memory.is_read_only();
}

bool c_utl_binary_block::is_empty() const {
    return length() == 0;
}

void c_utl_binary_block::clear() {
    set_length(0);
}

void c_utl_binary_block::purge() {
    set_length(0);
    m_Memory.purge();
}

//-----------------------------------------------------------------------------
// Simple string class. 
// NOTE: This is *not* optimal! Use in tools, but not runtime code
//-----------------------------------------------------------------------------
class c_utl_string {
public:
    c_utl_string();
    c_utl_string(const char *pString);
    c_utl_string(const c_utl_string& string);

    // Attaches the string to external memory. Useful for avoiding a copy
    c_utl_string(void* pMemory, int nSizeInBytes, int nInitialLength);
    c_utl_string(const void* pMemory, int nSizeInBytes);

    const char    *get() const;
    void        set(const char *pValue);

    // Set directly and don't look for a null terminator in pValue.
    void        set_direct(const char *pValue, int nChars);

    // Converts to c-strings
    operator const char*() const;

    // for compatibility switching items from UtlSymbol
    const char  *string() const { return get(); }

    // Returns strlen
    int            length() const;
    bool        is_empty() const;

    // Sets the length (used to serialize into the buffer )
    // Note: If nLen != 0, then this adds an extra uint8_t for a null-terminator.    
    void        set_length(int nLen);
    char        *get();
    void        clear();
    void        purge();

    // Strips the trailing slash
    void        strip_trailing_slash();

    c_utl_string &operator=(const c_utl_string &src);
    c_utl_string &operator=(const char *src);

    // Test for equality
    bool operator==(const c_utl_string &src) const;
    bool operator==(const char *src) const;
    bool operator!=(const c_utl_string &src) const { return !operator==(src); }
    bool operator!=(const char *src) const { return !operator==(src); }

    c_utl_string &operator+=(const c_utl_string &rhs);
    c_utl_string &operator+=(const char *rhs);
    c_utl_string &operator+=(char c);
    c_utl_string &operator+=(int rhs);
    c_utl_string &operator+=(double rhs);

    c_utl_string operator+(const char *pOther);
    c_utl_string operator+(int rhs);

    int format(const char *pFormat, ...);

    // Take a piece out of the string.
    // If you only specify nStart, it'll go from nStart to the end.
    // You can use negative numbers and it'll wrap around to the start.
    c_utl_string  slice(int32_t nStart = 0, int32_t nEnd = INT32_MAX);

    // Grab a substring starting from the left or the right side.
    c_utl_string left(int32_t nChars);
    c_utl_string right(int32_t nChars);

    // replace all instances of one character with another.
    c_utl_string replace(char cFrom, char cTo);

    // Calls right through to V_MakeAbsolutePath.
    c_utl_string abs_path(const char *pStartingDir = NULL);

    // Gets the filename (everything except the path.. c:\a\b\c\somefile.txt -> somefile.txt).
    c_utl_string unqualified_file_name();

    // Strips off one directory. Uses V_StripLastDir but strips the last slash also!
    c_utl_string dir_name();

    // Works like V_ComposeFileName.
    static c_utl_string path_join(const char *pStr1, const char *pStr2);

    // These can be used for utlvector sorts.
    static int __cdecl sort_case_insensitive(const c_utl_string *pString1, const c_utl_string *pString2);
    static int __cdecl sort_case_sensitive(const c_utl_string *pString1, const c_utl_string *pString2);

private:
    c_utl_binary_block m_Storage;
};


//-----------------------------------------------------------------------------
// methods
//-----------------------------------------------------------------------------
bool c_utl_string::is_empty() const {
    return length() == 0;
}

int __cdecl c_utl_string::sort_case_insensitive(const c_utl_string *pString1, const c_utl_string *pString2) {
    return _stricmp(pString1->string(), pString2->string());
}

int __cdecl c_utl_string::sort_case_sensitive(const c_utl_string *pString1, const c_utl_string *pString2) {
    return strcmp(pString1->string(), pString2->string());
}
