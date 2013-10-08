#include "cstring.h"

#define ITR_END                 0xdeadbeef

#define CSTR_PAD        1

struct _cstr_
{
    size_t size;
    size_t allocator_size;
    void * rend;
    char * val;
    void * end;
};

struct _base_iterator_
{
    short category;
    void * buf;
    void * ptr;
    size_t data_size;
    const void      (*set)  (base_iterator, void *);
          void *    (*ref)  (base_iterator);
    const void      (*inc)  (base_iterator);
    const void      (*dec)  (base_iterator);
    const short     (*cat)  (base_iterator);

};

/// ITERATOR Functions ///

const void b_itr_set(base_iterator this, void * p)
{
    void * pz = malloc(this->data_size+1);
    memset(pz,0,this->data_size+1);
    memcpy(pz,p,this->data_size);
    free(this->buf);
    this->ptr = p;
    this->buf = pz;
}

void * b_itr_ref(base_iterator this)
{
    return this->buf;
}

const void b_itr_inc(base_iterator this)
{
    if(this->category == ITR_FORWARD || this->category == ITR_BI_DIRECTIONAL)
        this->ptr++;
    else if(this->category == ITR_REVERSE || this->category == ITR_R_BI_DIRECTIONAL)
        this->ptr--;
    b_itr_set(this,this->ptr);
}

const void b_itr_dec(base_iterator this)
{
    if(this->category == ITR_FORWARD || this->category == ITR_BI_DIRECTIONAL)
        this->ptr--;
    else if(this->category == ITR_REVERSE || this->category == ITR_R_BI_DIRECTIONAL)
        this->ptr++;
    b_itr_set(this,this->ptr);
}

const short b_itr_cat(base_iterator this)
{
    return this->category;
}


/// CSTR_ITRERATOR Functions ///

const base_iterator c_itr_ref(cstr_iterator this)
{
    return this->base;
}

/// ITERATOR Allocator ///
base_iterator b_itr(const short category, size_t data_size)
{
    base_iterator b = malloc(sizeof(struct _base_iterator_));

    if(category < ITR_FORWARD || category > ITR_R_BI_DIRECTIONAL)
        b->category = ITR_BI_DIRECTIONAL;
    else
        b->category = category;

    b->ptr = NULL;
    b->buf = NULL;
    b->data_size = data_size;

    b->set = &b_itr_set;
    b->ref = &b_itr_ref;
    b->cat = &b_itr_cat;
    b->inc = &b_itr_inc;
    b->dec = &b_itr_dec;

    return b;
}

cstr_iterator cstr_itr(const short category)
{
    cstr_iterator citr = malloc(sizeof(struct _cstr_iterator_));

    citr->base = b_itr(category, sizeof(char));

    citr->ref = &c_itr_ref;
    citr->set = (const void*)citr->base->set;
    citr->val = (char*(*)(cstr_iterator))citr->base->ref;
    citr->cat = (const short(*)(cstr_iterator))citr->base->cat;
    citr->inc = (const void(*)(cstr_iterator))citr->base->inc;
    citr->dec = (const void(*)(cstr_iterator))citr->base->dec;

    return citr;
}


/// CSTR Allocator ///

cstr new_cstr(const char * str);

/// Modifiers ///

const void cstr_add(cstring * this, const char * s);
const void cstr_push_back(cstring * this, const char c);
const void cstr_pop_back(cstring * this);
const void cstr_assign(cstring * this, const char * s);
const void cstr_insert(cstring * this, size_t pos, const char * s);
const void cstr_erase(cstring * this, size_t pos, size_t len);
const void cstr_swap(cstring * this, cstring * str_2);


/// Element Access ///
const char cstr_at(cstring * this, size_t pos);
const char cstr_back(cstring * this);
const char cstr_front(cstring * this);


