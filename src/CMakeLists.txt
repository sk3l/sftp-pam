project(sftp-pam)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CC_FLAGS 
${CMAKE_CC_FLAGS} 
"-Wall -pedantic")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#find_package (Threads)
find_package (PAM REQUIRED)

set(SOURCES)
set(INCLUDES)

include_directories(${CMAKE_SOURCE_DIR})
link_directories(/usr/lib/x86_64-linux-gnu)

#set(
#INCLUDES
#${INCLUDES}
#${CMAKE_CURRENT_SOURCE_DIR}/hash_map_o.h)

set(
   SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/sftp-pam-auth.c)

#add_subdirectory(./app)
message("SOURCES => ${SOURCES}")
message("INCLUDES => ${INCLUDES}")

set(LIBNAME ${PROJECT_NAME})

set(APPNAME ${PROJECT_NAME}-test)

#set(LIBNAME_S "${PROJECT_NAME}_s")

#target_link_libraries (${LIBNAME_S} ${CMAKE_THREAD_LIBS_INIT})
#add_library(${LIBNAME_S} STATIC ${SOURCES})

add_library(${LIBNAME} SHARED ${SOURCES} ${INCLUDES})
add_executable(${APPNAME} test.c)
target_link_libraries(${APPNAME}  ${PAM_LIBRARIES})

### install ###
#install(TARGETS ${LIBNAME_S} ${LIBNAME} DESTINATION dist)
#install(DIRECTORY "${CMAKE_SOURCE_DIR}/" DESTINATION dist/includes FILES_MATCHING PATTERN "*.h")
