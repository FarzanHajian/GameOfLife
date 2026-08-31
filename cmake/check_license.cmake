# Copyright (c) 2026 Farzan Hajian.
# Licensed under the BSD 3-Clause License.

file(GLOB_RECURSE ALL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/tests/*.cpp"
    "${PROJECT_SOURCE_DIR}/tests/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/cmake/*.cmake"
)

set(LICENSE_LINE "Licensed under the BSD 3-Clause License")

set(MISSING_FILES "")

foreach(FILE ${ALL_SOURCE_FILES})
    file(READ "${FILE}" CONTENTS)
    string(FIND "${CONTENTS}" "${LICENSE_LINE}" FOUND_POS)
    if(FOUND_POS EQUAL -1)
        list(APPEND MISSING_FILES "${FILE}")
    endif()
endforeach()

if(MISSING_FILES)
    message("Files missing license header:")
    foreach(f ${MISSING_FILES})
        message("  ${f}")
    endforeach()
    message(FATAL_ERROR "License check failed")
endif()