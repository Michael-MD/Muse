cmake_minimum_required(VERSION 3.10)

project(Muse C)

# Ensure 64-bit architecture
set(CMAKE_GENERATOR_PLATFORM x64)

# Create the executable
file(GLOB_RECURSE SOURCE_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

set(FFTW3_DIR ${CMAKE_SOURCE_DIR}/ext/fftw-3.3.10)
set(LIBSNDFILE_DIR ${CMAKE_SOURCE_DIR}/ext/libsndfile)
set(PORTAUDIO_DIR ${CMAKE_SOURCE_DIR}/ext/portaudio)

# additional subdirectories
add_subdirectory(${FFTW3_DIR})
add_subdirectory(${LIBSNDFILE_DIR})
add_subdirectory(${PORTAUDIO_DIR})


# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${FFTW3_DIR}/api
    ${LIBSNDFILE_DIR}/include
    ${PORTAUDIO_DIR}/include
)

# Link libraries to executable
target_link_directories(Muse PRIVATE 
	${CMAKE_SOURCE_DIR}/build/ext/fftw-3.3.10/Release
	${CMAKE_SOURCE_DIR}/build/ext/libsndfile/Release
	${CMAKE_SOURCE_DIR}/build/ext/portaudio/Release
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	fftw3f_omp
	sndfile
	portaudio_static_x64
)

# Conditionally include dirent header files for Windows,
# dirent comes with standard C on unix-based systems
IF(WIN32)
	set(DIRECT_DIR ${CMAKE_SOURCE_DIR}/ext/dirent)
	add_subdirectory(${DIRECT_DIR})
	target_include_directories(${PROJECT_NAME} PUBLIC ${DIRECT_DIR}/include)
ENDIF()