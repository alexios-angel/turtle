cmake_minimum_required(VERSION 3.30.0)

project(turtle VERSION 1.0)
set(PROJECT_DESCRIPTION "My Python -> C++ Transcompiler")
set(PROJECT_MAINTAINER_GIT_USERNAME "alexios-angel")
set(PROJECT_MAINTAINER_NAME "Alexios Angel")

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(cotire/CMake/cotire)
include(ucm/cmake/ucm)
include(FetchContent)

#file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/include")
#file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/src")
#file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
#file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/src")
#file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

#vanilla flags
ucm_add_flags(CXX "-std=c++${CMAKE_CXX_STANDARD}")

# CTRE produces too many warnings :(
# Most of these disabled warnings are a good idea tho
#warnings
ucm_add_flags(CXX
        -Wall -Wextra -Wnewline-eof -Wmissing-prototypes
        #-Werror -ferror-limit=5 -fmax-errors=5
        -Wno-c99-designator -Wno-unused-command-line-argument -Wno-enum-constexpr-conversion
        -Wno-deprecated-enum-enum-conversion -Wno-deprecated-anon-enum-enum-conversion
        -Wno-missing-designated-field-initializers -Wno-missing-template-arg-list-after-template-kw
        #-Weverything
        #-Wno-c++20-compat -Wno-c++98-compat -Wno-pre-c++17-compat -Wno-c++98-compat-pedantic -Wno-undef
        #-Wno-padded -Wno-gnu-anonymous-struct -Wno-nested-anon-types
        #-Wno-missing-noreturn -Wno-float-equal -Wno-implicit-int-float-conversion
        #-Wno-sign-conversion -Wno-ctad-maybe-unsupported -Wno-class-varargs
        #-Wno-old-style-cast -Wno-shadow -Wno-reserved-macro-identifier
        )

# Debug flags
ucm_add_flags(CXX -ggdb3 -fno-eliminate-unused-debug-types -fdebug-macro)

#required flags to build project
ucm_add_flags(CXX -fsigned-char -fno-strict-aliasing -fbracket-depth=1024 -march=native)

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

#Generate the static library from the library sources

# find a boost install with the libraries filesystem and system
SET(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.85 REQUIRED
        COMPONENTS
        program_options
        log log_setup
        stacktrace_basic
        stacktrace_backtrace
        #stacktrace_addr2line
        #stacktrace_noop
        )

#Boost Macros to enable libaries
#add_compile_definitions(BOOST_STACKTRACE_LINK)
# boost::stacktrace - requires pthread, backtrace
# boost::log        - requires pthread
add_compile_definitions(BOOST_ENABLE_ASSERT_DEBUG_HANDLER)
add_compile_definitions(BOOST_STACKTRACE_DYN_LINK)
add_compile_definitions(BOOST_LOG_DYN_LINK)
find_package(Threads REQUIRED)
find_package(Backtrace)
# Create a sources variable with a link to all cpp files to compile

# check if boost was found
if (Boost_FOUND)
    message(STATUS "Boost found")
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Cannot find Boost")
endif ()


set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        ctre
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG 129d535e7c24fb42285c74a2cc14efc7b222758a
        GIT_PROGRESS TRUE
        USES_TERMINAL_DOWNLOAD TRUE
)

FetchContent_MakeAvailable(ctre)

include_directories("${ctre_SOURCE_DIR}/single-header")

# Add an executable with the above sources
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/global.cpp
        src/turtle.cpp
        src/langs/antlr/lexicalize.cpp
)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${Threads_LIBRARIES} ${Backtrace_LIBRARIES})
cotire(${PROJECT_NAME})
