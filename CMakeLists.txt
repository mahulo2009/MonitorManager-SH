###################################################################################
# CMake build rules for the Integration Service Monitor Manager SystemHandle library
###################################################################################
cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

project(is-monitormanager VERSION "3.0.0" LANGUAGES CXX)

###################################################################################
# Configure options
###################################################################################

option(BUILD_LIBRARY "Compile the Monitor Manager SystemHandle" ON)

###################################################################################
# Load external CMake Modules.
###################################################################################
if(BUILD_LIBRARY)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SANITIZERS_ROOT}/cmake)

    find_package(Sanitizers QUIET)

    if(SANITIZE_ADDRESS)
        message(STATUS "Preloading AddressSanitizer library could be done using \"${ASan_WRAPPER}\" to run your program.")
    endif()
endif()

###################################################################################
# External dependencies for the Integration Service Monitor Manager SystemHandle library
###################################################################################
if(BUILD_LIBRARY)
    include(ExternalProject)

    find_package(is-core REQUIRED)
    find_package(xtypes REQUIRED)
    if(WIN32)
        find_package(yaml-cpp)
    endif()
endif()

###################################################################################
# Configure the Integration Service Monitor Manager SystemHandle library
###################################################################################
if(BUILD_LIBRARY)
    add_library(${PROJECT_NAME}
        SHARED
            src/Publisher.hpp
            src/SystemHandle.cpp
    )
endif()

###################################################################################
# Find Monitor Manager dependency. If not, load it as external project
###################################################################################
if(BUILD_LIBRARY)

    # Library continuation
    if(Sanitizers_FOUND)
        add_sanitizers(${PROJECT_NAME})
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION
            ${PROJECT_VERSION}
        SOVERSION
            ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        CXX_STANDARD
            17
        CXX_STANDARD_REQUIRED
            YES
        )

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-Werror -Wall -Wextra -Wpedantic>
        )

    include(GNUInstallDirs)
    message(STATUS "Configuring [${PROJECT_NAME}]...")

    target_include_directories(${PROJECT_NAME}
        INTERFACE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        )

    include_directories(/usr/local/thirdparty/cpp-peglib/include/)
    
endif()

###################################################################################
# Install the Integration Service Monitor Manager SystemHandle library
###################################################################################
#if(BUILD_LIBRARY)
#    is_install_middleware_plugin(
#        MIDDLEWARE
#            monitormanager
#        TARGET
#            is-monitormanager
#    )
#endif()
###################################################################################
# Add the Integration Service Monitor Manager SystemHandle testing subdirectory
###################################################################################
if(BUILD_LIBRARY)
    include(CTest)

    if(BUILD_TESTS OR BUILD_MONITORMANAGER_TESTS)
        add_subdirectory(test)
    endif()
endif()

###################################################################################
# Integration Service Monitor Manager SystemHandle API Reference
###################################################################################
if(BUILD_API_REFERENCE)
    find_package(Doxygen REQUIRED)
    # Create doxygen directories
    add_custom_target(doc-dirs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doxygen
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doxygen/html
        COMMENT "Creating documentation directories" VERBATIM)

    set(IS_MONITORMANAGER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")
    file(GLOB_RECURSE HPP_FILES "${IS_MONITORMANAGER_INCLUDE_DIR}/*.h*")

    # Doxygen related variables
    set(DOXYGEN_INPUT_DIR "${IS_MONITORMANAGER_INCLUDE_DIR}/")
    set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/doxygen")
    set(DOXYGEN_INDEX_FILE "${PROJECT_BINARY_DIR}/doxygen/xml/index.xml")
    set(DOXYFILE_IN "${IS_DOXYGEN_CONFIG_FILE}")
    set(DOXYFILE_OUT "${PROJECT_BINARY_DIR}/doxygen-config")
    set(DOXYGEN_TAGFILES "../is-core/is_core.tag=../is-core/doxygen/html")
    set(DOXYGEN_HTML_DIR "${PROJECT_BINARY_DIR}/doxygen/html")

    # Configure doxygen
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # Doxygen command
    add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        DEPENDS ${HPP_FILES}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating doxygen documentation")

    # Generate API reference
    add_custom_target(doxygen ALL
        DEPENDS ${DOXYGEN_INDEX_FILE}
        COMMENT "Generated API documentation with doxygen" VERBATIM)
    add_dependencies(doxygen doc-dirs)

    # Install doxygen generated XML files
    install(DIRECTORY ${PROJECT_BINARY_DIR}/doxygen/xml
        DESTINATION doxygen
        COMPONENT doxygen)
    set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Monitor Manager System Handle doxygen")
    set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION
        "eProsima Monitor Manager System Handle doxygen documentation")
    set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} doxygen)
endif()