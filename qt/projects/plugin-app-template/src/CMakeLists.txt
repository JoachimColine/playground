add_subdirectory(app)

qt_add_executable(${PROJECT_NAME}
    ${RESOURCES_FILES}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    JApp::Logging
    Qt6::Core
    Qt6::Widgets
)
