cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(GeoProcessing_Project)

set(BOOST_INCLUDE_DIRS D:/dev/boost/include)
set(BOOST_LIBRARY_DIRS D:/dev/boost/lib)

set(GDAL_INCLUDE_DIRS D:/dev/gdal/include)
set(GDAL_LIBRARY_DIRS D:/dev/gdal/lib)
set(GDAL_LIBRARIES gdal_i.lib)

include_directories(${GDAL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
link_directories(${GDAL_LIBRARY_DIRS} ${BOOST_LIBRARY_DIRS})

aux_source_directory(. DIR_SRCS)

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp)
source_group("Header Files" FILES ${CURRENT_HEADERS})

add_executable(GeoProcessing ${DIR_SRCS} ${CURRENT_HEADERS})
target_link_libraries (GeoProcessing ${GDAL_LIBRARIES})
