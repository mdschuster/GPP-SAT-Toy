set(MMSRC
        src/vec2.cpp
)
add_library(mm STATIC ${MMSRC})
target_include_directories(mm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(mm PROPERTIES OUTPUT_NAME mm)
set_target_properties(mm PROPERTIES FOLDER "Dependencies")