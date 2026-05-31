cmake_minimum_required(VERSION 3.16)

project(tcp_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.cpp
    m_server.h
    m_globals.h
    m_processData.h
    m_functions.h)

set(SCRIPT_FILES "")
file(GLOB FRONT "FRONT/*.sh")
list(APPEND SCRIPT_FILES ${FRONT})

file(GLOB BACK "BACK/*.sh")
list(APPEND SCRIPT_FILES ${BACK})

file(GLOB BACK "data/*.json")
list(APPEND SCRIPT_FILES ${BACK})
file(GLOB BACK "data/*.conf")
list(APPEND SCRIPT_FILES ${BACK})

set_source_files_properties(${SCRIPT_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
target_sources(${PROJECT_NAME} PRIVATE ${SCRIPT_FILES})

include(GNUInstallDirs)
install(TARGETS tcp_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

foreach(SRC_PATH ${SCRIPT_FILES})
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SRC_PATH}")
    set(DEST_PATH "${CMAKE_CURRENT_BINARY_DIR}/${REL_PATH}")
    get_filename_component(DEST_DIR "${DEST_PATH}" DIRECTORY)

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DEST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC_PATH}" "${DEST_PATH}"
        COMMENT "Copying script: ${REL_PATH}"
        VERBATIM
    )
endforeach()