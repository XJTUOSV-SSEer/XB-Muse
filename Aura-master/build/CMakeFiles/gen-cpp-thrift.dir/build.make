# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/node10/yangxu/chensifan/XB-Muse/Aura-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/node10/yangxu/chensifan/XB-Muse/Aura-master/build

# Include any dependencies generated for this target.
include CMakeFiles/gen-cpp-thrift.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gen-cpp-thrift.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gen-cpp-thrift.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gen-cpp-thrift.dir/flags.make

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o: CMakeFiles/gen-cpp-thrift.dir/flags.make
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o: ../gen-cpp/SSEService.cpp
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o: CMakeFiles/gen-cpp-thrift.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/node10/yangxu/chensifan/XB-Muse/Aura-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o -MF CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o.d -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o -c /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/SSEService.cpp

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/SSEService.cpp > CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.i

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/SSEService.cpp -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.s

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o: CMakeFiles/gen-cpp-thrift.dir/flags.make
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o: ../gen-cpp/Server_constants.cpp
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o: CMakeFiles/gen-cpp-thrift.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/node10/yangxu/chensifan/XB-Muse/Aura-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o -MF CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o.d -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o -c /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_constants.cpp

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_constants.cpp > CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.i

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_constants.cpp -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.s

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o: CMakeFiles/gen-cpp-thrift.dir/flags.make
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o: ../gen-cpp/Server_types.cpp
CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o: CMakeFiles/gen-cpp-thrift.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/node10/yangxu/chensifan/XB-Muse/Aura-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o -MF CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o.d -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o -c /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_types.cpp

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_types.cpp > CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.i

CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/node10/yangxu/chensifan/XB-Muse/Aura-master/gen-cpp/Server_types.cpp -o CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.s

# Object files for target gen-cpp-thrift
gen__cpp__thrift_OBJECTS = \
"CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o" \
"CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o" \
"CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o"

# External object files for target gen-cpp-thrift
gen__cpp__thrift_EXTERNAL_OBJECTS =

libgen-cpp-thrift.a: CMakeFiles/gen-cpp-thrift.dir/gen-cpp/SSEService.cpp.o
libgen-cpp-thrift.a: CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_constants.cpp.o
libgen-cpp-thrift.a: CMakeFiles/gen-cpp-thrift.dir/gen-cpp/Server_types.cpp.o
libgen-cpp-thrift.a: CMakeFiles/gen-cpp-thrift.dir/build.make
libgen-cpp-thrift.a: CMakeFiles/gen-cpp-thrift.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/node10/yangxu/chensifan/XB-Muse/Aura-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libgen-cpp-thrift.a"
	$(CMAKE_COMMAND) -P CMakeFiles/gen-cpp-thrift.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gen-cpp-thrift.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gen-cpp-thrift.dir/build: libgen-cpp-thrift.a
.PHONY : CMakeFiles/gen-cpp-thrift.dir/build

CMakeFiles/gen-cpp-thrift.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gen-cpp-thrift.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gen-cpp-thrift.dir/clean

CMakeFiles/gen-cpp-thrift.dir/depend:
	cd /home/node10/yangxu/chensifan/XB-Muse/Aura-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/node10/yangxu/chensifan/XB-Muse/Aura-master /home/node10/yangxu/chensifan/XB-Muse/Aura-master /home/node10/yangxu/chensifan/XB-Muse/Aura-master/build /home/node10/yangxu/chensifan/XB-Muse/Aura-master/build /home/node10/yangxu/chensifan/XB-Muse/Aura-master/build/CMakeFiles/gen-cpp-thrift.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gen-cpp-thrift.dir/depend

