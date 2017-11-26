CC = gcc
CFLAGS = -m32 -Wall
AR = ar
ARFLAGS = crs

LIB = lib/libt2fs.a
APIDISK = lib/apidisk.o
T2FS = lib/t2fs.o

T2FS_SRC = src/t2fs.c

HEADERS := $(wildcard include/*.h)

all: $(LIB)

$(T2FS): $(T2FS_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(T2FS)
	$(AR) $(ARFLAGS) $@ $(T2FS) $(SUPPORT)

clean:
	-rm -f $(T2FS)
	-rm -f $(LIB)

