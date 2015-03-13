#Tinyxml Aspen Makefile.
#This is not the origenal makefile, it was created specifically for Aspen.
OUTPUT=../lib/libtinyxml.a
AR = ar
RANLIB = ranlib
CC=c++
#general build.
CFLAGS = -I../include -O2 -march=native -s
#release:
#CFLAGS	= -fpic -O3 -s -march=native -I../include -flto

S_FILES = tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp
O_FILES := $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_FILES)))
all: $(O_FILES)
	@echo Building Library
	@rm -f $(OUTPUT)
	@$(AR) cq $(OUTPUT) $(O_FILES)
	@$(RANLIB) $(OUTPUT)
.cpp.o: all
		@echo Compiling $<.
	@$(CC) -c $(CFLAGS) -o $(patsubst %.cpp,%.o, $<) $<
clean:
	@echo Cleaning
	@rm -f *.o
	@rm -f $(OUTPUT)
	
	
	
	
