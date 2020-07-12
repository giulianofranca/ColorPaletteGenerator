# https://docs.opencv.org/4.3.0/d3/d52/tutorial_windows_install.html
# https://docs.opencv.org/4.3.0/d7/d9f/tutorial_linux_install.html
# https://docs.opencv.org/4.3.0/d0/db2/tutorial_macos_install.html
# https://github.com/opencv/opencv/releases
# Used OpenCV 3.4.10

set(SOURCE_FILES
    "main.cpp"
    "generator.cpp" "headers/generator.hpp"
)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
message(STATUS "Found Qt: ${Qt5_VERSION}")
message(STATUS "CV Libs: ${OpenCV_LIBS}")

# Add a executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEFINE_SYMBOL QT_DEPRECATED_WARNINGS)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets ${OpenCV_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})