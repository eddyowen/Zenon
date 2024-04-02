project ( "glad" VERSION 1.0 )

# Take care of warnings about strcpy
if( MSVC )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list
    *.c* src/*.h* *.h* 
)

################# Add library and link ######################

# If STATIC/SHARED is omitted when defining the library, the default
# will depend on the value of the property BUILD_SHARED_LIBS
add_library ( ${PROJECT_NAME} ${_source_list} )

target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )