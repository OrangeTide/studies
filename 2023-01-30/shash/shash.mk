all:: test_shash
clean:: clean-test_shash
test_shash clean-test_shash: S=test_shash.c shash.c
test_shash: $$O
test_shash: CFLAGS += -Wall -W -O0 -g -MMD
