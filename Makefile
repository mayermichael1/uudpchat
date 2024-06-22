# default variables
CXX := clang++
CXXFLAGS := -Wall -Wextra -Werror
LDFLAGS :=
LDLIBS := 
CPPFLAGS :=

BUILDDIR := ./build

OBJECTS := ${BUILDDIR}/main.o
OBJECTS += ${BUILDDIR}/tui.o
OBJECTS += ${BUILDDIR}/server.o
OBJECTS += ${BUILDDIR}/client.o

CXXFLAGS += -O0 -g 
# CXXFLAGS += -O2

# link the target here
${BUILDDIR}/uudpchat: builddir ${OBJECTS}
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BUILDDIR)/uudpchat $(LDFLAGS) $(LDLIBS)

# compile all objects here
${BUILDDIR}/main.o: main.cpp include/tui.h include/networking.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp -o $(BUILDDIR)/main.o

${BUILDDIR}/tui.o: src/tui.cpp include/tui.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c src/tui.cpp -o $(BUILDDIR)/tui.o

${BUILDDIR}/server.o: src/server.cpp include/server.h include/networking.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c src/server.cpp -o $(BUILDDIR)/server.o

${BUILDDIR}/client.o: src/client.cpp include/client.h include/networking.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c src/client.cpp -o $(BUILDDIR)/client.o

#${BUILDDIR}/src.o: src/src.cpp include/src.h
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c src/src.cpp -o $(BUILDDIR)/src.o

${BUILDDIR}/client_time_wrapper: builddir client_time_wrapper.cpp
	$(CXX) $(CXXFLAGS) client_time_wrapper.cpp -o $(BUILDDIR)/client_time_wrapper $(LDFLAGS) $(LDLIBS)

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

