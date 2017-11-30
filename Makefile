CC = gcc
CFLAGS = -m32 -Wall
AR = ar
ARFLAGS = crs

LIB = lib/libt2fs.a
APIDISK = lib/apidisk.o
T2FS = lib/t2fs.o
FILEOP = lib/file_operations.o
FSMANAGER = lib/fsmanager.o

T2FS_SRC = src/t2fs.c
FILEOP_SRC = src/file_operations.c
FSMANAGER_SRC = src/fsmanager.c

HEADERS := $(wildcard include/*.h)

all: $(LIB)

$(T2FS): $(T2FS_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FILEOP): $(FILEOP_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(FSMANAGER): $(FSMANAGER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(T2FS) $(FILEOP) $(FSMANAGER)
	$(AR) $(ARFLAGS) $@ $(T2FS) $(FILEOP) $(FSMANAGER) $(APIDISK)

clean:
	-rm -f $(T2FS)
	-rm -f $(FILEOP)
	-rm -f $(FSMANAGER)
	-rm -f $(LIB)

