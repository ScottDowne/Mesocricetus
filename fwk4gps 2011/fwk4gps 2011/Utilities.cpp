/* Utilities Implementation
 *
 * iUtilities.cpp
 * fwk4gps version 2.0
 * gam666/dps901
 * October 4 2011
 * copyright (c) 2011 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include <fstream>      // for ostream, <<, close()
#include "iWindow.h"    // for the Window Interface
#include "iUtilities.h" // for the Utility Interface

//---------------------------------- Utilities --------------------------------
//
// The Utilities module supports the Framework
//
// logError adds error message msg to the error log file
//
void logError(const wchar_t* msg) {

    std::wofstream fp("error.log", std::ios::app);
    if (fp) {
         fp << msg << std::endl;
         fp.close();
    }
}

// error pops up a Message Box displaying msg and adds the
// message to the log file
//
void error(const wchar_t* msg, const wchar_t* more) {

    int len = strlen(msg);
    if (more) len += strlen(more);
    wchar_t* str = new wchar_t[len + 1];
	strcpy(str, msg, len);
	if (more) strcat(str, more, len);

	WindowAddress()->messageBox(str); 

    logError(str);

    delete [] str;
}

// strlen returns the length of str
//
unsigned strlen(const wchar_t* str) {

    unsigned len = 0u;
    while (*str++) len++;
	return len;
}

// strcmp compares string a and string b
//
int strcmp(const wchar_t* a, const wchar_t* b) { 

	int diff = 0;
    while (*a && *b && !(diff = *a++ - *b++));
    return diff;
}

// strcat catenates src to dest[sizeDest+1] without overflow and
// returns the address of dest[]
// this function is a safe version of strcat
//
wchar_t* strcat(wchar_t* dest, const wchar_t* str, int sizeDest) {

    wchar_t* rc = dest;
    *(dest + sizeDest) = '\0';
    while (*dest++) sizeDest--;
    dest--;
    while (sizeDest-- && (*dest++ = *str++)) ;

    return rc;
}

// strcpy copies src into dest[sizeDest+1] without overflow and
// returns the address of dest[]
// this function is a safe version of strcpy and a version of
// strncpy that does not require last character padding with a null
// byte
//
wchar_t* strcpy(wchar_t* dest, const wchar_t* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = *src++)) ;

    return dest;
}

// strcpyFromMB copies multi-byte src into wide character dest[sizeDest+1] 
// without overflow and returns the address of dest[]
//
wchar_t* strcpyFromMB(wchar_t* dest, const char* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = *src++)) ;

    return dest;
}

// strcpyFromWC copies wide character src into multi-byte dest[sizeDest+1] 
// without overflow and returns the address of dest[]
//
char* strcpyFromWC(char* dest, const wchar_t* src, int sizeDest) {

    *(dest + sizeDest) = '\0';
    while (sizeDest-- && (*dest++ = char(*src++))) ;

    return dest;
}

// sprintf prints a, b, c into str
//
int sprintf(wchar_t* str, int a, int b, int c) {

    wchar_t s[11];
    
    strcpy(str, itowc(s, a), 10);
    strcat(str, L", ", 12);
    strcat(str, itowc(s, b), 22);
    strcat(str, L", ", 24);
    strcat(str, itowc(s, c), 34);

	return 0;
}

// sprintf prints integer a and a suffix string into str
//
int sprintf(wchar_t* str, int a, const wchar_t* suffix) {

    wchar_t s[11];

    strcpy(str, L" ", 1);
    strcat(str, itowc(s, a), 11);
    strcat(str, suffix, strlen(suffix) + 11);

    return 0;
}

// nameWithDir adds directory to filename to create nameWithDir
//
wchar_t* nameWithDir(wchar_t* nameWithDir, const wchar_t* directory, 
 const wchar_t* filename, int sizeDest) {

	strcpy(nameWithDir, directory, sizeDest);
	strcat(nameWithDir, L"\\", sizeDest);
	strcat(nameWithDir, filename, sizeDest);

	return nameWithDir;
}

