project ( ZenonEngine VERSION 1.0 )

# Take care of warnings about strcpy
if( MSVC )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

set ( ZENON_ENGINE_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" )

# Include the Zenon Engine source files that we may need
ZENON_ENGINE_INC("")

# GLFW
if(NOT TARGET glfw)
	foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS" "GLFW_INSTALL")
		set(${_glfw3_option} OFF CACHE INTERNAL "")
	endforeach()
	set(GLFW_INCLUDE_NONE OFF CACHE INTERNAL "")
	add_subdirectory(../external/glfw glfw)
	set_target_properties(glfw PROPERTIES FOLDER "External")
endif()

# GLAD
if(NOT TARGET glad)
	add_subdirectory(../external/glad glad)
	set_target_properties(glad PROPERTIES FOLDER "External")
endif()

# ASSIMP
#if(NOT TARGET Assimp)
#	foreach(_assimp_option "BUILD_SHARED_LIBS" "ASSIMP_INSTALL")
#		set(${_assimp_option} OFF CACHE INTERNAL "")
#	endforeach()
#	add_subdirectory(../external/assimp assimp)
#	set_target_properties(assimp PROPERTIES FOLDER "External")
#	set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "External")
#	set_target_properties(unit PROPERTIES FOLDER "External")
#	set_target_properties(zlibstatic PROPERTIES FOLDER "External")
#endif()

set ( LINK_LIBS
    glfw ${GLFW_LIBRARIES}
	glad
	opengl32.lib
)

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list
    *.cpp* src/*.h* src/*.hpp* *.h* ${GENERATED_INC_FOLDER}/*.h ${GENERATED_INC_FOLDER}/*.cpp *.inl
)

################# Add library and link ######################

# If STATIC/SHARED is omitted when defining the library, the default
# will depend on the value of the property BUILD_SHARED_LIBS
add_library ( ${PROJECT_NAME} ${_source_list} )

# Make sure the compiler can find include files for our Engine library
target_include_directories ( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Link libs
target_link_libraries( ${PROJECT_NAME} LINK_PUBLIC ${LINK_LIBS} )

# Prettify folders in solution
assign_source_group(${_source_list})