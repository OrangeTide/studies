/*
 * test_shash.c - test program for string hash table library
 *
 * Written in 2023 by Jon Mayo <jon@rm-f.net>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shash.h"

int
main()
{
	const char *test_keys[] = { "tenuous", "strong", "amusement", "zany", "burly", };
	const char *test_values[] = { "present", "wait", "boorish", "thrill", "dry", };
	const unsigned n = sizeof(test_keys) / sizeof(*test_keys);
	H_STR *t = h_str_new(1024, free);

	/* set all the keys */
	for (unsigned i = 0; i < n; i++) {
		const char *k = test_keys[i];
		char *v = strdup(test_values[i]);
		h_str_set(t, k, v);
		fprintf(stderr, "INFO:create key=\"%s\" value=\"%s\"\n", k, v);
	}

	/* check that they are set correctly */
	for (unsigned i = 0; i < n; i++) {
		const char *k = test_keys[i];
		const char *v = h_str_get(t, k);
		if (strcmp(v, test_values[i])) {
			fprintf(stderr, "RESULT:FAILURE:test data mismatch:test key=\"%s\" test value=\"%s\" real value=\"%s\"\n", k, test_values[i], v);
			return 1;
		}
	}

	fprintf(stderr, "PASS:create\n");

	/* replace them in a different order */
	for (unsigned i = 0; i < n; i++) {
		const char *k = test_keys[(i * 7) % n];
		char *v = strdup(test_values[i]);
		h_str_replace(t, k, v);
		fprintf(stderr, "INFO:replace key=\"%s\" value=\"%s\"\n", k, v);
	}

	/* check that they are set correctly */
	for (unsigned i = 0; i < n; i++) {
		const char *k = test_keys[(i * 7) % n];
		const char *v = h_str_get(t, k);
		if (strcmp(v, test_values[i])) {
			fprintf(stderr, "RESULT:FAILURE:test data mismatch:test key=\"%s\" test value=\"%s\" real value=\"%s\"\n", k, test_values[i], v);
			return 1;
		}
	}

	fprintf(stderr, "PASS:replace\n");

	h_str_free(t);

	fprintf(stderr, "RESULT:success!\n");

	return 0;
}
