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
CC	=c++
#debugging code (should always be used except for special cases).
CFLAGS	= -g -std=c++11 -O1 -march=native
#release:
#CFLAGS = -s -O3 -std=c++11 -DNDEBUG -flto -march=native
#warning flags
CFLAGS += -pedantic -Wall -Wextra -Wno-unused-parameter -Wno-maybe-uninitialized
#CFLAGS += -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wuseless-cast

LDFLAGS= -Xlinker -Map=../bin/output.map

#makes the compiler use our includes.
CFLAGS += -isystem ../include
CFLAGS += -L../lib

#required libraries
LDFLAGS	+= -lz -lcrypto -langelscript -levent -lpthread

#formatting specific flags
FORMATTER = astyle
FORMAT_FLAGS = --style=gnu

###
#modules
#core modules: modules that ship with the mud--do not add anything here without good reason.
COREMODULES = scripts syslog help
#external modules: modules found in the mod directory. Just list the name of the module.
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
property.cpp uuid.cpp olcGroup.cpp olcManager.cpp\
match.cpp calloutManager.cpp optionManager.cpp flag.cpp door.cpp\
optionMeta.cpp bitfield.cpp attribute.cpp affect.cpp objectManager.cpp
S_FILES += modules/modules.cpp
S_FILES += xml/tinystr.cpp  xml/tinyxml.cpp  xml/tinyxmlerror.cpp  xml/tinyxmlparser.cpp  

###
###YOU SHOULD NOT MODIFY ANYTHING PAST THIS POINT.
###IF YOU HAVE CHANGES, MAKE THEM ABOVE TO THE FLAGS.
###

#include the external modules into the makefile:
include $(patsubst %,%/include.mod, $(COREMODULES))
include modules/include.mod
O_FILES += $(patsubst %.cpp,%.o, $(filter %.cpp, $(S_FILES)))

all: $(O_FILES)
	@$(RM) $(OUTPUT)
	@$(ECHO) Linking.
	@$(CC) $(CFLAGS) -o $(OUTPUT) $(O_FILES) $(LDFLAGS)

%.o: %.cpp
	@$(ECHO) Compiling $<.
	@$(CC) $(PROF) -c $(CFLAGS) -o $(patsubst %.cpp,%.o, $<) $<
	
clean:
	@$(ECHO) Cleaning
	@$(RM) $(O_FILES)
	@$(RM) xml/*.o
	@$(RM) $(OUTPUT)

format:
	@$(ECHO) `date '+%H:%M:%S:'` Formatting
	@$(FORMATTER) $(FORMAT_FLAGS) $(S_FILES)
	@$(FORMATTER) $(FORMAT_FLAGS) *.h
	@$(RM) -f *.orig
	@for d in $(MODULES);do cd $$d;$(FORMATTER) $(FORMAT_FLAGS) *.h;$(FORMATTER) $(FORMAT_FLAGS) *.cpp;$(RM) *.orig;cd $(CURDIR);done
	@for d in $(COREMODULES);do cd $$d;$(FORMATTER) $(FORMAT_FLAGS) *.h;$(FORMATTER) $(FORMAT_FLAGS) *.cpp;$(RM) *.orig;cd $(CURDIR);done
	@$(ECHO) Done.
