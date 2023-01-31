/*
 * shash.c - hash table library for strings
 *
 * Written in 2023 by Jon Mayo <jon@rm-f.net>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#include "shash.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static unsigned long
shash(const char *s)
{
	unsigned long h;
	int c;
	for (h = 5381; (c = (unsigned char)*s++); h += (h << 5) + c)
		;
	return h;
}

struct h_str_entry {
	char *k;
	void *v;
};

struct h_str {
	unsigned n;
	struct h_str_entry *e;
	void (*free_value)(void *);
};

static struct h_str_entry *
find(H_STR *T, const char *k)
{
	unsigned long h = shash(k);

	struct h_str_entry *e, *start = &T->e[h & (T->n - 1)];

	e = start; /* detect completely filled table */
	/* linear probe to resolve collisions */
	while (e->k) {
		if (strcmp(e->k, k) == 0) {
			return e; /* found */
		}

		h++;
		e = &T->e[h & (T->n - 1)];
		if (e == start) {
			return NULL; /* looped around */
		}
	}

	return e;
}

H_STR *
h_str_new(unsigned limit, void (*free_value)(void*))
{
	H_STR *T = malloc(sizeof(*T));
	if (!T) {
		return NULL;
	}

	if (!limit) {
		limit = 64;
	} else {
		limit--;
		limit |= limit >> 1;
		limit |= limit >> 2;
		limit |= limit >> 4;
		limit |= limit >> 8;
		limit |= limit >> 16;
		limit++;
	}

	*T = (H_STR){ limit,  malloc(limit * sizeof(*T->e)), free_value };

	if (!T->e) {
		free(T);
		return NULL;
	}

	return T;
}

void
h_str_free(H_STR *T)
{
	if (T) {
		unsigned i, n = T->n;
		for (i = 0; i < n; i++) {
			free(T->e[i].k);
			if (T->free_value) {
				T->free_value(T->e[i].v);
			}
		}
		free(T->e);
		free(T);
	}
}

int
h_str_set(H_STR *T, const char *k, void *v)
{
	struct h_str_entry *e = find(T, k);

	if (!e || e->k) {
		return -1; /* error or duplicate entry */
	}

	/* create the key and set the value */
	e->k = strdup(k);
	e->v = v;

	return 0;
}

void *
h_str_get(H_STR *T, const char *k)
{
	struct h_str_entry *e = find(T, k);

	return e && e->k ? e->v : NULL;
}

int
h_str_replace(H_STR *T, const char *k, void *v)
{
	struct h_str_entry *e = find(T, k);

	if (!e) {
		return -1; /* error */
	}

	/* create the key */
	if (!e->k) {
		e->k = strdup(k);
	}

	/* free the old value */
	if (e->v && T->free_value) {
		T->free_value(e->v);
	}

	e->v = v;

	return 0;
}

#if 0 // add this if we change the linear probing algorithm
int
h_str_delete(H_STR *T, const char *k)
{
	struct h_str_entry *e = find(T, k);

	if (!e || !e->k) {
		return -1; /* error or no entry */
	}

	/* free the key */
	if (e->k) {
		free(e->k);
		e->k = NULL;
	}

	/* free the value */
	if (e->v && T->free_value) {
		T->free_value(v);
	}

	abort(); // the table needs to be rehashed - that's a limitation of linear probing

	return 0;
}
#endif
