void *h_create(int size, int (*hashf)(void *, int, int));
int h_insert(void* ht, void *val, int size);
int h_delete(void* ht, void *val, void (*free_info)(void *), int size);
int h_exist(void *ht, void *val, int size);
void h_free(void *ht, void (*free_info)(void *));
void* h_resize(void *ht, int size, int (*hashf)(void *, int, int));