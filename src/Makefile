#aspen Makefile
#compiler and project specific variables
OUTPUT=../bin/aspen

#program defines
MAKE=make -s
RM = rm -f
ECHO = echo
#compiler flags:
#enable if you want to profile.
PROF =
LDFLAGS= -L../lib
CFLAGS	= -g -std=c++11 -O0 -march=native -Wall -pedantic -I../include

#makes the compiler use our includes.
CFLAGS += -isystem ../include -I../frisk/inc
CFLAGS += -L../lib

#required libraries
LDFLAGS	= -L../lib -lz -lcrypto -langelscript -ltinyxml -lpthread

#formatting specific flags
FORMATTER = astyle
FORMAT_FLAGS = --style=gnu

###
#modules
#core modules: modules that ship with the mud--do not add anything here without good reason.
COREMODULES = scripts syslog help
#external modules: modules found in the modules directory. Just list the name of the module.
#e.g: MODULES = quest board
MODULES =

#source files to compile:
S_FILES= world.cpp variant.cpp baseObject.cpp entity.cpp player.cpp\
socket.cpp utils.cpp log.cpp server.cpp main.cpp\
exception.cpp command.cpp com_gen.cpp com_wiz.cpp event.cpp\
delayedEvent.cpp eventManager.cpp staticObject.cpp living.cpp channel.cpp\
modules.cpp component.cpp npc.cpp editor.cpp exit.cpp\
room.cpp zone.cpp com_builder.cpp com_movement.cpp ComponentFactory.cpp\
components.cpp olc.cpp socials.cpp olcs.cpp callout.cpp\
option.cpp baseSocket.cpp banList.cpp listener.cpp inputHandlers.cpp\
property.cpp uuid.cpp olcGroup.cpp olcManager.cpp playerManager.cpp\
match.cpp calloutManager.cpp optionManager.cpp flag.cpp door.cpp\
optionMeta.cpp bitfield.cpp attribute.cpp affect.cpp objectManager.cpp\
page.cpp serializationHelpers.cpp telnetParser.cpp objectContainer.cpp

S_TEST_FILES=$(filter-out main.cpp, $(S_FILES))
S_TEST_FILES+=../tests/test_main.cpp


###
###YOU SHOULD NOT MODIFY ANYTHING PAST THIS POINT.
###IF YOU HAVE CHANGES, MAKE THEM ABOVE TO THE FLAGS.
###

#include the external modules into the makefile:
include $(patsubst %,%/include.mod, $(COREMODULES))
#external modules that ship with Aspen:
include modules/include.mod
#user-defined modules:
include extensions/include.mod
O_FILES += $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_FILES)))
O_TEST_FILES = $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_TEST_FILES)))


all: $(O_FILES) ../lib/libangelscript.a ../lib/libtinyxml.a
	@$(RM) $(OUTPUT)
	@$(ECHO) Linking.
	@$(CXX) $(CFLAGS) -o $(OUTPUT) $(O_FILES) $(LDFLAGS)

test: $(O_TEST_FILES) ../lib/libangelscript.a ../lib/libtinyxml.a ../lib/libfrisk.a
	@$(RM) $(OUTPUT)
	@$(ECHO) Linking.
	@$(CXX) $(CFLAGS) -o ../tests/test $(O_TEST_FILES) -lfrisk $(LDFLAGS)
	cd ../tests && ./test

%.o: %.cpp
	@$(ECHO) Compiling $<.
	@$(CXX) $(PROF) -c $(CFLAGS) -o $(patsubst %.cpp,%.o, $<) $<

../lib/libangelscript.a:
	@cd ../angelscript && $(MAKE)
../lib/libtinyxml.a:
	@cd ../xml && $(MAKE)
../lib/libfrisk.a:
	@cd ../frisk/src && $(MAKE)

clean:
	@$(ECHO) Cleaning
	@$(RM) $(O_FILES)
	@$(RM) xml/*.o
	@$(RM) $(OUTPUT)

format:
	@$(ECHO) Formatting
	@$(FORMATTER) $(FORMAT_FLAGS) $(S_FILES)
	@$(FORMATTER) $(FORMAT_FLAGS) *.h
	@$(FORMATTER) $(FORMAT_FLAGS) *.hpp
	@$(RM) -f *.orig
	@for d in $(MODULES);do cd $$d;$(FORMATTER) $(FORMAT_FLAGS) *.h;$(FORMATTER) $(FORMAT_FLAGS) *.cpp;$(RM) *.orig;cd $(CURDIR);done
	@for d in $(COREMODULES);do cd $$d;$(FORMATTER) $(FORMAT_FLAGS) *.h;$(FORMATTER) $(FORMAT_FLAGS) *.hpp;$(FORMATTER) $(FORMAT_FLAGS) *.cpp;$(RM) *.orig;cd $(CURDIR);done
	@$(ECHO) Done.
