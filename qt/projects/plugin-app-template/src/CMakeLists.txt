add_subdirectory(app)

qt_add_executable(${PROJECT_NAME}
    ${RESOURCES_FILES}
    main.cpp
)
