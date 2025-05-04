project(Sandbox VERSION 1.0)

set (INCLUDE_DIRS
	"../Engine"
)

set (LINK_LIBS
	"Engine"
)

# ============== Add Exe and link ================= #

# Setup visual studio source groups / filters ==
file(GLOB_RECURSE _source_list
    *.cpp* *.h* *.hpp* *.h* *.inl)

# Link our normal libraries and create an executeable 
add_executable(${PROJECT_NAME} ${_source_list})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

# Link libs
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS})

# Prettify folders in solution
assign_source_group(${_source_list})