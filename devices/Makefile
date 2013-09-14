ROOTDIR := ..
include $(ROOTDIR)/makefile.conf

INCLUDES=-I$(ROOTDIR)/CMSIS/Include -I$(ROOTDIR)/include

CFLAGS+=-fno-builtin -g $(INCLUDES)

LIBDIR=$(ROOTDIR)/lib

SOURCES := $(shell find -name "*.c")
DEPFILES := $(patsubst %.c,%.d,$(SOURCES))
OBJS := $(patsubst %.c,%.o,$(SOURCES))
LSTFILES := $(patsubst %c,%lst,$(SOURCES))
ASMFILES := $(patsubst %c,%s,$(SOURCES))

TARGET = $(LIBDIR)/libdevices.a

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(LIBDIR)
	$(AR) r $@ $?

clean:
	rm -f  $(TARGET) $(OBJS)

