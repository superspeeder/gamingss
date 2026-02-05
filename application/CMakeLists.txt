set(CMAKE_CXX_STANDARD 26)

add_executable(application src/main.cpp)
target_include_directories(application PRIVATE src/)
target_link_libraries(application PRIVATE neuron::neuron)