/// String Operartions ///
const char * cstr_data(cstring * this);
const size_t cstr_copy(cstring * this, char ** s, size_t pos, size_t len);
const size_t cstr_find(cstring * this, const char * s, size_t * pos);
const size_t cstr_find_first(cstring * this, const char * s, size_t pos);
const size_t cstr_find_last(cstring * this, const char * s, size_t pos);
const size_t cstr_find_first_not(cstring * this, const char * s, size_t pos);
const size_t cstr_find_last_not(cstring * this, const char * s, size_t pos);
const char * cstr_substr(cstring * this, size_t pos, size_t len);
const bool   cstr_compare(cstring * this, const char * s);


/// Capacity ///
const size_t cstr_len(cstring * this);
const size_t cstr_max(cstring * this);
const size_t cstr_resize(cstring * this, size_t nsize);
const void   cstr_clear(cstring * this);
const bool   cstr_empty(cstring * this);
const void   cstr_shrink(cstring * this);


/// Iterators ///
cstr_iterator cstr_begin(cstring * this);
cstr_iterator cstr_rbegin(cstring * this);
cstr_iterator cstr_end(cstring * this);
cstr_iterator cstr_rend(cstring * this);


cstring * string(const char * str)
{
    cstring * cs = malloc(sizeof(struct _cstring_));

    if(str==NULL)
        cs->str = new_cstr("");
    else
        cs->str = new_cstr(str);

    cs->append = &cstr_add;
    cs->push_back = &cstr_push_back;
    cs->pop_back = &cstr_pop_back;
    cs->assign = &cstr_assign;
    cs->erase = &cstr_erase;
    cs->insert = &cstr_insert;
    cs->swap = &cstr_swap;
    cs->at = &cstr_at;
    cs->back = &cstr_back;
    cs->front = &cstr_front;
    cs->data = &cstr_data;
    cs->copy = &cstr_copy;
    cs->find = &cstr_find;
    cs->find_first_of = &cstr_find_first;
    cs->find_last_of = &cstr_find_last;
    cs->find_first_not_of = &cstr_find_first_not;
    cs->find_last_not_of = &cstr_find_last_not;
    cs->substr = &cstr_substr;
    cs->compare = &cstr_compare;
    cs->length = &cstr_len;
    cs->max_size = &cstr_max;
    cs->resize = &cstr_resize;
    cs->clear = &cstr_clear;
    cs->empty = &cstr_empty;
    cs->shrink_to_fit = &cstr_shrink;
    cs->begin = &cstr_begin;
    cs->rbegin = &cstr_rbegin;
    cs->end = &cstr_end;
    cs->rend = &cstr_rend;

    return cs;
}

void  delete_string(cstring * this)
{
    free(this->str);
    free(this);
    this = NULL;
}

/// CSTR Allocator ///

cstr new_cstr(const char * str)
{
    cstr s = malloc(sizeof(struct _cstr_) + strlen(str) + CSTR_PAD);

    s->size = strlen(str);
    s->allocator_size = sizeof(struct _cstr_) + strlen(str) + CSTR_PAD;
    s->val = malloc(strlen(str) + CSTR_PAD);

    memset(s->val,0, strlen(str) + CSTR_PAD);
    memcpy(s->val, str, strlen(str));

    s->rend = (void*)ITR_END;
    s->end = (void*)ITR_END;

    return s;
}


/// Modifiers ///

void cstr_append(cstring * this, const char * s)
{
    if(this == NULL)
        return;

    size_t ns = this->str->size + strlen(s) + CSTR_PAD;
    char * sz = malloc(ns);

    memset(sz, 0, ns);
    memcpy(sz, this->str->val, this->str->size);
    memcpy(sz + this->str->size, s, strlen(s));

    free(this->str);

    this->str = new_cstr(sz);

    free(sz);
}

void cstr_push_back(cstring * this, const char c)
{
    if(this == NULL)
        return;

    char s[2] = {c, '\0'};
    cstr_append(this, s);
}

void cstr_pop_back(cstring * this)
{
    if(this == NULL)
        return;

    size_t ns = this->str->size;
    char * sz = malloc(ns);

    memset(sz, 0, ns);
    memcpy(sz, this->str->val, this->str->size - 1);

    free(this->str);

    this->str = new_cstr(sz);

    free(sz);
}

