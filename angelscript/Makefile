#Angelscript Aspen Makefile.
#This is not the origenal makefile, it was created specifically for Aspen.
OUTPUT=../lib/libangelscript.a
AR = ar
RANLIB = ranlib
CFLAGS = -O2 -march=native -s -I../include

S_FILES = as_atomic.cpp as_builder.cpp as_bytecode.cpp as_callfunc.cpp as_callfunc_x86.cpp\
as_callfunc_x64_gcc.cpp as_compiler.cpp as_configgroup.cpp as_context.cpp as_datatype.cpp\
as_gc.cpp as_generic.cpp as_globalproperty.cpp as_memory.cpp as_module.cpp\
as_objecttype.cpp as_outputbuffer.cpp as_parser.cpp as_restore.cpp as_scriptcode.cpp\
as_scriptengine.cpp as_scriptfunction.cpp as_scriptnode.cpp as_scriptobject.cpp as_string.cpp\
as_string_util.cpp as_thread.cpp as_tokenizer.cpp as_typeinfo.cpp as_variablescope.cpp\
scriptstdstring.cpp scriptstdstring_utils.cpp scriptarray.cpp contextmgr.cpp\
scripthelper.cpp scriptdictionary.cpp scriptmath.cpp scriptgrid.cpp

include ../src/extensions/make.inc

O_FILES := $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_FILES)))
all: $(O_FILES)
	@echo Building Library
	@rm -f $(OUTPUT)
	@$(AR) cq $(OUTPUT) $(O_FILES)
	@$(RANLIB) $(OUTPUT)
.cpp.o: all
		@echo Compiling $<.
	@$(CXX) -c $(CFLAGS) -o $(patsubst %.cpp,%.o, $<) $<
clean:
	@echo Cleaning
	@rm -f *.o
	@rm -f $(OUTPUT)
