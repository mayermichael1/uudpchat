# default variables
CXX := clang
CXXFLAGS := -Wall -Wextra -Werror
LDFLAGS :=
LDLIBS := 
CPPFLAGS :=

BUILDDIR := ./build

OBJECTS := ${BUILDDIR}/main.o
#OBJECTS += ${BUILDDIR}/src.o

CXXFLAGS += -Og -g 
# CXXFLAGS += -O2

# link the target here
${BUILDDIR}/uudpchat: builddir ${OBJECTS}
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BUILDDIR)/uudpchat $(LDFLAGS) $(LDLIBS)

# compile all objects here
${BUILDDIR}/main.o: main.cpp #includes 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp -o $(BUILDDIR)/main.o

#${BUILDDIR}/src.o: src/src.cpp include/src.h
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c src/src.cpp -o $(BUILDDIR)/src.o


# create build folder if not exists
.PHONY: builddir
builddir:  
	mkdir -p $(BUILDDIR)

# clean up build directory
.PHONY: clean
clean: 
	rm -rf ${OBJECTS}
	rm -rf ${BUILDDIR}/uudpchat
	rm -f ./compile_commands.json