void cstr_assign(cstring * this, const char * s)
{
    if(this == NULL)
        return;

    free(this->str);
    this->str = new_cstr(s);
}

void cstr_insert(cstring * this, size_t pos, const char * s)
{
    if(this == NULL)
        return;

    if(pos <= this->str->size)
    {
        size_t ns = this->str->size + CSTR_PAD + strlen(s);
        size_t end = ns - pos - strlen(s) - CSTR_PAD;
        char * sz = malloc(ns);

        memset(sz, 0, ns);
        memcpy(sz, this->str->val, pos);
        memcpy(sz + pos, s, strlen(s));
        memcpy(sz + pos + strlen(s) , this->str->val + pos, end);

        free(this->str);

        this->str = new_cstr(sz);

        free(sz);
    }
}

void cstr_erase(cstring * this, size_t pos, size_t len)
{
    if(this == NULL)
        return;

    if(pos < this->str->size && len <= this->str->size)
    {
        while(pos + len > this->str->size)
            --len;

        size_t ns = this->str->size - len + CSTR_PAD;
        size_t end = this->str->size - pos - len;
        char * sz = malloc(ns);

        memset(sz, 0, ns);
        memcpy(sz, this->str->val, pos);
        memcpy(sz + pos, this->str->val + pos + len, end);

        free(this->str);

        this->str = new_cstr(sz);

        free(sz);
    }
}

void cstr_swap(cstring * this, cstring * str_2)
{
    if(this == NULL || str_2 == NULL)
        return;

    cstr tmp = new_cstr(this->str->val);
    this->str = new_cstr(str_2->str->val);
    str_2->str = new_cstr(tmp->val);
    free(tmp);
}



/// Element Access ///
const char cstr_at(cstring * this, size_t pos)
{
    if(this == NULL)
        return '\0';

    char c = '\0';
    pos < this->str->size && this->str->size > 0 ? (c = this->str->val[pos]) : 0;
    return c;
}

const char cstr_back(cstring * this)
{
    if(this == NULL)
        return '\0';

    char c = '\0';
    this->str->size >= 1 ? (c = this->str->val[this->str->size - 1]) : 0;
    return c;
}

const char cstr_front(cstring * this)
{
    if(this == NULL)
        return '\0';

    char c = '\0';
    this->str->size >= 1 ? (c = this->str->val[0]) : 0;
    return c;
}



/// String Operartions ///
const char * cstr_data(cstring * this)
{
    if(this == NULL)
        return "";
    return this->str->val;
}

const size_t cstr_copy(cstring * this, char ** s, size_t pos, size_t len)
{
    // safety check
    if(this == NULL || this->str->size == 0)
    {
        // reallocates return string to null to keep user safe
        *s = malloc(CSTR_PAD);
        memset(*s,0,CSTR_PAD);
        return npos;
    }

    if(pos < this->str->size && len <= this->str->size)
    {
        while(pos + len > this->str->size)
            --len;

        *s = malloc(len + CSTR_PAD);

        memset(*s, 0, len + CSTR_PAD);
        memcpy(*s, this->str->val + pos, len);
    }
    else
    {
        len = npos;
    }

    return len;
}

const size_t cstr_find(cstring * this, const char * s, size_t * nxtpos)
{
    // valid string test
    if(this == NULL || !strlen(s) || !this->str->size || strcmp(this->str->val,"") == 0)
        return npos;

    size_t ret = npos;
    size_t ipos = 0;
    size_t slen = strlen(s);
    // checks for a previous search of this string and starts
    // at a position after the last string
    if(nxtpos != NULL)
        ipos = *nxtpos;


    // logic : - check first and last char before checking string
    // iterate through characters until mismatch or until full match
    for( ; ipos != this->str->size && ipos + slen <= this->str->size; ++ipos)
    {
        char * ip = &this->str->val[ipos];
        char * ep = (char*)&s[0];

        // Tests for single character length strings
        if(slen - 1 == 0 && *ip == *ep)
        {
            ret = ipos;
            break;
        }

        // tests for start and end matching characters,
        // this indicates a high probability match and investigates
        if(*ip == *ep && *(ip + slen - 1) == *(ep + slen - 1))
        {
            while(*++ip == *++ep && (*(ip + 1) && *(ep + 1)));
            // tests for a complete match by seeing if the search went
            // the entire way through the search string
            if(*ep == s[slen-1])
            {
                ret = ipos;
                break;
            }
        }
    }

    // sets the next search position if another search is run
    if(nxtpos != NULL)
        *nxtpos = ret + slen;

    return ret;
}

