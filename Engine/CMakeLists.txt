project (Engine VERSION 0.1)

# Take care of warnings about strcpy
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# ################### Dependencies #######################

include(FetchContent)

# === GLFW ===

set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
set(GLFW_INCLUDE_NONE OFF CACHE INTERNAL "")

FetchContent_Declare(glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	EXCLUDE_FROM_ALL
	)
FetchContent_MakeAvailable(glfw)
set_target_properties(glfw PROPERTIES FOLDER "ThirdParty")

# === GLAD ===

FetchContent_Declare(glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glad)
set_target_properties(glad PROPERTIES FOLDER "ThirdParty")

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
    glfw ${GLFW_LIBRARIES}
	glad
	glm
	spdlog
	imgui
	opengl32.lib
)

# ################ Add Engine library and link ######################

# Setup visual studio source groups / filters
file(GLOB_RECURSE _source_list
    *.cpp* src/*.h* src/*.hpp* *.h *.inl
)

# If STATIC/SHARED is omitted when defining the library, the default
# will depend on the value of the property BUILD_SHARED_LIBS
add_library (${PROJECT_NAME} ${_source_list})

# Make sure the compiler can find include files for our Engine library
target_include_directories (${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS} ${stb_SOURCE_DIR})

# Link libs
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:ZN_DEBUG>
  $<$<CONFIG:Release>:ZN_RELEASE>
  $<$<BOOL:${WIN32}>:ZN_WINDOWS_PLATFORM>
  _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
)

# Prettify folders in solution
assign_source_group(${_source_list})