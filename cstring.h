/** C++ std::string clone : CSTRING
 *
 *  This attempts to mimic the std::string class found in C++ implementation of string.h
 *  cstring stores pointers to functions that act almost identically to those of its
 *  C++ counterpart and can be accessed like a C++ class using (*) or -> dereferences
 *  and accessing the function members of the cstring struct.
 *
 *  All functions require a reference to the cstring and should be called by passing
 *  the pointer to itself as the first parameter.
 *
 *  E.G. cstring->data(cstring) // returns a const char * to the content of cstring
 *       cstring->find(cstring,"testing",0) // returns the position of the first occurence of "testing"
 *
 *  NOTE:
 *  CSTRING is not meant to be a memory efficient form of a string implementation
 *  this implementation is designed for ease-of-use over efficiency.
 *
 *  Written by Mark Zammit 26/02/2013
 *  Copyright (c) 2013 Mark Zammit. All rights reserved.
 */


/* EXAMPLES OF USE
 *
 * cstring * str = string("this is a cstring");
 * printf("%s str->data()\n", str->data(str));
 * printf("'a' can be found at %d in str->find()\n", str->find(str,"a",0", NULL));
 * printf("%s is %d characters long\n", str->data(str), str->length(str));
 * printf("str can't be longer than %d characters on your processor\n",
 *        str->max_size(str));
 * str->clear(str);
 * printf("str is now clear? %d\n", str->empty(str));
 * delete_string(str);
 *
 */

/** NOTE: Need to add UNICODE support */

#ifndef CSTRING_H
#define CSTRING_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef ITERATOR
#define ITERATOR

#define ITR_FORWARD             0
#define ITR_BI_DIRECTIONAL      1
#define ITR_REVERSE             2
#define ITR_R_BI_DIRECTIONAL    3

#endif

typedef struct _cstring_          cstring;
typedef struct _cstr_           * cstr;
typedef struct _base_iterator_  * base_iterator;
typedef struct _cstr_iterator_  * cstr_iterator;

static const long int npos = LONG_MAX;


struct _cstr_iterator_
{
    // inherits from base_iterator
    base_iterator base;

    // Inheritance reference
    const base_iterator (*ref)  (cstr_iterator);

    // Allocator
    const void      (*set)  (cstr_iterator, char *t);

    // Dereference of pointer value
          char *    (*val)  (cstr_iterator);

    // Modifiers
    const void      (*inc)  (cstr_iterator);
    const void      (*dec)  (cstr_iterator);

    // Directional Category
    const short     (*cat)  (cstr_iterator);
};

cstr_iterator cstr_itr(const short category);


// Initializes a new cstring and allocates function pointers
// should no string be required at time of init then "" should be passed
// passing a value of NULL will be treated effectively as ""
cstring *   string(const char * init_str);

// Frees up the memory allocations for the cstring and allocates it to NULL
// calls to cstring functions should not be found after this, runtime errors
// will result if attempts are made
const void  delete_string(cstring * );

// CSTRING INTERFACE
struct _cstring_
{
    cstr str;   // pointer to a string data type

    /* Modifiers */

    // Adds a string sequence to the end of the current string
    const   void        (*append)           (cstring * this, const char * str);

    // Adds a character to the end of the string
    const   void        (*push_back)        (cstring * this, const char chr);

    // Erases the last character at the end of the string
    const   void        (*pop_back)         (cstring * this);

    // Changes the string value to a new value
    const   void        (*assign)           (cstring * this, const char * str);

    // Inserts a string sequence into the string at any position
    // the sequence are inserted to the lhs of that position, any character
    // in that position will begin on the rhs of the the inserted string
    const   void        (*insert)           (cstring * this, size_t pos, const char * str);

    // Erases a sequence of strings from the current string, a position
    // must be indicated at which to start from and the length of the erase
    const   void        (*erase)            (cstring * this, size_t pos, size_t len);

