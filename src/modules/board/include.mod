MODULEPATH=board
FULLPATH=$(MODULESPATH)/$(MODULEPATH)/
FILES += boardPost.cpp board.cpp boardManager.cpp boardModule.cpp
S_FILES += $(patsubst %.cpp, $(FULLPATH)%.cpp, $(FILES))
