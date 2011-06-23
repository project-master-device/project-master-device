include(${ArduinoCode_SOURCE_DIR}/arduino_diecimila_crosscompile.cmake)

add_executable(at90_can_test
  main.c
  can.c
)