    // Swaps the contents of 2 cstring types
    const   void        (*swap)             (cstring * this, cstring * str);

    /* Element Access */
    // Retrieves the character at an element position, positions start at 0
    const   char        (*at)               (cstring * this, size_t pos);

    // Retrieves the last character in the string
    const   char        (*back)             (cstring * this);

    // Retrieves the first character in the string
    const   char        (*front)            (cstring * this);


    /* String Operations */

    // Converts the string into a const char pointer
    const   char *      (*data)                 (cstring * this);

    // Copies the contents from a starting elemenet for a given length
    // into a buffer which is dynamically allocated memory, anything stored in the
    // buffer previously is erased before use
    const   size_t      (*copy)                 (cstring * this, char ** buf, size_t pos, size_t len);

    // Searches the string for the first occurrence of the sequence specified by its arguments
    // All characters of the search string must be found exactly or 'npos' is returned
    // When 'pos' is specified it will begin its search from that position, a pointer to a
    // size_t data type is optional and will store the position of the character proceeding result
    const   size_t      (*find)                 (cstring * this, const char * str, size_t * nxt_pos);

    // Searches the string for the first character that matches ANY of the
    // characters specified in its arguments and returns the first match.
    // A starting position can be specified in which the search will begin from
    const   size_t      (*find_first_of)        (cstring * this, const char * str, size_t pos);

    // Searches the string for the last character that matches ANY of the
    // characters specified in its arguments and returns the first match.
    // A starting position can be specified in which the search will begin from
    const   size_t      (*find_last_of)         (cstring * this, const char * str, size_t pos);

    // Searches the string for the first character that doesn't matches ANY of the
    // characters specified in its arguments and returns the first match.
    // A starting position can be specified in which the search will begin from
    const   size_t      (*find_first_not_of)    (cstring * this, const char * str, size_t pos);

    // Searches the string for the last character that doesn't matches ANY of the
    // characters specified in its arguments and returns the first match.
    // A starting position can be specified in which the search will begin from
    const   size_t      (*find_last_not_of)     (cstring * this, const char * str, size_t pos);

    // Returns a const char pointer to a string that is a subset of the cstring
    // this is a copy of the subset not a pointer to the subset in the cstring
    // A position must be specified at which the subset begins and the length to splice
    const   char *      (*substr)               (cstring * this, size_t pos, size_t len);

    // Compares the cstring contents with that of another string, returns true or false
    // strings must be an identical match (case sensitive)
    const   bool        (*compare)              (cstring * this, const char * str);


    /* Capacity */

    // String length excluding null terminator
    const   size_t      (*length)           (cstring * this);

    // Max possible size string could be (this is processor specific)
    const   size_t      (*max_size)         (cstring * this);

    // ** needs editing for smaller sizes
    // Resizes the string by appending null characters
    const   size_t      (*resize)           (cstring * this, size_t new_size);

    // Erases the contents of the string and is effectively ""
    const   void        (*clear)            (cstring * this);

    // Tests for any non-null terminator characters in string
    const   bool        (*empty)            (cstring * this);

    // Shrinks the string array by erasing trailing null character sequences
    // this reduces the capacity to fit the size of the string
    const   void        (*shrink_to_fit)    (cstring * this);


    /* Iterators */

    // Iterator pointer to start of string sequence, this is a forward iterator
    cstr_iterator       (*begin)            (cstring * this);

    // Iterator pointer to end of string sequence, this is a reverse iterator
    cstr_iterator       (*rbegin)           (cstring * this);

    // Iterator pointer to an indicator that sits after the string in memory
    // !! trying to dereference this will cause a runtime error
    cstr_iterator       (*end)              (cstring * this);

    // Iterator pointer to an indicator that sits before the string in memory
    // !! trying to dereference this will cause a runtime error
    cstr_iterator       (*rend)             (cstring * this);
};


#endif
