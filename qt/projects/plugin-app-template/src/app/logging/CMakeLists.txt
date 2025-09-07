file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(logging STATIC ${SRC_FILES})

target_link_libraries(logging PUBLIC
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(logging PROPERTIES
    AUTOMOC ON
)

add_library(JApp::Logging ALIAS logging)