const size_t cstr_find_first_of(cstring * this, const char * s, size_t pos)
{
    // valid string test
    if(this == NULL || !strlen(s) || !this->str->size || strcmp(this->str->val,"") == 0)
        return npos;

    size_t ret = npos;
    size_t ipos = pos;
    size_t slen = strlen(s);

    // reads through the string forwards
    for( ; ipos <= this->str->size; ++ipos)
    {
        char * ip = &this->str->val[ipos];  // internal pointer
        char * sp = (char*)&s[0];                  // start pointer
        char * ep = (char*)&s[slen-1];             // end pointer

        // iterate through search str for any matches to current pointer
        // only does this for searches larger than a single character
        if(slen > 1)
            while((*ip != *sp && *ip != *ep) && ++sp != ep && --ep != sp);

        // tests for any matches
        if(*ip == *sp || *ip == *ep)
        {
            ret = ipos;
            break;
        }
    }

    return ret;
}

const size_t cstr_find_last_of(cstring * this, const char * s, size_t pos)
{
    // valid string test
    if(this == NULL || !strlen(s) || !this->str->size || strcmp(this->str->val,"") == 0)
        return npos;

    size_t ret = npos;
    size_t ipos = this->str->size-1;
    size_t slen = strlen(s);

    // reads through the string backwards
    for( ; ipos >= pos && ipos > 0; --ipos)
    {
        char * ip = &this->str->val[ipos];  // internal pointer
        char * sp = (char*)&s[0];                  // start pointer
        char * ep = (char*)&s[slen-1];             // end pointer

        // iterate through search str for any matches to current pointer
        // only does this for searches larger than a single character
        if(slen > 1)
            while((*ip != *sp && *ip != *ep) && ++sp != ep && --ep != sp);

        // tests for any matches
        if(*ip == *sp || *ip == *ep)
        {
            ret = ipos;
            break;
        }
    }

    return ret;
}

const size_t cstr_find_first_not_of(cstring * this, const char * s, size_t pos)
{
    // valid string test
    if(this == NULL || !strlen(s) || !this->str->size || strcmp(this->str->val,"") == 0)
        return npos;

    size_t ret = npos;
    size_t ipos = pos;
    size_t slen = strlen(s);

    // reads through the string forwards
    for( ; ipos < this->str->size; ++ipos)
    {
        char * ip = &this->str->val[ipos];  // internal pointer
        char * sp = (char*)&s[0];                  // start pointer
        char * ep = (char*)&s[slen-1];             // end pointer

        // iterate through search str for any matches to current pointer
        // only does this for searches larger than a single character
        if(slen > 1)
            while((*ip != *sp && *ip != *ep) && ++sp != ep && --ep != sp);

        // tests for any matches, continues to search if a match is found
        // otherwise it escapes and returns the position
        if(*ip == *sp || *ip == *ep)
            continue;
        else
        {
            ret = ipos;
            break;
        }
    }

    return ret;
}

const size_t cstr_find_last_not_of(cstring * this, const char * s, size_t pos)
{
    // valid string test
    if(this == NULL || !strlen(s) || !this->str->size || strcmp(this->str->val,"") == 0)
        return npos;

    size_t ret = npos;
    size_t ipos = this->str->size-1;
    size_t slen = strlen(s);

    // reads through the string backwards
    for( ; ipos >= pos && ipos > 0; --ipos)
    {
        char * ip = &this->str->val[ipos];  // internal pointer
        char * sp = (char*)&s[0];                  // start pointer
        char * ep = (char*)&s[slen-1];             // end pointer

        // iterate through search str for any matches to current pointer
        // only does this for searches larger than a single character
        if(slen > 1)
            while((*ip != *sp && *ip != *ep) && ++sp != ep && --ep != sp);

        // tests for any matches
        if(*ip == *sp || *ip == *ep)
            continue;
        else
        {
            ret = ipos;
            break;
        }
    }

    return ret;
}

