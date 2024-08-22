cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and configure SFML
find_package(SFML 2.5 COMPONENTS audio system REQUIRED)

# Find Catch2
find_package(Catch2 2.13 REQUIRED)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Manually set pqxx paths
set(PQXX_INCLUDE_DIR "/usr/include/pqxx")  # Adjust this path if necessary
set(PQXX_LIBRARY "/usr/lib/aarch64-linux-gnu/libpqxx.so")  # Path to the shared library or static library

# Check if pqxx paths are valid
if(EXISTS ${PQXX_INCLUDE_DIR} AND EXISTS ${PQXX_LIBRARY})
    set(PQXX_FOUND TRUE)
    message(STATUS "Found pqxx: ${PQXX_LIBRARY}")
else()
    set(PQXX_FOUND FALSE)
    message(FATAL_ERROR "Could not find pqxx library.")
endif()

# Define the main executable
add_executable(MyApp
    MyApp.cpp                
    Album.cpp
    Album.h
    AlbumSelectionDialog.cpp
    AlbumSelectionDialog.h
    AlbumSettingsDialog.cpp
    AlbumSettingsDialog.h
    Music.cpp
    Music.h
    PlayerManager.cpp
    PlayerManager.h
    PlayingQueue.cpp
    PlayingQueue.h
    SearchResultsDialog.cpp
    SearchResultsDialog.h
    SettingsDialog.cpp
    MyFrame.h
    MyFrame.cpp
    MyFrameIDs.h
)

# Link libraries for the main executable
target_link_libraries(MyApp
    sfml-audio
    sfml-system
    ${PQXX_LIBRARY}
    ${wxWidgets_LIBRARIES}
)

# Define the test executable
add_executable(MyAppTests
    test_PlayerManager.cpp 
    Album.cpp
    Album.h
    AlbumSelectionDialog.cpp
    AlbumSelectionDialog.h
    AlbumSettingsDialog.cpp
    AlbumSettingsDialog.h
    Music.cpp
    Music.h
    PlayerManager.cpp
    PlayerManager.h
    PlayingQueue.cpp
    PlayingQueue.h
    SearchResultsDialog.cpp
    SearchResultsDialog.h
    SettingsDialog.cpp
    MyFrame.h
    MyFrame.cpp
    MyFrameIDs.h
)

# Link libraries for the test executable
target_link_libraries(MyAppTests
    Catch2::Catch2
    sfml-audio
    sfml-system
    ${PQXX_LIBRARY}
    ${wxWidgets_LIBRARIES}
)

# Enable testing
enable_testing()
add_test(NAME MyAppTests COMMAND MyAppTests)

# Specify include directories for targets
target_include_directories(MyApp PRIVATE
    ${SFML_INCLUDE_DIR}
    ${CATCH2_INCLUDE_DIR}
    ${wxWidgets_INCLUDE_DIRS}
    ${PQXX_INCLUDE_DIR}
)

target_include_directories(MyAppTests PRIVATE
    ${SFML_INCLUDE_DIR}
    ${CATCH2_INCLUDE_DIR}
    ${wxWidgets_INCLUDE_DIRS}
    ${PQXX_INCLUDE_DIR}
)
