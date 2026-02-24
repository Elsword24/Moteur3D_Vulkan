file(GLOB_RECURSE SOURCES_FILES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cxx"
)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(SOURCES_FILES ${SOURCES_FILES} PARENT_SCOPE)
endif()
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCES_FILES}
)
message("Sources Target")
