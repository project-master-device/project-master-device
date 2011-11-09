include(${ArduinoCode_SOURCE_DIR}/arduino_diecimila_crosscompile.cmake)

include_directories(${ArduinoCode_SOURCE_DIR}/libarduinocore)
LINK_DIRECTORIES(${ArduinoCode_SOURCE_DIR}/libarduinocore)

link_libraries(arduinocore)

add_executable(none
  main.cpp
)
