set(SOURCE_DIR "Source")

set(SOURCES
    main.cpp
        rect.cpp
        shape.cpp
)

# Add the executable target
add_executable(GPP-SAT-Toy ${SOURCES})
set_target_properties(GPP-SAT-Toy PROPERTIES OUTPUT_NAME prog.x)
target_include_directories(GPP-SAT-Toy PRIVATE ${PROJECT_SOURCE_DIR}/external/raylib/src)
target_include_directories(GPP-SAT-Toy PRIVATE ${PROJECT_SOURCE_DIR}/external/)

target_link_libraries(GPP-SAT-Toy imgui)
target_link_libraries(GPP-SAT-Toy raylib)
target_link_libraries(GPP-SAT-Toy mm)

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()





