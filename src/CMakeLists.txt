# Copyright (C) 2015 Christopher Gilbert.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 
# Locate project sources
#
FILE (GLOB_RECURSE ts_HDRS RELATIVE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/include/*.h)

#
# Install target
#
FOREACH (H_FILE ${ts_HDRS})
	SET (H_INSTALL_DIR "benchpress")
	STRING (REGEX MATCH "(.*)[/\\]" DIR ${H_FILE})
	INSTALL (FILES ${H_FILE} DESTINATION include/${H_INSTALL_DIR}/${DIR})
ENDFOREACH (H_FILE ${ts_HDRS})

if (BUILD_BENCHPRESS_EXAMPLES)
    add_subdirectory (examples)
endif()

include(CTest)
if (BUILD_BENCHPRESS_TESTS)
    add_subdirectory (test)
endif()

# thx to https://cliutils.gitlab.io/modern-cmake/