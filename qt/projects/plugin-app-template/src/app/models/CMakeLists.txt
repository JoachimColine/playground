file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(models STATIC ${SRC_FILES})

target_link_libraries(models PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::Qml
    JApp::Logging
)

target_include_directories(models
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(models PROPERTIES
    AUTOMOC ON
)

add_library(JApp::Models ALIAS models)
