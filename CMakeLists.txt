cmake_minimum_required(VERSION 3.16)

project(ModernOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure debug symbols are generated for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
endif()

find_package(OpenGL REQUIRED)

set(GLEW_ROOT "C:/Program Files/Microsoft Visual Studio/18/Community/Libraries/glew-2.3.1")

find_path(GLEW_INCLUDE_DIR
    NAMES GL/glew.h
    PATHS "${GLEW_ROOT}/include"
    REQUIRED
)

find_library(GLEW_LIBRARY
    NAMES glew32 glew
    PATHS
        "${GLEW_ROOT}/lib/Release/x64"
        "${GLEW_ROOT}/lib/Release/Win32"
        "${GLEW_ROOT}/lib"
    REQUIRED
)

add_library(GLEW::GLEW UNKNOWN IMPORTED)
set_target_properties(GLEW::GLEW PROPERTIES
    IMPORTED_LOCATION "${GLEW_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
)

include(FetchContent)
fetchcontent_declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm"
    GIT_TAG "1.0.3"
    GIT_SHALLOW TRUE)
fetchcontent_makeavailable(glm)

fetchcontent_declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.5.1"
    GIT_SHALLOW TRUE)
fetchcontent_makeavailable(glfw)

fetchcontent_declare(
    microsoft-gsl
    GIT_REPOSITORY "https://github.com/microsoft/gsl.git"
    GIT_TAG "v5.0.0"
    GIT_SHALLOW TRUE)
fetchcontent_makeavailable(microsoft-gsl)

fetchcontent_declare(
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.17.0"
    GIT_SHALLOW TRUE)
fetchcontent_makeavailable(spdlog)

#file(GLOB_RECURSE VulkanEngineSources CONFIGURE_DEPENDS
	#"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
#)

add_executable(ModernOpenGL main.cpp)

# Copy the GLEW DLL next to the executable so it can be loaded at runtime.
if(WIN32)
    set(GLEW_DLL "${GLEW_ROOT}/bin/Release/x64/glew32.dll")
    if(EXISTS "${GLEW_DLL}")
        add_custom_command(TARGET ModernOpenGL POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${GLEW_DLL}"
                "$<TARGET_FILE_DIR:ModernOpenGL>"
        )
    endif()
endif()


target_link_libraries(ModernOpenGL PRIVATE OpenGL::GL)
target_link_libraries(ModernOpenGL PRIVATE glfw)
target_link_libraries(ModernOpenGL PRIVATE GLEW::GLEW)
target_link_libraries(ModernOpenGL PRIVATE glm)
target_link_libraries(ModernOpenGL PRIVATE Microsoft.GSL::GSL)
target_link_libraries(ModernOpenGL PRIVATE spdlog)
