cmake_minimum_required(VERSION 3.29)
project(Vulkan)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Add option to enable/disable C++ 20 module
option(ENABLE_CPP20_MODULE "Enable C++ 20 module support for Vulkan" OFF)

# Enable C++ module dependency scanning only if C++ 20 module is enabled
if(ENABLE_CPP20_MODULE)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()


find_package (Vulkan 1.4.335 REQUIRED)      # Require Vulkan SDK version 1.4.335 or higher
message("Vulkan found : ${Vulkan_FOUND}")
message("Vulkan include : ${Vulkan_INCLUDE_DIR}")


if(UNIX AND NOT APPLE)
    find_package (glfw3 3.4.0 EXACT REQUIRED CONFIG)
#    find_package (glm 1.0.3 EXACT REQUIRED CONFIG)
#
#    find_package (tinyobjloader  REQUIRED CONFIG)
#    find_package (TinyGLTF  REQUIRED CONFIG)
#    find_package(Stb 0.02 EXACT REQUIRED)
#    find_package(Ktx 4.4.2 EXACT REQUIRED CONFIG)

    message("glfw3 found : ${glfw3_FOUND}")
#    message("glm found : ${glm_FOUND}")
#    message("tinyobjloader found : ${tinyobjloader_FOUND}")
#    message("TinyGLTF found : ${TinyGLTF_FOUND}")
#    message("Stb found : ${stb_FOUND}")
#    message("KTX include : ${Ktx_FOUND}")
endif ()



# set up Vulkan C++ module only if enabled
if(ENABLE_CPP20_MODULE)
    add_library(VulkanCppModule)
    add_library(Vulkan::cppm ALIAS VulkanCppModule)

    target_compile_definitions(VulkanCppModule
            PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
    )
    target_include_directories(VulkanCppModule
            PUBLIC
            "${Vulkan_INCLUDE_DIR}"
    )
    target_link_libraries(VulkanCppModule
            PUBLIC
            Vulkan::Vulkan
    )

    set_target_properties(VulkanCppModule PROPERTIES CXX_STANDARD 20)

    # Add MSVC-specific compiler options for proper C++ module support
    if(MSVC)
        target_compile_options(VulkanCppModule PRIVATE
                /std:c++latest      # Use latest C++ standard for better module support
                /permissive-        # Standards conformance mode
                /Zc:__cplusplus     # Enable correct __cplusplus macro
                /EHsc               # Enable C++ exception handling
                /Zc:preprocessor    # Use conforming preprocessor
                /translateInclude   # Automatically translate #include to import for standard library
        )
    endif()

    target_sources(VulkanCppModule
            PUBLIC
            FILE_SET cxx_modules TYPE CXX_MODULES
            BASE_DIRS
            "${Vulkan_INCLUDE_DIR}"
            FILES
            "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
    )


    # Add the vulkan.cppm file directly as a source file
    target_sources(VulkanCppModule
            PRIVATE
            "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
    )
else()
    # Create a dummy interface library when C++ 20 module is disabled
    add_library(VulkanCppModule INTERFACE)
    add_library(Vulkan::cppm ALIAS VulkanCppModule)
    target_link_libraries(VulkanCppModule INTERFACE Vulkan::Vulkan)
    target_compile_definitions(VulkanCppModule
            INTERFACE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
    )
endif()

add_executable (glslang::validator IMPORTED)
add_executable (${PROJECT_NAME})
find_program (GLSLANG_VALIDATOR "glslangValidator" HINTS $ENV{VULKAN_SDK}/bin REQUIRED)
set_property (TARGET glslang::validator PROPERTY IMPORTED_LOCATION "${GLSLANG_VALIDATOR}")
find_program(SLANGC_EXECUTABLE slangc HINTS $ENV{VULKAN_SDK}/bin REQUIRED)

function (add_slang_shader_target TARGET)
    cmake_parse_arguments("SHADER" "" "SOURCES" "" ${ARGN})
    set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
    set (ENTRY_POINTS -entry vertMain -entry fragMain)
    add_custom_command(
            OUTPUT ${SHADERS_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
            COMMENT "Create Shaders Directory"
    )
    add_custom_command(
            OUTPUT ${SHADERS_DIR}/slang.spv
            COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4+spvRayQueryKHR -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
            WORKING_DIRECTORY ${SHADERS_DIR}
            DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
            COMMENT "Compiling Slang Shaders"
            VERBATIM
    )
    add_custom_target(${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
#    cmake_parse_arguments ("SHADER" "" "CHAPTER_NAME" "SOURCES" ${ARGN})
#    set (SHADERS_DIR ${SHADER_CHAPTER_NAME}/shaders)
#    file(GLOB HAS_COMPUTE ${CHAPTER_SHADER}.comp)
#    set (ENTRY_POINTS -entry vertMain -entry fragMain)
#    if(HAS_COMPUTE)
#        list(APPEND ENTRY_POINTS -entry compMain)
#    endif()
#    add_custom_command (
#            OUTPUT ${SHADERS_DIR}
#            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
#    )
#    add_custom_command (
#            OUTPUT  ${SHADERS_DIR}/slang.spv
#            COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4+spvRayQueryKHR -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
#            WORKING_DIRECTORY ${SHADERS_DIR}
#            DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
#            COMMENT "Compiling Slang Shaders"
#            VERBATIM
#    )
#    add_custom_target (${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
endfunction()
set (SHADER_SLANG_TARGET ${PROJECT_NAME}_slang_shader)
set(SHADER_SLANG_SOURCES ${CMAKE_SOURCE_DIR}/shaders/shader.slang)
add_slang_shader_target(${SHADER_SLANG_TARGET} SOURCES ${SHADER_SLANG_SOURCES})
add_dependencies(${PROJECT_NAME} ${SHADER_SLANG_TARGET})

if(WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/external/include
    )

    target_link_directories(${PROJECT_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/external/lib
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
            glfw3-s
            SPIRV-Reflect
    )
    message("External Libraries Target")
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            glfw
    )
    message("Libraries vcpkg Target")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::cppm
)
message("Vulkan Target")

add_subdirectory(src)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/include/
)

message("Build Setup")

