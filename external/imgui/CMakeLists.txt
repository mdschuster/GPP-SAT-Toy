#imgui library
set(IMGUISRC
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    rlImGui.cpp
    imgui_stdLib.cpp
    imgui.cpp)
add_library(imgui STATIC ${IMGUISRC})
set_target_properties(imgui PROPERTIES OUTPUT_NAME imgui)
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")