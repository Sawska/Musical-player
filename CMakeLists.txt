cmake_minimum_required(VERSION 3.10)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)

find_package(SFML REQUIRED COMPONENTS audio system)
find_package(Catch2 REQUIRED)

include_directories(
    /opt/homebrew/include
    /opt/homebrew/Cellar/libpqxx/7.9.2/include
    /opt/homebrew/Cellar/postgresql@14/14.12/include
    /opt/homebrew/Cellar/nlohmann-json/3.11.3/include
)

link_directories(
    /opt/homebrew/lib
    /opt/homebrew/Cellar/libpqxx/7.9.2/lib/
    /opt/homebrew/Cellar/postgresql@14/14.12/lib/postgresql@14
)

include(${wxWidgets_USE_FILE})

# Main application
add_executable(MyApp
    main.cpp 
    Music.cpp Album.cpp Album.h Music.h PlayerManager.cpp
    PlayerManager.h PlayingQueue.cpp PlayingQueue.h
)

target_link_libraries(MyApp
    ${wxWidgets_LIBRARIES}
    pq
    pqxx
    sfml-audio
    sfml-system
)

# Tests
add_executable(MyAppTests
    test_PlayerManager.cpp  # Your test file
    Music.cpp
    Album.cpp
    PlayerManager.cpp
    PlayingQueue.cpp
)

target_link_libraries(MyAppTests
    Catch2::Catch2
    pq
    pqxx
    sfml-audio
    sfml-system
)

enable_testing()
add_test(NAME MyAppTests COMMAND MyAppTests)
