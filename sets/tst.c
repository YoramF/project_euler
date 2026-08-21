#include <stdio.h>
#include <stdlib.h>

#include "sets.h"

typedef struct {
    long unsigned int key;
    char data [10];
} record_t;

void dump_d(BLOCK *b, unsigned int off, unsigned int size) {
    // for (int i = 0; i < size; i++)
        // printf("%d,", b->data[off*size+i]);
        printf("%lu", *(long unsigned *)&b->data[off*size]);
}

void dump_e (S_ELEMENT *e, SET *s) {
    if (e->block_ptr == NULL)
        printf("EMPTY\n");
    else {
        printf("\n");
        do {
            dump_d(e->block_ptr, e->e_off, s->e_size);
            e = e->e_next;
            printf("\n");
        }
        while (e != NULL);
    }
}

void dump_set(SET *s) {
    printf("set-> e_size: %d\n", s->e_size);
    printf("e_amount: %ld\n", s->e_amount);
    printf("set:\n");
    for (int i = 0; i < s->hash_n; i++) {
        printf("[%d] ", i);
        dump_e(&s->set[i], s);
    }
}

void gen_data (char *data, int n, int d) {
    for (int i = 0; i < n; i++)
        data[i] = d + '0';
}

unsigned int i_func(const unsigned int n, const void *item,  const unsigned int i_size) {
    record_t *r1;

    r1 = (record_t *)item;
    return (r1->key % n);
}

bool c_func(const void *e_item, const void *n_item,  const unsigned int i_size){
    record_t *r1, *r2;

    r1 = (record_t *)e_item;
    r2 = (record_t *)n_item;

    return (r1->key == r2->key);
}

unsigned int i_func_p(const unsigned int n, const void *item,  const unsigned int i_size) {
    record_t **r1;

    r1 = (record_t **)item;
    return ((*r1)->key % n);
}

bool c_func_p(const void *e_item, const void *n_item,  const unsigned int i_size){
    record_t **r1, **r2;

    r1 = (record_t **)e_item;
    r2 = (record_t **)n_item;

    return ((*r1)->key == (*r2)->key);
}

int main () {
    SET *s;
    long unsigned int lli = 6L;
    long unsigned int li = 7L;
    int k;
    record_t i_rec, r_rec;
    record_t *i_rec_p, *r_rec_p, **gn_rec_p;

    s = set_create(51, sizeof(lli));

    for (int i = 1; i <= 50; i++) {
        lli *= i;
        k = set_insert(s, &lli);
        printf("inset %lu, returned %d\n", lli, k);
    }

    printf("set amount: %ld\n", set_amount(s));

    lli = 6L;
    for (int i = 1; i <= 50; i++) {
        lli *= i;
        printf("value: %lu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }

    dump_set(s);

    set_clear(s);

    dump_set(s);
    lli = 6L;
    for (int i = 1; i <= 50; i++) {
        lli *= i;
        printf("value: %lu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        k = set_insert(s, &lli);
        printf("inset %lu, returned %d\n", lli, k);
    }
    printf("set amount: %ld\n", set_amount(s));
    dump_set(s);

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        printf("value: %lu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }
    set_clear(s);
    dump_set(s);

    lli = 9L;
    for (int i = 1; i <= 40; i++) {
        lli *= i;
        printf("value: %lu, %s\n", lli, set_e_in_set(s, &lli)? "In set": "not in set");
    }
    
    
    // we need to delete the set since now we are going to use different type of item
    set_delete(s);

    // now use set with optional variables
    printf("Start using user mode functions on record\n");
    s = set_create(20, sizeof(record_t), &i_func, &c_func);
    li = 6L;
    for (int i = 1; i <= 50; i++) {
        li *= i;
        i_rec.key = li;
        gen_data(&i_rec.data[0], sizeof(i_rec.data), i);
        k = set_insert(s, &i_rec);
        printf("inset %lu, returned %d\n", li, k);
    }

    li = 6L;
    for (int i = 1; i <= 40; i++) {
        li *= i;
        i_rec.key = li;
        gen_data(&i_rec.data[0], sizeof(i_rec.data), i+2);
        k = set_insert(s, &i_rec, &r_rec);
        printf("inset %lu, returned %d\n", li, k);
        if (k == 0)
            printf("key1: %lu, data1: %.10s; key2: %lu, data2: %.10s\n", i_rec.key, i_rec.data, r_rec.key, r_rec.data);
    }
    printf("set amount: %ld\n", set_amount(s));
    dump_set(s);

    // again, we change the type of item so new set need to be created
    set_delete(s);    

    // now use set with optional variables and store pointers instead of actual data
    // In case the pointer is point to a key'ed record and we need to avoid entering the same key into
    // the set, the i_func() and c_func() need to deal with pointer to pionter, the item we pass in set_insert
    // is the address of the variable that holds the pointer, and the item we save in the set is the pointer value.
    // it is very tricky since we must not free the pointer, as long as the set is active
    // and we can't clear the set, before we loop over all items in the set (set_get_next_e()) and free all pointers
    // and remember, in this case set_get_next_e() returns a pointer to pointer!
    // in general it is very risky to use pointers instead of actual data and there is not really memory saving here since
    // the ram the saved poiner is not going to be released as long as the set is active!!
    // it is here to demonstrate the flexibility of the sets lib.
    printf("Start using pointers\n");
    s = set_create(20, sizeof(record_t *), &i_func_p, &c_func_p);
    li = 1L;
    for (int i = 1; i <= 50; i++) {
        li *= i;
        i_rec_p = malloc(sizeof(record_t));
        i_rec_p->key = li;
        gen_data(&(i_rec_p->data[0]), sizeof(i_rec_p->data), i);
        k = set_insert(s, &i_rec_p);
        printf("inset %p, returned %d\n", i_rec_p, k);
    }


    li = 1L;
    for (int i = 1; i <= 40; i++) {
        li *= i;
        i_rec_p = malloc(sizeof(record_t));
        i_rec_p->key = li;
        gen_data(&(i_rec_p->data[0]), sizeof(i_rec_p->data), i+1);
        k = set_insert(s, &i_rec_p, &r_rec_p);
        printf("inset %p, returned %d\n", i_rec_p, k);
        if (k == 0) {
            printf("key1: %lu, data1: %.10s; key2: %lu, data2: %.10s\n", i_rec_p->key, i_rec_p->data, r_rec_p->key, r_rec_p->data);
            free(i_rec_p);
        }

    }
    printf("set amount: %ld\n", set_amount(s));
    dump_set(s);

    S_GET_NEXT s_g_n = set_reset_get(s);

    while ((gn_rec_p = set_get_next_e(&s_g_n)) != NULL) {
        printf("p: %p, key: %lu, data: %.10s\n", (*gn_rec_p), (*gn_rec_p)->key, (*gn_rec_p)->data);
        // note that we are freeing the value that gn_rec_p is pointing to.
        free(*gn_rec_p);
    }

    // only after we retrieved all values from the set, we can cear it.
    set_delete(s);  

    return 0;
}