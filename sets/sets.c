/*
Implrement SETs library
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdarg.h>

#include "sets.h"
#include "crc.h"

#define MAX_U_INT 1048576

static unsigned int _i_func_8(const unsigned int n, const void *item,  const unsigned int i_size) {
    unsigned char v;
    v = *(unsigned char *)item;
    return v % n;
}

static unsigned int _i_func_16(const unsigned int n, const void *item,  const unsigned int i_size) {
    unsigned short v;
    v = *(unsigned short *)item;
    return v % n;
}

static unsigned int _i_func_32(const unsigned int n, const void *item,  const unsigned int i_size) {
    unsigned int v;
    v = *(unsigned int *)item;
    return v % n;
}

static unsigned int _i_func_64(const unsigned int n, const void *item,  const unsigned int i_size) {
    unsigned long long v;
    v = *(unsigned long long *)item;
    return v % n;
}

static unsigned int _i_func(const unsigned int n, const void *item,  const unsigned int i_size) {
    unsigned short crc;
    crc = CRC_Calc16(item, i_size);
    return crc % n;
}

static bool _is_eq_func_8(const void *e_item, const void *n_item,  const unsigned int i_size) {
    unsigned char e_v;
    unsigned char n_v;
    n_v = *(unsigned char *)n_item;
    e_v = *(unsigned char *)e_item;
    return ((e_v - n_v) == 0);
}

static bool _is_eq_func_16(const void *e_item, const void *n_item,  const unsigned int i_size) {
    unsigned short e_v;
    unsigned short n_v;
    n_v = *(unsigned short *)n_item;
    e_v = *(unsigned short *)e_item;
    return ((e_v - n_v) == 0);
}

static bool _is_eq_func_32(const void *e_item, const void *n_item,  const unsigned int i_size) {
    unsigned int e_v;
    unsigned int n_v;
    n_v = *(unsigned int *)n_item;
    e_v = *(unsigned int *)e_item;
    return ((e_v - n_v) == 0L);
}

static bool _is_eq_func_64(const void *e_item, const void *n_item,  const unsigned int i_size) {
    unsigned long long e_v;
    unsigned long long n_v;
    n_v = *(unsigned long long *)n_item;
    e_v = *(unsigned long long *)e_item;
    return ((e_v - n_v) == 0LL);
}

static bool _is_eq_func(const void *e_item, const void *n_item,  const unsigned int i_size) {
    return (memcmp(e_item, n_item, i_size) == 0);
}

// create a new set
// this function can have 2 additonal optional variables which are address of insert and compare functions
// insert function: unsigned int (*_i_func)(const unsigned int n, const void *item,  const unsigned int i_size)
// compare function: bool (*c_func)(const void *e_item, const void *n_item,  const unsigned int i_size))
// that will do the internal comperison instead
// of the default ones
SET *internal_set_create (const size_t max_items, const unsigned int element_size, ...) {
    SET *set;
    BLOCK *block;
    size_t e_size;
    unsigned int hash_n;
    va_list args;

    unsigned int (*i_func)(const unsigned int n, const void *item,  const unsigned int i_size);
    bool (*c_func)(const void *e_item, const void *n_item,  const unsigned int i_size);

    // get optional argument first
    va_start(args);
    i_func = va_arg(args, typeof(i_func));
    if (i_func == (void *)-1) {
        // no optional arguments
        i_func = NULL;
        c_func = NULL;
    }
    else {
        c_func = va_arg(args, typeof(c_func));
        if (c_func == (void *)-1) {
            fprintf(stderr, "Inser function was specified but compare function\n");
            va_end(args);
            return NULL;
        }
    }

    va_end(args);


    hash_n = max_items > MAX_U_INT? MAX_U_INT: max_items;

    // allocate first block of data
    if ((block = calloc(hash_n*element_size+sizeof(BLOCK), sizeof(char))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        return NULL;
    }
    block->e_left = hash_n;

    // allocate SET
    e_size = hash_n * sizeof(S_ELEMENT);
    if ((set = calloc(e_size+sizeof(SET), sizeof(char))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        free(block);
        return NULL;        
    }

    // complete data in SET
    set->block_ptr = block;
    set->e_size = element_size;
    set->hash_n = hash_n;

    // set->i_func
    if (i_func != NULL) {
        set->i_func = i_func;
        set->is_eq_func = c_func;
    }
    else {
        switch (element_size) {
            case 1:
                set->i_func = _i_func_8;
                set->is_eq_func = _is_eq_func_8;
                break;
            case 2:
                set->i_func = _i_func_16;
                set->is_eq_func = _is_eq_func_16;
                break;
            case 4:
                set->i_func = _i_func_32;
                set->is_eq_func = _is_eq_func_32;
                break;
            case 8:
                set->i_func = _i_func_64;
                set->is_eq_func = _is_eq_func_64;
                break;
            default:
                set->i_func = _i_func;
                set->is_eq_func = _is_eq_func;
                break;
        }
    }


    return set;
}

// delete a set
void set_delete (SET *set) {
    BLOCK *blk, *blk_n;
    S_ELEMENT *e, *en;
    unsigned int u;

    blk = set->block_ptr;
    while (blk != NULL) {
        blk_n = blk->b_next;
        free(blk);
        blk = blk_n;
    }

    // free elemets lists
    for (u = 0; u < set->hash_n; u++) {
        if (set->set[u].e_next != NULL) {
            e = set->set[u].e_next;
            while (e != NULL) {
                en = e->e_next;
                free(e);
                e = en;
            }
        }
    }

    free(set);
}

// ckear set content
void set_clear (SET *set) {
    BLOCK *bl, *ebl;
    S_ELEMENT *e, *en;
    unsigned int u;

    // clear all blocks of data
    bl = set->block_ptr->b_next;
    while(bl != NULL) {
        ebl = bl->b_next;
        free(bl);
        bl = ebl;
    }
    bl = set->block_ptr;
    memset(&bl->data, 0, set->e_size*set->hash_n);
    bl->e_left = set->hash_n;
    bl->n_space = 0;
    bl->b_next = NULL;

    set->e_amount = 0L;
    // free elemets lists
    for (u = 0; u < set->hash_n; u++) {
        if (set->set[u].e_next != NULL) {
            e = set->set[u].e_next;
            while (e != NULL) {
                en = e->e_next;
                free(e);
                e = en;
            }
            set->set[u].e_next = NULL;  
        }
        set->set[u].block_ptr = NULL;
        set->set[u].e_off = 0;
    }
}

// write new data into block and return block address
BLOCK *_set_w_block (SET *set, const void *item, unsigned int *e_off) {
    BLOCK *bl, *nbl;
    unsigned char *d_off;

    bl = set->block_ptr;
    nbl = bl->b_next;

    // get to last block in chain
    while (nbl != NULL) {
        bl = nbl;
        nbl = nbl->b_next;
    }

    // check if last data blockis is full. if it does add new data block
    if (bl->e_left == 0) {
        if ((nbl = calloc(set->hash_n*set->e_size+sizeof(BLOCK), sizeof(char))) == NULL) {
            fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
            return NULL;
        }
        bl->b_next = nbl;
        bl = nbl;
        bl->e_left = set->hash_n;
    }

    *e_off = bl->n_space;
    memcpy(&(bl->data[(bl->n_space)*(set->e_size)]), item, set->e_size);
    bl->n_space++;
    bl->e_left--;

    return bl;
}

// inset item into hash table
static S_ELEMENT _set_direct_insert_e (SET *set, const void *item) {
    S_ELEMENT se;

    se.e_next = NULL;
    se.block_ptr = _set_w_block(set, item, &se.e_off);

    return se;   
}

// insert new item indirect and return address of new S_ELEMENT
static S_ELEMENT *_set_indirect_insert_e (SET *set, const void *item) {
    S_ELEMENT *ns_e;

    if ((ns_e = calloc(1, sizeof(S_ELEMENT))) == NULL) {
        fprintf(stderr, "Failed to allocate RAM: %s\n", strerror(errno));
        return NULL;        
    }

    ns_e->block_ptr = _set_w_block(set, item, &ns_e->e_off);

    return ns_e;
}


// insert new item to set. return 1 on success, 0 if item already in set, -1 on error
// if this argument is specified and is not NULL, the function will return the item value (the existing one or the just inserted one)
// the 3rd argument must be a valid address to where the item will be written
// if the function fail and return -1, the value where the 3rd variable is pointing to is not updated.
int internal_set_insert (SET *set, void *item, ...) {
    unsigned int hash_i;
    S_ELEMENT *n_e, *c_e;
    unsigned char *data_p;
    void *r_item;
    va_list args;

    // get optional arguments
    va_start(args);
    r_item = va_arg(args, typeof(r_item));
    if (r_item == (void *)-1)
        r_item = NULL;

    va_end(args);

    hash_i = set->i_func(set->hash_n, item, set->e_size);

    // check if item place is empty
    if (set->set[hash_i].block_ptr == NULL) {
        set->set[hash_i] = _set_direct_insert_e(set, item);
        set->e_amount++;
        // if r_item was provided copy item value to r_item
        if (r_item != NULL)
            memcpy(r_item, item, set->e_size);
        return 1;
    }

    // if hash location already in set
    c_e = &(set->set[hash_i]);
    data_p = &(c_e->block_ptr->data[(c_e->e_off)*(set->e_size)]);
    if (set->is_eq_func(data_p, item, set->e_size)) {
        // item already in set. if r_item was provided copy existing item value to r_item
        if (r_item != NULL)
            memcpy(r_item, data_p, set->e_size);
        return 0;
    }


    // check all existing elements in this location 
    n_e = c_e->e_next;
    while (n_e != NULL) {
        c_e = n_e;
        data_p = &(n_e->block_ptr->data[(n_e->e_off)*(set->e_size)]);
        if (set->is_eq_func(data_p, item, set->e_size)) {
        // item already in set. if r_item was provided copy existing item value to r_item
        if (r_item != NULL)
            memcpy(r_item, data_p, set->e_size);
        return 0;
        }

        n_e = n_e->e_next;
    }

    // if we got here, we need to add the new element into the chain
    if ((c_e->e_next = _set_indirect_insert_e (set, item)) != NULL) {
        set->e_amount++;
        // if r_item was provided copy item value to r_item
        if (r_item != NULL)
            memcpy(r_item, item, set->e_size);
        return 1;
    }

    // we failed to insert new element
    return -1;
}

// return true of element in set
bool set_e_in_set (SET *set, void *item) {
    S_ELEMENT *s_e;
    unsigned char *data_p;
    unsigned int hash_i;

    hash_i = set->i_func(set->hash_n, item, set->e_size);
    s_e = &(set->set[hash_i]);

    while (s_e != NULL && s_e->block_ptr != NULL) {
        data_p = &(s_e->block_ptr->data[(s_e->e_off)*(set->e_size)]);        
        if (set->is_eq_func(data_p, item, set->e_size))
            return true;
        s_e = s_e->e_next;
    }

    return false;
}

// this function is used to fetch existing item from set based on given item, where the
// key to store the item is not the item itself but a portion of it, and the insert and compare functions
// are user provided functions. If the item is not in the set, it returns false, otherwise the saved item value
// is copied to r_item address.
bool set_get_e (SET *set, void *item, void *r_item) {
    S_ELEMENT *s_e;
    unsigned char *data_p;
    unsigned int hash_i;

    hash_i = set->i_func(set->hash_n, item, set->e_size);
    s_e = &(set->set[hash_i]);

    while (s_e != NULL && s_e->block_ptr != NULL) {
        data_p = &(s_e->block_ptr->data[(s_e->e_off)*(set->e_size)]);        
        if (set->is_eq_func(data_p, item, set->e_size)) {
            memcpy(r_item, data_p, set->e_size);
            return true;
        }

        s_e = s_e->e_next;
    }

    return false;
}

// get next element in set
void *set_get_next_e (S_GET_NEXT *g_n) {
    S_ELEMENT *s_e;
    unsigned int hash_i;
    SET *s;

    if (g_n == NULL)
        return NULL;

    s_e = g_n->s_e;
    hash_i = g_n->hash_i;
    s = g_n->set;

    // check if inside S_ELEMENT chain
    if (s_e != NULL && s_e->block_ptr != NULL) {
        g_n->s_e = s_e->e_next;
        return (&(s_e->block_ptr->data[(s_e->e_off)*(s->e_size)]));
    }
    else
        hash_i++;

    // we need to get to next S_ELEMENT in SET
    while (hash_i < s->hash_n) {
        s_e = &(s->set[hash_i]);
        if (s_e->block_ptr != NULL) {
            g_n->s_e = s_e->e_next;
            g_n->hash_i = hash_i;
            return (&(s_e->block_ptr->data[(s_e->e_off)*(s->e_size)]));         
        }
        hash_i++;
    }

    // reaced end of set!
    return NULL;
}

// reset intem retrival from a set
S_GET_NEXT set_reset_get (SET *set) {
    S_GET_NEXT g_n;

    g_n.set = set;
    g_n.hash_i = 0;
    g_n.s_e = &(set->set[0]);

    return g_n;
}  
