# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fs/Programming/arduino/workspaces/learning/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fs/Programming/arduino/workspaces/learning/test/build

# Include any dependencies generated for this target.
include libarduinocore/CMakeFiles/arduinocore.dir/depend.make

# Include the progress variables for this target.
include libarduinocore/CMakeFiles/arduinocore.dir/progress.make

# Include the compile flags for this target's objects.
include libarduinocore/CMakeFiles/arduinocore.dir/flags.make

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o: ../libarduinocore/HardwareSerial.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/HardwareSerial.cpp

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arduinocore.dir/HardwareSerial.cpp.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/HardwareSerial.cpp > CMakeFiles/arduinocore.dir/HardwareSerial.cpp.i

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arduinocore.dir/HardwareSerial.cpp.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/HardwareSerial.cpp -o CMakeFiles/arduinocore.dir/HardwareSerial.cpp.s

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o: ../libarduinocore/Print.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arduinocore.dir/Print.cpp.o -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Print.cpp

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arduinocore.dir/Print.cpp.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Print.cpp > CMakeFiles/arduinocore.dir/Print.cpp.i

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arduinocore.dir/Print.cpp.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Print.cpp -o CMakeFiles/arduinocore.dir/Print.cpp.s

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o: ../libarduinocore/wiring.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/wiring.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring.c

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/wiring.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring.c > CMakeFiles/arduinocore.dir/wiring.c.i

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/wiring.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring.c -o CMakeFiles/arduinocore.dir/wiring.c.s

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o: ../libarduinocore/wiring_shift.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/wiring_shift.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_shift.c

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/wiring_shift.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_shift.c > CMakeFiles/arduinocore.dir/wiring_shift.c.i

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/wiring_shift.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_shift.c -o CMakeFiles/arduinocore.dir/wiring_shift.c.s

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o: ../libarduinocore/WInterrupts.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/WInterrupts.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WInterrupts.c

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/WInterrupts.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WInterrupts.c > CMakeFiles/arduinocore.dir/WInterrupts.c.i

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/WInterrupts.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WInterrupts.c -o CMakeFiles/arduinocore.dir/WInterrupts.c.s

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o: ../libarduinocore/wiring_digital.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/wiring_digital.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_digital.c

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/wiring_digital.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_digital.c > CMakeFiles/arduinocore.dir/wiring_digital.c.i

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/wiring_digital.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_digital.c -o CMakeFiles/arduinocore.dir/wiring_digital.c.s

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o: ../libarduinocore/WMath.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arduinocore.dir/WMath.cpp.o -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WMath.cpp

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arduinocore.dir/WMath.cpp.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WMath.cpp > CMakeFiles/arduinocore.dir/WMath.cpp.i

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arduinocore.dir/WMath.cpp.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WMath.cpp -o CMakeFiles/arduinocore.dir/WMath.cpp.s

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o: ../libarduinocore/Tone.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arduinocore.dir/Tone.cpp.o -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Tone.cpp

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arduinocore.dir/Tone.cpp.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Tone.cpp > CMakeFiles/arduinocore.dir/Tone.cpp.i

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arduinocore.dir/Tone.cpp.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/Tone.cpp -o CMakeFiles/arduinocore.dir/Tone.cpp.s

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o: ../libarduinocore/WString.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/arduinocore.dir/WString.cpp.o -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WString.cpp

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/arduinocore.dir/WString.cpp.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WString.cpp > CMakeFiles/arduinocore.dir/WString.cpp.i

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/arduinocore.dir/WString.cpp.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/WString.cpp -o CMakeFiles/arduinocore.dir/WString.cpp.s

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o: ../libarduinocore/pins_arduino.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/pins_arduino.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/pins_arduino.c

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/pins_arduino.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/pins_arduino.c > CMakeFiles/arduinocore.dir/pins_arduino.c.i

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/pins_arduino.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/pins_arduino.c -o CMakeFiles/arduinocore.dir/pins_arduino.c.s

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o: ../libarduinocore/wiring_analog.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/wiring_analog.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_analog.c

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/wiring_analog.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_analog.c > CMakeFiles/arduinocore.dir/wiring_analog.c.i

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/wiring_analog.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_analog.c -o CMakeFiles/arduinocore.dir/wiring_analog.c.s

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.provides.build

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o: libarduinocore/CMakeFiles/arduinocore.dir/flags.make
libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o: ../libarduinocore/wiring_pulse.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/fs/Programming/arduino/workspaces/learning/test/build/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/arduinocore.dir/wiring_pulse.c.o   -c /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_pulse.c

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/arduinocore.dir/wiring_pulse.c.i"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_pulse.c > CMakeFiles/arduinocore.dir/wiring_pulse.c.i

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/arduinocore.dir/wiring_pulse.c.s"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && avr-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore/wiring_pulse.c -o CMakeFiles/arduinocore.dir/wiring_pulse.c.s

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.requires:
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.requires

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.provides: libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.requires
	$(MAKE) -f libarduinocore/CMakeFiles/arduinocore.dir/build.make libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.provides.build
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.provides

libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.provides.build: libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.provides.build

# Object files for target arduinocore
arduinocore_OBJECTS = \
"CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o" \
"CMakeFiles/arduinocore.dir/Print.cpp.o" \
"CMakeFiles/arduinocore.dir/wiring.c.o" \
"CMakeFiles/arduinocore.dir/wiring_shift.c.o" \
"CMakeFiles/arduinocore.dir/WInterrupts.c.o" \
"CMakeFiles/arduinocore.dir/wiring_digital.c.o" \
"CMakeFiles/arduinocore.dir/WMath.cpp.o" \
"CMakeFiles/arduinocore.dir/Tone.cpp.o" \
"CMakeFiles/arduinocore.dir/WString.cpp.o" \
"CMakeFiles/arduinocore.dir/pins_arduino.c.o" \
"CMakeFiles/arduinocore.dir/wiring_analog.c.o" \
"CMakeFiles/arduinocore.dir/wiring_pulse.c.o"

# External object files for target arduinocore
arduinocore_EXTERNAL_OBJECTS =

libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/build.make
libarduinocore/libarduinocore.a: libarduinocore/CMakeFiles/arduinocore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libarduinocore.a"
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && $(CMAKE_COMMAND) -P CMakeFiles/arduinocore.dir/cmake_clean_target.cmake
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/arduinocore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libarduinocore/CMakeFiles/arduinocore.dir/build: libarduinocore/libarduinocore.a
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/build

libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/HardwareSerial.cpp.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/Print.cpp.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/wiring.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/wiring_shift.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/WInterrupts.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/wiring_digital.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/WMath.cpp.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/Tone.cpp.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/WString.cpp.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/pins_arduino.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/wiring_analog.c.o.requires
libarduinocore/CMakeFiles/arduinocore.dir/requires: libarduinocore/CMakeFiles/arduinocore.dir/wiring_pulse.c.o.requires
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/requires

libarduinocore/CMakeFiles/arduinocore.dir/clean:
	cd /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore && $(CMAKE_COMMAND) -P CMakeFiles/arduinocore.dir/cmake_clean.cmake
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/clean

libarduinocore/CMakeFiles/arduinocore.dir/depend:
	cd /home/fs/Programming/arduino/workspaces/learning/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fs/Programming/arduino/workspaces/learning/test /home/fs/Programming/arduino/workspaces/learning/test/libarduinocore /home/fs/Programming/arduino/workspaces/learning/test/build /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore /home/fs/Programming/arduino/workspaces/learning/test/build/libarduinocore/CMakeFiles/arduinocore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libarduinocore/CMakeFiles/arduinocore.dir/depend

