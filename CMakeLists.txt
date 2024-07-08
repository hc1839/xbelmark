cmake_minimum_required(VERSION 3.14)
project(xbelmark)

# User options.

set(
  BUILD_TEST
  OFF
  CACHE
  BOOL
  "Whether to build the test executable."
)

# Language settings.

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Includes.

include(GNUInstallDirs)

# Environment variables.

set(CPATH $ENV{CPATH})
if(NOT WIN32)
  string(REPLACE ":" ";" CPATH "${CPATH}")
endif()

set(C_INCLUDE_PATH $ENV{C_INCLUDE_PATH})
if(NOT WIN32)
  string(REPLACE ":" ";" C_INCLUDE_PATH "${C_INCLUDE_PATH}")
endif()

set(CPLUS_INCLUDE_PATH $ENV{CPLUS_INCLUDE_PATH})
if(NOT WIN32)
  string(REPLACE ":" ";" CPLUS_INCLUDE_PATH "${CPLUS_INCLUDE_PATH}")
endif()

set(LIBRARY_PATH $ENV{LIBRARY_PATH})
if(NOT WIN32)
  string(REPLACE ":" ";" LIBRARY_PATH "${LIBRARY_PATH}")
endif()

# Find third-party packages and libraries.

find_package(
  Qt6
  REQUIRED
  COMPONENTS
    Core
    Gui
    Network
    Widgets
)

find_library(
  XML2_LIB
  xml2
  PATHS
    ${LIBRARY_PATH}
  REQUIRED
)

find_library(
  XSLT_LIB
  xslt
  PATHS
    ${LIBRARY_PATH}
  REQUIRED
)

qt_standard_project_setup()

# Add subdirectories and definitions.

set(SRC_MAIN_DIR ${PROJECT_SOURCE_DIR}/src/main)
set(SRC_MAIN_CPP_DIR ${SRC_MAIN_DIR}/cpp)
set(SRC_MAIN_CPP_PROJECT_DIR ${SRC_MAIN_CPP_DIR}/${PROJECT_NAME})

set(BUILD_SRC_MAIN_DIR ${PROJECT_BINARY_DIR}/src/main)
set(BUILD_SRC_MAIN_CPP_DIR ${BUILD_SRC_MAIN_DIR}/cpp)
set(BUILD_SRC_MAIN_CPP_PROJECT_DIR ${BUILD_SRC_MAIN_CPP_DIR}/${PROJECT_NAME})

add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR})
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/datetime)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/enumeration)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/html)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/memory)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/paste)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/xml)
add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/xslt)

if(WIN32)
  add_compile_definitions("WIN32")
  add_subdirectory(${SRC_MAIN_CPP_PROJECT_DIR}/winshell)
endif()

# Build include directives.
set(HDR_INCS ${HDR_NAMES})
list(TRANSFORM HDR_INCS PREPEND "#include \"${PROJECT_NAME}/")
list(TRANSFORM HDR_INCS APPEND "\"")
string(REPLACE ";" "\n" HDR_INCS ${HDR_INCS})

# Configure the all-inclusive header file.
configure_file(
  ${SRC_MAIN_CPP_DIR}/${PROJECT_NAME}.h.in
  ${BUILD_SRC_MAIN_CPP_DIR}/${PROJECT_NAME}.h
  @ONLY
)

# Build paths to source files.
set(SRC_PATHS ${SRC_NAMES})
list(TRANSFORM SRC_PATHS PREPEND ${SRC_MAIN_CPP_PROJECT_DIR}/)
set(BUILD_SRC_PATHS ${BUILD_SRC_NAMES})
list(TRANSFORM BUILD_SRC_PATHS PREPEND ${BUILD_SRC_MAIN_CPP_PROJECT_DIR}/)
set(SRCS ${SRC_PATHS} ${BUILD_SRC_PATHS})

# Project libraries and executables.

add_library(
  ${PROJECT_NAME}lib
  STATIC
  ${SRCS}
)

