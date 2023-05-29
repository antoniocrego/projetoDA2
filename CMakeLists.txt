cmake_minimum_required(VERSION 3.24)
project(projetoDA2)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/data_structures)
include_directories(src/menu)

add_executable(projetoDA2
        src/data_structures/Graph.cpp
        src/data_structures/Graph.h
        src/data_structures/Heap.cpp
        src/data_structures/Heap.h
        src/data_structures/MutablePriorityQueue.h
        src/data_structures/UFDS.cpp
        src/data_structures/UFDS.h
        src/data_structures/VertexEdge.cpp
        src/data_structures/VertexEdge.h
        src/menu/Menu.cpp
        src/menu/Menu.h
        src/main.cpp
        src/network.cpp
        src/network.h
        src/Program.cpp
        src/Program.h)
