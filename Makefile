ROOTDIR=$(CURDIR)

include $(CURDIR)/makefile.conf
NAME=freedom-bare
OUTDIR=bin

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

INCLUDES=-I$(ROOTDIR)/CMSIS/Include -I$(ROOTDIR)/devices -I$(ROOTDIR)/include

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin  $(INCLUDES)
#CFLAGS+= -g $(INCLUDES)

LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP) -L$(LIBDIR)

SUBDIRS = devices

.PHONY: $(SUBDIRS) 

all: tags $(OUTDIR) $(SUBDIRS) $(NAME).srec 

$(SUBDIRS):
	for d in $(SUBDIRS); do make -C $$d; done

LIBS = -ldevices


SOURCES = main.c retarget.c
DEPFILES := $(patsubst %.c,%.d,$(SOURCES))
OBJS := $(patsubst %.c,%.o,$(SOURCES))
LSTFILES := $(patsubst %c,%lst,$(SOURCES))
ASMFILES := $(patsubst %c,%s,$(SOURCES))


%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

$(NAME).axf: $(OBJS) $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@ -lnosys $(LIBS)

%.srec: %.axf
	$(OBJCOPY) -O srec $< $@

$(OUTDIR):
	mkdir -p $@

tags: $(shell find -name "*.c" -o -name "*.h")
	@ctags -R

clean: 
	rm -f *.axf *.map *.o *.srec
	for d in $(SUBDIRS); do make -C $$d clean; done

