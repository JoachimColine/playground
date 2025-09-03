add_library(logging STATIC
    include/JApp/Log.h
    include/JApp/Logger.h
    src/Logger.cpp
)

target_link_libraries(logging PUBLIC
    Qt6::Core
    Qt6::Widgets
)

# for including "logging/Log.h"
target_include_directories(logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(logging PROPERTIES
    AUTOMOC ON
)

# Create a convenient alias for linking
add_library(JApp::Logging ALIAS logging)
