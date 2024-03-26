function(add_git_submodule submodule_path)
# add a Git submodule directory to CMake, assuming the
# Git submodule directory is a CMake project.
#
# Usage: in CMakeLists.txt
# 
# include(AddGitSubmodule.cmake)
# add_git_submodule(mysubmod_dir)

  find_package(Git REQUIRED)

  # Check if the submodule directory exists
  if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${submodule_path}/.git")
    zn_log("Initializing and updating submodule at ${submodule_path}")
    # Initialize and update the submodule
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${submodule_path}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT
                    OUTPUT_QUIET)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        zn_error(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
  endif()

endfunction()