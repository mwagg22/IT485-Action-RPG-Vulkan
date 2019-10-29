##############################################################################
#
# The Linux-GCC Makefile
#
##############################################################################


#
# Compiler stuff -- adjust to your system.
#

# Linux
PROJECT = sj
CC      = gcc
#CC      = clang

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))

LIB_PATH = ../libs

INC_PATHS = ../include
INC_PARAMS =$(foreach d, $(INC_PATHS), -I$d)

LFLAGS = -g  -shared -Wl,-soname,lib$(PROJECT).so.1 -o $(LIB_PATH)/lib$(PROJECT).so.1 -lm
CFLAGS = -g  -fPIC -Wall -pedantic -std=gnu99 -fgnu89-inline -Wno-unknown-pragmas -Wno-variadic-macros
# -fgnu89-inline 
# -std=gnu99 
# -ffast-math for relase version

DOXYGEN = doxygen

#
# Targets
#

$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) $(INC_PARAMS)

docs:
	$(DOXYGEN) ../docs/Doxyfile

sources:
	echo (patsubst %.c,%.o,$(wildcard *.c)) > makefile.sources


makefile.dep: depend

depend:
	@touch makefile.dep
	@-rm makefile.dep
	@echo Creating dependencies.
	@for i in *.c; do $(CC) $(INC) -MM $(INC_PARAMS) $$i; done > makefile.dep
	@echo Done.

clean:
	rm *.o $(LIB_PATH)/lib$(PROJECT).a

count:
	wc -l *.c $(foreach d, $(INC_PATHS), $d/*.h) makefile

static:
	ar rcs $(LIB_PATH)/lib$(PROJECT).a $(OBJECTS)

#
# Dependencies.
#

include makefile.dep

#
# The default rule.
#

.c.o:
	$(CC) $(CFLAGS) $(INC_PARAMS) -c $<


