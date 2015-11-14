cmake_minimum_required(VERSION 3.3)
project(MasterThesis)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(MasterThesis src/analyzer.cpp src/analyzer.h src/gathread.cpp src/gathread.h src/helper_functions.cpp src/helper_functions.h src/main.cpp src/matrix.cpp src/matrix.h src/problem.cpp src/problem.h src/qap.cpp src/qap.h src/salbp.cpp src/salbp.h src/settings.cpp src/settings.h src/task.cpp src/task.h src/tsreferenceset.cpp src/tsreferenceset.h src/tsthread.cpp src/tsthread.h src/solution/qap_solution.cpp src/solution/qap_solution.h src/solution/randomkeysolution.cpp src/solution/randomkeysolution.h src/solution/salbp_solution.cpp src/solution/salbp_solution.h src/solution/solution.cpp src/solution/solution.h src/solution/solutionbase.cpp src/solution/solutionbase.h)
target_link_libraries(MasterThesis Threads::Threads)

install(TARGETS MasterThesis RUNTIME DESTINATION bin)
