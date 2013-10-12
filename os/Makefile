ROOTDIR := ..
include $(ROOTDIR)/makefile.conf

INCLUDES=-I$(ROOTDIR)/CMSIS/Include -I$(ROOTDIR)/include -I$(ROOTDIR)/devices

CFLAGS+=-fno-builtin $(INCLUDES) -g

LIBDIR=$(ROOTDIR)/lib

SOURCES := $(shell find -name "*.c")
DEPFILES := $(patsubst %.c,%.d,$(SOURCES))
OBJS := $(patsubst %.c,%.o,$(SOURCES))
LSTFILES := $(patsubst %c,%lst,$(SOURCES))
ASMFILES := $(patsubst %c,%s,$(SOURCES))

TARGET = $(LIBDIR)/libos.a

all: $(TARGET)

$(TARGET): $(OBJS) switch.o
	mkdir -p $(LIBDIR)
	$(AR) r $@ $?

clean:
	rm -f  $(TARGET) $(OBJS)

