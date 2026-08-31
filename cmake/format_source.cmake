# Copyright (c) 2026 Farzan Hajian.
# Licensed under the BSD 3-Clause License.

find_program(CLANG_FORMAT clang-format)
file(GLOB_RECURSE ALL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
)