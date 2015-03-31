#Frisk aspen makefile
OUTPUT=../../lib/libfrisk.a
AR = ar
RANLIB = ranlib
CFLAGS = -std=c++11 -Wall -c

S_FILES = Test.cpp ConsoleReporter.cpp TestCollection.cpp
O_FILES := $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_FILES)))

all:  $(O_FILES)
	@echo Building Frisk...
	@rm -f $(OUTPUT)
	@$(AR) cq $(OUTPUT) $(O_FILES)
	@$(RANLIB) $(OUTPUT)
	@echo Finished.

.cpp.o: all
		@echo Compiling $<.
	@$(CXX) -c $(CFLAGS) -o $(patsubst %.cpp,%.o, $<) $<
clean:
	@echo Cleaning
	@rm -f *.o
