
#
# Locate test sources
#
FILE (GLOB_RECURSE test_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Test Files" FILES ${test_SRCS})

#
# Test build rules
#
IF (NOT CMAKE_CROSSCOMPILING)
	FOREACH (T_FILE ${test_SRCS})
		GET_FILENAME_COMPONENT (T_NAME ${T_FILE} NAME_WE)

		add_executable(${T_NAME} ${T_FILE})

		target_compile_features(${T_NAME} PUBLIC cxx_std_14)
		set_target_properties(${T_NAME} PROPERTIES CXX_EXTENSIONS OFF)

		target_include_directories(${T_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}/src")
		target_include_directories(${T_NAME} PRIVATE ${ICU_INCLUDE_DIRS})
		target_link_libraries(${T_NAME} PRIVATE ${ICU_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
		target_compile_options(${T_NAME} PRIVATE "-fno-omit-frame-pointer")
	ENDFOREACH (T_FILE ${test_SRCS})
ENDIF (NOT CMAKE_CROSSCOMPILING)