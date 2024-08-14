cmake_minimum_required(VERSION 3.10)
project(MyApp)


link_directories(/opt/homebrew/Cellar/postgresql@14/14.12/lib)


find_package(wxWidgets REQUIRED COMPONENTS core base)


include(${wxWidgets_USE_FILE})


add_executable(MyApp main.cpp)


target_link_libraries(MyApp ${wxWidgets_LIBRARIES} pq pqxx)
