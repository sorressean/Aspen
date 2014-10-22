###
#Add your module code here.
#You can either use an include like so:
#include foo/include.mod
#Or you can just add to the S_FILES variable:
#S_FILES += foo/foo.cpp foo/bar.cpp foobar/foo.cpp etc.

S_FILES += modules/modules.cpp
MODULESPATH=modules
include $(MODULESPATH)/board/include.mod
