cmake_minimum_required(VERSION 3.10)
project(MyApp)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Include the wxWidgets headers
include(${wxWidgets_USE_FILE})

# Add your source files
add_executable(MyApp main.cpp)

# Link against wxWidgets libraries
target_link_libraries(MyApp ${wxWidgets_LIBRARIES})