const char * cstr_substr(cstring * this, size_t pos, size_t len)
{
    if(this == NULL || this->str->size == 0)
        return "";

    char * sz = NULL;

    if(pos < this->str->size && len <= this->str->size)
    {
        while(pos + len > this->str->size)
            --len;

        sz = malloc(len + CSTR_PAD);

        memset(sz, 0, len + CSTR_PAD);
        memcpy(sz, this->str->val + pos, len);
    }

    return sz;
}

const bool   cstr_compare(cstring * this, const char * s)
{
    if(this == NULL)
        return false;
    if(strlen(s) == this->str->size)
    {
        char * c1 = &this->str->val[0];
        char * c2 = (char*)&s[0];

        if(*c1 != *c2)
            return false;

        while(*c1++ && *c2++)
            if(*c1 != *c2)
                return false;

        return true;
    }

    return false;
}

const bool   cstr_instr(cstring * this, const char *s)
{
    if(this == NULL)
        return false;

    return cstr_find(this,s,0) == npos ? false : true;
}



/// Capacity ///
const size_t cstr_len(cstring * this)
{
    if(this == NULL)
        return npos;
    return this->str->size;
}

const size_t cstr_max(cstring * this)
{
    if(this == NULL)
        return npos;
    return LONG_MAX;
}

const size_t cstr_resize(cstring * this, size_t nsize)
{
    if(this == NULL)
        return npos;
    if(nsize > 0 && nsize < LONG_MAX && nsize != this->str->size)
    {
        size_t ns = nsize + CSTR_PAD;
        char * sz = malloc(ns);

        memset(sz, 0, ns);
        if(ns > this->str->size)
            memcpy(sz, this->str->val,this->str->size);
        else
            memcpy(sz, this->str->val,ns-1);

        free(this->str->val);
        this->str->val = malloc(ns);

        this->str->size = ns - 1;
        this->str->allocator_size = sizeof(struct _cstr_) + ns;
        memcpy(this->str->val, sz, ns);

        free(sz);
    }
    else
    {
        nsize = npos;
    }

    return nsize;
}

void   cstr_clear(cstring * this)
{
    if(this == NULL)
        return;
    this->str = new_cstr("");
}

const bool   cstr_empty(cstring * this)
{
    if(this == NULL)
        return false;
    return this->str->size == 0;
}

void   cstr_shrink(cstring * this)
{
    if(this == NULL)
        return;

    while(--this->str->size > strlen(this->str->val));

    this->str = new_cstr(this->str->val);
}

/// Iterators ///
cstr_iterator cstr_begin(cstring * this)
{
    if(this == NULL)
        return (cstr_iterator)ITR_END;
    cstr_iterator itr = cstr_itr(ITR_BI_DIRECTIONAL);
    this->str->size != 0 ? (itr->set((cstr_iterator)itr->base,&this->str->val[0]))
                         : (itr->set((cstr_iterator)itr->base,(char*)&this->str->end));
    return itr;
}

cstr_iterator cstr_rbegin(cstring * this)
{
    if(this == NULL)
        return (cstr_iterator)ITR_END;
    cstr_iterator itr = cstr_itr(ITR_R_BI_DIRECTIONAL);
    this->str->size != 0 ? (itr->set((cstr_iterator)itr->base,&this->str->val[this->str->size-1]))
                         : (itr->set((cstr_iterator)itr->base,(char*)&this->str->rend));
    return itr;
}

cstr_iterator cstr_end(cstring * this)
{
    if(this == NULL)
        return (cstr_iterator)ITR_END;
    return (cstr_iterator)this->str->end;
}

cstr_iterator cstr_rend(cstring * this)
{
    if(this == NULL)
        return (cstr_iterator)ITR_END;
    return (cstr_iterator)this->str->rend;
}
