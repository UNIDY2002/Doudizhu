# Generated with qt-init
# See: https://github.com/UNIDY2002/qt-init

cmake_minimum_required(VERSION 3.15.4)
project(Doudizhu)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER C:/Qt/Tools/mingw810_64/bin/gcc)
set(CMAKE_CXX_COMPILER C:/Qt/Tools/mingw810_64/bin/g++)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH C:/Qt/5.15.0/mingw81_64)

# For new modules, simply add them here.
find_package(Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)

aux_source_directory(. DIRS_SRCS)

include_directories(.)

# Resource files are supposed to be added below.
qt5_add_resources(QRC_FILES)

add_executable(Doudizhu main.cpp ${QRC_FILES} ui/Welcome.cpp ui/Welcome.h network/NetworkPolicy.cpp network/NetworkPolicy.h network/DecentralizedServer.cpp network/DecentralizedServer.h network/DecentralizedClient.cpp network/DecentralizedClient.h network/utils.h network/utils.cpp network/threading.cpp network/threading.h ui/Game.cpp ui/Game.h)

# And do not forget to link new modules here.
target_link_libraries(Doudizhu Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
