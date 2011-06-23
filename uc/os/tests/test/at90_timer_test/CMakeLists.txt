include(${ArduinoCode_SOURCE_DIR}/arduino_diecimila_crosscompile.cmake)

add_executable(at90_timer_test
  main.c
  devices.c
)
