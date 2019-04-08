project(PolycraftLauncher_gui LANGUAGES CXX)

set(GUI_SOURCES
    DesktopServices.h
    DesktopServices.cpp

    # Icons
    icons/MMCIcon.h
    icons/MMCIcon.cpp
    icons/IconList.h
    icons/IconList.cpp

    SkinUtils.cpp
    SkinUtils.h
)
################################ COMPILE ################################

add_library(PolycraftLauncher_gui SHARED ${GUI_SOURCES})
set_target_properties(PolycraftLauncher_gui PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN 1)

generate_export_header(PolycraftLauncher_gui)

# Link
target_link_libraries(PolycraftLauncher_gui PolycraftLauncher_iconfix PolycraftLauncher_logic Qt5::Gui)

# Mark and export headers
target_include_directories(PolycraftLauncher_gui PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

# Install it
install(
    TARGETS PolycraftLauncher_gui
    RUNTIME DESTINATION ${LIBRARY_DEST_DIR}
    LIBRARY DESTINATION ${LIBRARY_DEST_DIR}
)
