target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/vulkan.h
        ${CMAKE_SOURCE_DIR}/src/camera.cpp
        ${CMAKE_SOURCE_DIR}/src/camera.h
        ${CMAKE_SOURCE_DIR}/src/input.h
        ${CMAKE_SOURCE_DIR}/src/mesh.h
        ${CMAKE_SOURCE_DIR}/src/window.h
        ${CMAKE_SOURCE_DIR}/src/window.cpp
)
message("Sources Target")
