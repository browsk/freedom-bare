include ../makefile.conf
NAME=retarget

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin -g

LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

all: $(NAME)-$(CORE).srec

$(NAME)-$(CORE).axf: buffer.c main.c led.c uart.c $(NAME).c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@ -lnosys

%.srec: %.axf
	$(OBJCOPY) -O srec $< $@

clean:
	rm -f *.axf *.map *.o
