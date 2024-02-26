project( Sanbox VERSION 1.0 )

#======== Engine Setup =========#

##include_directories(
##	Gameplay/inc
##)

ZENON_ENGINE_INC("../ZenonEngine/")

set ( LINK_LIBS
	"ZenonEngine"
)

# == Setup visual studio source groups / filters == #

file( GLOB_RECURSE _source_list
    *.cpp* src/*.h* src/*.hpp* *.h* *.inl )

# ============== Add Exe and link ================= #

# Link our normal libraries and create an executeable 
add_executable( ${PROJECT_NAME} ${_source_list} )

# Include directories
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS} )

# Link libs
target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS} )

# Prettify folders in solution
assign_source_group(${_source_list})