target_include_directories(
  ${PROJECT_NAME}lib
  PUBLIC
  ${BUILD_SRC_MAIN_CPP_DIR}
  ${CPATH}
  ${CPLUS_INCLUDE_PATH}
  ${SRC_MAIN_CPP_DIR}
)

target_link_libraries(
  ${PROJECT_NAME}lib
  ${XML2_LIB}
  ${XSLT_LIB}
  Qt6::Core
  Qt6::Gui
  Qt6::Network
  Qt6::Widgets
)

install(
  TARGETS ${PROJECT_NAME}lib
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(TARGET_NAMES ${PROJECT_NAME})
if(WIN32)
  list(APPEND TARGET_NAMES ${PROJECT_NAME}w)
endif()

foreach(TARGET_NAME ${TARGET_NAMES})
  add_executable(
    ${TARGET_NAME}
    ${SRC_MAIN_CPP_DIR}/main.cc
  )
  target_link_libraries(
    ${TARGET_NAME}
    ${PROJECT_NAME}lib
  )
  if(${TARGET_NAME} STREQUAL ${PROJECT_NAME}w)
    set_target_properties(
      ${TARGET_NAME}
      PROPERTIES
        WIN32_EXECUTABLE ON
    )
  endif()
  install(
    TARGETS ${TARGET_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endforeach()

install(
  DIRECTORY
    ${SRC_MAIN_CPP_DIR}/
    ${BUILD_SRC_MAIN_CPP_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  OPTIONAL
  FILES_MATCHING
  PATTERN "*.h*"
  PATTERN "*.in" EXCLUDE
)

install(
  DIRECTORY
    ${PROJECT_SOURCE_DIR}/local/
    ${PROJECT_BINARY_DIR}/local/
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  OPTIONAL
  PATTERN "*.in" EXCLUDE
)

if(BUILD_TEST)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
  )
  FetchContent_MakeAvailable(googletest)

  include(CTest)
  include(GoogleTest)

  enable_testing()

  set(SRC_TEST_DIR ${PROJECT_SOURCE_DIR}/src/test)
  set(SRC_TEST_CPP_DIR ${SRC_TEST_DIR}/cpp)
  set(SRC_TEST_CPP_PROJECT_DIR ${SRC_TEST_CPP_DIR}/${PROJECT_NAME})

  set(BUILD_SRC_TEST_DIR ${PROJECT_BINARY_DIR}/src/test)
  set(BUILD_SRC_TEST_CPP_DIR ${BUILD_SRC_TEST_DIR}/cpp)
  set(BUILD_SRC_TEST_CPP_PROJECT_DIR ${BUILD_SRC_TEST_CPP_DIR}/${PROJECT_NAME})

  add_subdirectory(${SRC_TEST_CPP_PROJECT_DIR})

  # Build paths to source files.
  set(TEST_SRC_PATHS ${TEST_SRC_NAMES})
  list(TRANSFORM TEST_SRC_PATHS PREPEND ${SRC_TEST_CPP_PROJECT_DIR}/)
  set(BUILD_TEST_SRC_PATHS ${BUILD_TEST_SRC_NAMES})
  list(
    TRANSFORM BUILD_TEST_SRC_PATHS
    PREPEND ${BUILD_SRC_TEST_CPP_PROJECT_DIR}/
  )
  set(TEST_SRCS ${TEST_SRC_PATHS} ${BUILD_TEST_SRC_PATHS})

  add_executable(
    test_instantiator
    ${SRC_TEST_CPP_DIR}/test_instantiator.cc
    ${TEST_SRCS}
  )

  target_include_directories(
    test_instantiator
    PUBLIC
    ${BUILD_SRC_MAIN_CPP_DIR}
    ${BUILD_SRC_TEST_CPP_DIR}
    ${SRC_MAIN_CPP_DIR}
    ${SRC_TEST_CPP_DIR}
  )

  target_link_libraries(
    test_instantiator
    gtest_main
  )

  add_test(
    NAME test_instantiator
    COMMAND test_instantiator
  )

  gtest_discover_tests(test_instantiator)
endif()
