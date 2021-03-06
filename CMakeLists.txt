cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

include(GoogleTest)
include(ProcessorCount)

project(mRMR
		VERSION 1.0
		DESCRIPTION "mRMR (minimum Redundancy Maximum Relevance Feature Selection)"
		LANGUAGES CXX)


## CMAKE Parameters
################################################################################
set(CMAKE_CXX_STANDARD 11)		# Standard
set(CMAKE_BUILD_TYPE Release)	# Default Build
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/build")

## multiproc
################################################################################
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()

## Dependencies List
################################################################################
file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/dependencies/ ${CMAKE_SOURCE_DIR}/dependencies/*)
set(DEPENDENCIES_LIST "")
foreach(child ${children})
	if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/${child})
		list(APPEND DEPENDENCIES_LIST ${child})
	endif()
endforeach()

## Code Coverage Configuration
################################################################################
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	# Add required flags (GCC & LLVM/Clang)
	target_compile_options(coverage_config INTERFACE
		-O0		# no optimization
		-g		 # generate debug info
		--coverage # sets all required flags
	)
	if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
		target_link_options(coverage_config INTERFACE --coverage)
	else()
		target_link_libraries(coverage_config INTERFACE --coverage)
	endif()
endif()

## Google tests Configuration
################################################################################
add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

if($ENV{GOOGLETEST_DIR})
	set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
	set(GOOGLETEST_DIR "${CMAKE_SOURCE_DIR}/dependencies/googletest")
endif ()
if(EXISTS ${GOOGLETEST_DIR})
	set(GTestSrc ${GOOGLETEST_DIR}/googletest)
	set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
else ()
	message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif()


## Sources
################################################################################
set(headers
	"src/CMRMR.hpp"
	"src/test_mRMR.hpp"
)
source_group("headers" FILES ${headers})

set(sources
	${GOOGLETEST_DIR}/googlemock/src/gmock-all.cc
	${GOOGLETEST_DIR}/googletest/src/gtest-all.cc
	"src/CMRMR.cpp"
	"src/main.cpp"
)
source_group("sources" FILES ${sources})

set(ALL_FILES ${headers} ${sources})

## Message Status
################################################################################
message(STATUS "
\tProject Name : ${PROJECT_NAME} ${VERSION}
\tProcessor Count : ${N}
\tCmake Version : ${CMAKE_VERSION}
\tStandard : ${CMAKE_CXX_STANDARD}
\tCommpiler : ${CMAKE_CXX_COMPILER}
\tBuild Type: ${CMAKE_BUILD_TYPE}
\tBuild Directoies : ${CMAKE_INSTALL_PREFIX}
\tDependencies : ${DEPENDENCIES_LIST}
\tCode coverage : ${CODE_COVERAGE}")


## Executable
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

# Additional include directories
set_property(TARGET ${PROJECT_NAME}
	APPEND PROPERTY INCLUDE_DIRECTORIES
		${GOOGLETEST_DIR}/googletest
		${GOOGLETEST_DIR}/googletest/include
		${GOOGLETEST_DIR}/googlemock
		${GOOGLETEST_DIR}/googlemock/include
		${CMAKE_SOURCE_DIR}/src
)

# link
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries(${PROJECT_NAME} PUBLIC coverage_config)

enable_testing()
gtest_discover_tests(${PROJECT_NAME})
