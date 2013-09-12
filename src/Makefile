include ../makefile.conf
NAME=retarget

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

INCLUDES=-ICMSIS/Include

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin -g $(INCLUDES)

LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

all: $(NAME)-$(CORE).srec

SOURCES := $(shell find -name "*.c")
DEPFILES := $(patsubst %.c,%.d,$(SOURCES))
OBJS := $(patsubst %.c,%.o,$(SOURCES))
LSTFILES := $(patsubst %c,%lst,$(SOURCES))
ASMFILES := $(patsubst %c,%s,$(SOURCES))

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(NAME)-$(CORE).axf: $(OBJS) $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@ -lnosys

%.srec: %.axf
	$(OBJCOPY) -O srec $< $@

clean:
	rm -f *.axf *.map *.o
