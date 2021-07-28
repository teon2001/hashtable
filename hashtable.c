#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Val{
    int key;
    int size;
    void* info;
    struct Val *next;
} Val;

typedef struct {
    void** table;
    int (*hashf)(void *, int, int);
    int size;
} Hasht;

void *h_create(int size, int (*hashf)(void *, int, int)) {
    Hasht *h = malloc(sizeof(Hasht));
    h->hashf = hashf;
    h->size = size;
    h->table = malloc(size * sizeof(void*));
    for(int i = 0; i < size; i++) {
        h->table[i] = l_create();
    }
    return h;
}

static Hasht *conv(void *ht) {
    return (Hasht *)ht;
}

static Val* create_node(void *val, int size, int key) {
    Val* cel = malloc(sizeof(Val));
    cel->info = val;
    cel->size = size;
    cel->key = key;
    cel->next = NULL;
}

int h_insert(void* ht, void *val, int size) {
    Hasht* h = conv(ht);
    int idx = h->hashf(val, h->size, size);
    Val* cel = create_node(val, size, idx);
    return l_insert_first(&h->table[idx], cel);

}

static int comp_info(void* a, void* b) {
    //0 = egalitate
    //--val sa fie Val
    //if(((Val*)a)->size != ((Val*)b)->size) return 1;
    return memcmp(((Val*)a)->info, b, ((Val*)a)->size);
}

static int comp(void *a, void *b) {
    //0 = egalitate
    if(((Val*)a)->size != ((Val*)b)->size) return 1;
    return memcmp(((Val*)a)->info, ((Val*)b)->info, ((Val*)b)->size);
}

static void free_val(Val* i, void (*free_info)(void *)) {
    free_info(i->info);
    free(i);
}
static void afisare(void* info) {
    printf("%d ", *(int*)((Val*)info)->info);
}
int h_delete(void* ht, void *val, void (*free_info)(void *), int size) {
    Hasht* h = conv(ht);
    int idx = h->hashf(val, h->size, size);
    void* i = l_remove_info(&h->table[idx], val, comp_info);
    free_val(i, free_info);
}

int h_exist(void *ht, void *val, int size ) {
    Hasht* h = conv(ht);
    int idx = h->hashf(val, h->size, size);
    return l_contains(h->table[idx], val, comp);
    
}

void h_free(void *ht, void (*free_info)(void *)) {
    Hasht* h = conv(ht);
    for(int i = 0; i < h->size; i++) {
        l_free(&h->table[i], free_info);
    }
    free(h);
}
/*
3 5 6
8 9
0 9 7 4

*/

static int count_elements(Hasht* h) {
    int sum = 0;
    for(int i = 0; i < h->size; i++)
        sum = sum + l_length(h->table[i]);
    return sum;
}

static Val** get_values(void *ht, int size) {
    Hasht *h = conv(ht);
    Val** vector = malloc(50* sizeof(Val*));
    // for(int i = 0; i < count_elements(h); i++) {
    //     vector[i] = malloc(sizeof(Val));
    // }
    int contor = 0;
    for(int i = 0; i < size; i++) {
        for(Val* iter = h->table[i]; iter != NULL; iter = iter->next) {
            printf("numarul: %d\n", *(int*)iter->info);
                /*primesc seg cand pun printf, in cometariul de mai sus am incercat
                aloc si fiecare Val* si tot nu merge  */
            vector[contor++] = iter; 
        }
    }
    return vector;

}

void* h_resize(void *ht, int new_size, int (*hashf)(void *, int, int)) {
    Hasht* h = conv(ht);
    Hasht* new_h = conv(h_create(new_size, hashf));
    Val** vector = get_values(h, h->size);
    
    for(int i = 0; i < count_elements(h); i++) {
        h_insert(new_h, vector[i]->info, vector[i]->size);
    }
return new_h;




    // for(int i = 0; i < h->size; i++) {
    //     for(Val* iter = h->table[i]; iter != NULL; iter = iter->next) {
    //         printf("gfdfghbjk\n");
    //         printf("idx vechi: %p\n", );
    //         h_insert(new_h, iter->info, iter->size);
    //         printf("idx nou: %d\n", iter->key);
    //     }
    // }
    // return (void*)new_h;
}