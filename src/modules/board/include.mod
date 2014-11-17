BOARDFILES = board/boardPost.cpp board/board.cpp board/boardManager.cpp board/boardModule.cpp
S_FILES += $(patsubst %.cpp, $(MODULESPATH)/%.cpp, $(BOARDFILES))
