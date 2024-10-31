cmake_minimum_required(VERSION 3.19)

set(SOURCES 
    model/custom-search-wrapper.cpp
    main.cpp)

add_executable(main ${SOURCES})

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(main PUBLIC Qt5::Core Qt5::Widgets)