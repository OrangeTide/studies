#ifndef SHASH_H_
#define SHASH_H_
typedef struct h_str H_STR;
H_STR *h_str_new(unsigned limit, void (*free_value)(void*));
void h_str_free(H_STR *T);
int h_str_set(H_STR *T, const char *k, void *v);
void *h_str_get(H_STR *T, const char *k);
int h_str_replace(H_STR *T, const char *k, void *v);
#endif
