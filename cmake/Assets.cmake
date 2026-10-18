function(add_raw_assets)
  set(options)
  set(oneValueArgs TARGET HEADER BANK)
  set(multiValueArgs)
  cmake_parse_arguments(
    ASSET
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
  )

  if (NOT ASSET_BANK)
    message(FATAL_ERROR "Starting bank BANK is required!")
  endif()

  find_program(
    GENERATE_ASSETS_DATA
    generate-assets-data
    PATHS "${CMAKE_SOURCE_DIR}/tools"
  )
  if (NOT GENERATE_ASSETS_DATA)
    message(FATAL_ERROR "The generate-assets-data tool is required!")
  endif()

  set(ASSET_SRC "${CMAKE_SOURCE_DIR}/assets")

  if (ASSET_TARGET)
    set(ASSET_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_TARGET}")
  else()
    set(ASSET_DEST "${CMAKE_CURRENT_BINARY_DIR}/assets.cpp")
  endif()

  if (ASSET_HEADER)
    set(ASSET_HEADER_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_HEADER}")
  else()
    set(ASSET_HEADER_DEST "${CMAKE_CURRENT_BINARY_DIR}/assets.hpp")
  endif()

  file(GLOB ASSETS_FILES CONFIGURE_DEPENDS "${ASSET_SRC}/*")

  add_custom_command(
    OUTPUT ${ASSET_DEST} ${ASSET_HEADER_DEST}
    COMMAND ${GENERATE_ASSETS_DATA}
            generate ${ASSET_DEST} ${ASSET_HEADER_DEST} ${ASSET_SRC}
            --starting-bank ${ASSET_BANK} --chr-rom
    DEPENDS ${GENERATE_ASSETS_DATA} ${ASSETS_FILES}
  )
endfunction()

function(add_metasprite_asset)
  set(options)
  set(oneValueArgs SOURCE TARGET HEADER NAMESPACE)
  set(multiValueArgs)
  cmake_parse_arguments(ASSET "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN})

  if (NOT ASSET_SOURCE)
    message(FATAL_ERROR "Raw asset FILE is required!")
  endif()

  find_program(
    GENERATE_METASPRITES
    generate-metasprites
    PATHS "${CMAKE_SOURCE_DIR}/tools"
  )
  if (NOT GENERATE_METASPRITES)
    message(FATAL_ERROR "The generate-metasprites tool is required!")
  endif()

  set(ASSET_SRC "${CMAKE_SOURCE_DIR}/assets/${ASSET_SOURCE}")

  if (ASSET_TARGET)
    set(ASSET_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_TARGET}")
  else()
    set(ASSET_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_SOURCE}.cpp")
  endif()

  if (ASSET_HEADER)
    set(ASSET_HEADER_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_HEADER}")
  else()
    set(ASSET_DEST "${CMAKE_CURRENT_BINARY_DIR}/${ASSET_SOURCE}.hpp")
  endif()

  get_filename_component(BASE_NAME ${ASSET_DEST} NAME)
  get_filename_component(BASE_HEADER_NAME ${ASSET_HEADER_DEST} NAME)

  add_custom_command(OUTPUT ${ASSET_DEST} ${ASSET_HEADER_DEST} COMMAND
                     ${GENERATE_METASPRITES} generate ${ASSET_DEST}
                     ${ASSET_HEADER_DEST} ${ASSET_SRC} --namespace
                     ${ASSET_NAMESPACE} DEPENDS ${GENERATE_METASPRITES}
                     ${ASSET_SRC})
endfunction()
