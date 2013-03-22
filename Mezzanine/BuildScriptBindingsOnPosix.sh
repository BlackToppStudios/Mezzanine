#!/bin/bash
cd src

cat Scripting/Lua51/SwigHeader.txt > scriptbindinglua51.cpp
swig2.0 -c++ -v -Wextra -lua -includeall -o scriptbindinglua51.cpp.tmp mezzanine.h
cat scriptbindinglua51.cpp.tmp >> scriptbindinglua51.cpp
cat Scripting/Lua51/SwigFooter.txt >> scriptbindinglua51.cpp
rm scriptbindinglua51.cpp.tmp
mv scriptbindinglua51.cpp Scripting/Lua51/

cat Scripting/Lua51/SwigHeader.txt > scriptbindinglua51unsafe.cpp
swig2.0 -c++ -v -DSWIG_UNSAFE -Wextra -lua -includeall -o scriptbindinglua51unsafe.cpp.tmp mezzanine.h
cat scriptbindinglua51unsafe.cpp.tmp >> scriptbindinglua51unsafe.cpp
cat Scripting/Lua51/SwigFooter.txt >> scriptbindinglua51unsafe.cpp
rm scriptbindinglua51unsafe.cpp.tmp
mv scriptbindinglua51unsafe.cpp Scripting/Lua51/

