# user / system env
# $env:QT_QPA_PLATFORM_PLUGIN_PATH = "D:\Library\Qt\5.15.2\msvc2019_64\plugins\platforms"

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(qt5rootdir "D:/Library/Qt/5.15.2/msvc2019_64")

if (DEFINED qt5rootdir)
    set(Qt5_DIR "${qt5rootdir}/lib/cmake/Qt5")

    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    add_library(Qt5::All INTERFACE IMPORTED GLOBAL)
    target_link_libraries(
        Qt5::All INTERFACE
        Qt5::Core
        Qt5::Widgets
    )

    message(STATUS "[INFO] Qt5 Found -> ${Qt5_DIR}")
else()
    message(FATAL_ERROR "usage: cmake -S . -B build -G Ninja -D qt5rootdir=D:/Library/Qt/5.15.2/msvc2019_64")
endif()
