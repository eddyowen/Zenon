## Macro to include all necessary engine header files
## And set some project settings that are the same on everything that uses Fling
MACRO( ZENON_ENGINE_INC EngineDir )

# Take care of warnings about strcpy
    if( MSVC )
        add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    endif()

    include_directories(
		${EngineDir}
        ../external/glfw/include
        ../external/glad/include
        ../external/glm
        ../external/assimp
	)

ENDMACRO(ZENON_ENGINE_INC)