.SECONDEXPANSION:
.SUFFIXES:
.PRECIOUS: %.o
.PHONY: all clean
%: %.o ; $(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)
%.o: %.c ; $(CC) -c -o $@ $(CFLAGS) $(CPPFLAGS) $<
O=$(S:.c=.o)
D=$(S:.c=.d)
all::
clean-%: ; $(RM) $* $O $D
-include $(wildcard *.d)
include shash.mk
