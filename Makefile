# This Program was written by:
# 
# Garrett O'Hara cssc1136 RedId: 822936303
# 
# CS 480 | Professor Shen | March 2022

CXX=g++
CXXFLAGS=-std=c++11 -g
CFLAGS = -c -Wall -Iinclude


# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
tlb : paging.o tree.o level.o mymap.o cache.o tracereader.h tracereader.c output_mode_helpers.h output_mode_helpers.c summary.h summary.cpp
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^ 

tree.o : tree.h tree.cpp

level.o : level.h level.cpp

paging.o : arguments.h paging.cpp

mymap.o : mymap.h mymap.cpp

cache.o : cache.h cache.cpp

clean :
	rm *.o

