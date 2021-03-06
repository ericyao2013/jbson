project(jbson CXX)
cmake_minimum_required(VERSION 2.8)

option(JBSON_ENABLE_TESTING "Enables testing of jbson" OFF)
option(JBSON_SANITIZE_ADDRESS "Use -fsanitize=address where available" OFF)
option(JBSON_LEAK_CHECKER "Enable memory leak checking where available" OFF)

set(JBSON_DOC_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/doc/" CACHE PATH
"Directory to output generated documentation")

if(${JBSON_SANITIZE_ADDRESS})
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
 if(${JBSON_LEAK_CHECKER})
  SET(ENV{ASAN_OPTIONS} "detect_leaks=1")
 endif(${JBSON_LEAK_CHECKER})
elseif(${JBSON_LEAK_CHECKER})
 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
endif()

find_package(Boost 1.55.0 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

FILE(GLOB_RECURSE jbson_headers ${CMAKE_CURRENT_SOURCE_DIR}/include/jbson/*.hpp)
add_custom_target(jbson_headers_lib ALL SOURCES ${jbson_headers})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdocumentation")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++1y -pthread -Wall -Wextra")

if(${JBSON_ENABLE_TESTING})
enable_testing()

include(ExternalProject)
ExternalProject_Add(
    gtest_${PROJECT_NAME}
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
    SVN_REVISION -r 660
    UPDATE_COMMAND ""
    TIMEOUT 10
    CMAKE_ARGS --no-warn-unused-cli -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
        "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -std=c++11" -DBUILD_SHARED_LIBS=OFF
    # Disable install step
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)
ExternalProject_Get_Property(gtest_${PROJECT_NAME} source_dir)
set(GTEST_INCLUDE_DIRS ${source_dir}/include)
message(STATUS "Gtest include dirs: ${GTEST_INCLUDE_DIRS}")

include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
add_library(${PROJECT_NAME}_test_main OBJECT test/test_main.cpp)

add_dependencies(${PROJECT_NAME}_test_main gtest_${PROJECT_NAME})

ExternalProject_Get_Property(gtest_${PROJECT_NAME} binary_dir)
set(GTEST_LIBRARIES "-L${binary_dir} -lgtest")
message(STATUS "Gtest libs: ${GTEST_LIBRARIES}")

add_definitions(-DJBSON_FILES="${CMAKE_CURRENT_SOURCE_DIR}/test")

function(cxx_test test_name)
  add_executable(${PROJECT_NAME}_${test_name} ${test_name}.cpp $<TARGET_OBJECTS:${PROJECT_NAME}_test_main>)
  target_link_libraries(${PROJECT_NAME}_${test_name} ${GTEST_LIBRARIES})
  add_test(${PROJECT_NAME}_${test_name} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}_${test_name})
endfunction()

add_subdirectory(test)

endif(${JBSON_ENABLE_TESTING})

find_package(Doxygen)

if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

install(DIRECTORY include/jbson DESTINATION include)
