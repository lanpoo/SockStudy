# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lanpo/work/code/cpp/TCPAndUDP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lanpo/work/code/cpp/TCPAndUDP/build

# Include any dependencies generated for this target.
include CMakeFiles/clientUDP.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clientUDP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clientUDP.dir/flags.make

CMakeFiles/clientUDP.dir/clientUDP.cpp.o: CMakeFiles/clientUDP.dir/flags.make
CMakeFiles/clientUDP.dir/clientUDP.cpp.o: ../clientUDP.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lanpo/work/code/cpp/TCPAndUDP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clientUDP.dir/clientUDP.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientUDP.dir/clientUDP.cpp.o -c /home/lanpo/work/code/cpp/TCPAndUDP/clientUDP.cpp

CMakeFiles/clientUDP.dir/clientUDP.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientUDP.dir/clientUDP.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lanpo/work/code/cpp/TCPAndUDP/clientUDP.cpp > CMakeFiles/clientUDP.dir/clientUDP.cpp.i

CMakeFiles/clientUDP.dir/clientUDP.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientUDP.dir/clientUDP.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lanpo/work/code/cpp/TCPAndUDP/clientUDP.cpp -o CMakeFiles/clientUDP.dir/clientUDP.cpp.s

# Object files for target clientUDP
clientUDP_OBJECTS = \
"CMakeFiles/clientUDP.dir/clientUDP.cpp.o"

# External object files for target clientUDP
clientUDP_EXTERNAL_OBJECTS =

clientUDP: CMakeFiles/clientUDP.dir/clientUDP.cpp.o
clientUDP: CMakeFiles/clientUDP.dir/build.make
clientUDP: CMakeFiles/clientUDP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lanpo/work/code/cpp/TCPAndUDP/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable clientUDP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clientUDP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clientUDP.dir/build: clientUDP

.PHONY : CMakeFiles/clientUDP.dir/build

CMakeFiles/clientUDP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clientUDP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clientUDP.dir/clean

CMakeFiles/clientUDP.dir/depend:
	cd /home/lanpo/work/code/cpp/TCPAndUDP/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lanpo/work/code/cpp/TCPAndUDP /home/lanpo/work/code/cpp/TCPAndUDP /home/lanpo/work/code/cpp/TCPAndUDP/build /home/lanpo/work/code/cpp/TCPAndUDP/build /home/lanpo/work/code/cpp/TCPAndUDP/build/CMakeFiles/clientUDP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clientUDP.dir/depend

