add_metasprite_asset(SOURCE "metasprites.nss" TARGET "ui-metasprites.cpp"
                     HEADER "ui-metasprites.hpp" NAMESPACE "UI" BANK "fixed")

add_raw_assets(BANK -1)

add_library(SourceObj
  OBJECT

  main.cpp
  global.cpp
  phone.cpp

  board.cpp
  snake.cpp

  attributes.cpp
  input.cpp
  ggsound.cpp
  log.cpp
  metasprites.s
  position.cpp
  soundtrack.hpp

  ${CMAKE_CURRENT_BINARY_DIR}/ui-metasprites.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/assets.cpp
)

set_property(
  SOURCE
  soundtrack.s
  PROPERTY
  OBJECT_DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/soundtrack.asm
  # ${CMAKE_CURRENT_BINARY_DIR}/soundtrack_dpcm.asm
  ${CMAKE_CURRENT_SOURCE_DIR}/ggsound.inc
)

set_target_properties(SourceObj PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(
  SourceObj
  PRIVATE
  ${CMAKE_BINARY_DIR}/generated
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_options(SourceObj PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-Wno-c23-extensions>
  $<$<COMPILE_LANGUAGE:CXX>:--embed-dir=${CMAKE_SOURCE_DIR}/assets>
)

