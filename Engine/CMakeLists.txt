project (Engine VERSION 0.1)

# Take care of warnings about strcpy
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ################### Dependencies #######################

find_package(Python3 COMPONENTS Interpreter REQUIRED)

if(NOT Python3_Interpreter_FOUND)
    zn_error(FATAL_ERROR "Python 3 is required to generate GLAD2, but it was not found. Please install Python 3.")
endif()

include(FetchContent)

# === GLFW ===

FetchContent_Declare(glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	EXCLUDE_FROM_ALL
)

set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
set(GLFW_INCLUDE_NONE ON CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)
set_target_properties(glfw PROPERTIES FOLDER "ThirdParty")

# === GLAD ===

FetchContent_Declare(glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
  SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)
# I need to manually set the LOCATION as when building glad from source,
# glad_add_library would generate the loader in ${CMAKE_CURRENT_BINARY_DIR}/gladsources/${TARGET},
# so I'm forcing it to generate it in ThirdParty/glad, for better project organization
set(ZN_GLAD_GENERATED_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glad")

glad_add_library(glad_gl_core_45 STATIC REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.5 LOCATION ${ZN_GLAD_GENERATED_SOURCES_DIR})
set_target_properties(glad_gl_core_45 PROPERTIES FOLDER "ThirdParty")

# === GLM ===

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glm)
set_target_properties(glm PROPERTIES FOLDER "ThirdParty")

# === STB ===

FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(stb)

# === SPDLOG ===

FetchContent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.x
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "ThirdParty")

# === ASSIMP ===

FetchContent_Declare(assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v6.0.2
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(assimp)
set_target_properties(assimp PROPERTIES FOLDER "ThirdParty")

#Assimp builds Zlib from source and generates a static library
set_target_properties(zlibstatic PROPERTIES FOLDER "ThirdParty")

# === IMGUI ===

FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(imgui)

if(NOT TARGET imgui)
  file(GLOB IMGUI_SOURCES 
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
  add_library(imgui STATIC ${IMGUI_SOURCES})
  target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
  )
  set_target_properties(imgui PROPERTIES FOLDER "ThirdParty")
endif()

set (LINK_LIBS
    glad_gl_core_45
    glfw ${GLFW_LIBRARIES}
    glm
	spdlog
	imgui
    assimp
	opengl32.lib
)

# ################ Add Engine library and Link ######################

# Setup visual studio source groups / filters
file(GLOB_RECURSE _source_list CONFIGURE_DEPENDS *.cpp *.hpp *.h *.inl)

# If STATIC/SHARED is omitted when defining the library, the default
# will depend on the value of the property BUILD_SHARED_LIBS
add_library (${PROJECT_NAME} ${_source_list})

# Make sure the compiler can find include files for our Engine library
target_include_directories (${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS} ${stb_SOURCE_DIR})

# Link libs
target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:ZN_DEBUG>
  $<$<CONFIG:Release>:ZN_RELEASE>
  $<$<BOOL:${WIN32}>:ZN_WINDOWS_PLATFORM>
  _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
)

# Prettify folders in solution
assign_source_group(${_source_list})