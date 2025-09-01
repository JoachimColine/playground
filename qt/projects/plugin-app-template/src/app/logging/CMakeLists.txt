add_library(logging STATIC
    Log.h
    Logger.h
    Logger.cpp
)

target_link_libraries(logging PUBLIC
    Qt6::Core
    Qt6::Widgets
)

# for including "logging/Log.h"
target_include_directories(logging PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(logging PROPERTIES
    AUTOMOC ON
)

# Create a convenient alias for linking
add_library(JApp::Logging ALIAS logging)
