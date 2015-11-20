cmake_minimum_required(VERSION 3.3)
project(MasterThesis)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_BUILD_TYPE debug)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(MasterThesis src/helper_functions.cpp src/helper_functions.h src/main.cpp src/matrix.cpp src/matrix.h src/measure.cpp src/measure.h src/settings.cpp src/settings.h src/analyzer/analyzer.cpp src/analyzer/analyzer.h src/analyzer/gathread.cpp src/analyzer/gathread.h src/analyzer/tsreferenceset.cpp src/analyzer/tsreferenceset.h src/analyzer/tsthread.cpp src/analyzer/tsthread.h src/problem/problem.cpp src/problem/problem.h src/problem/qap.cpp src/problem/qap.h src/problem/salbp.cpp src/problem/salbp.h src/problem/task.cpp src/problem/task.h src/solution/perm_solution.cpp src/solution/perm_solution.h src/solution/randomkeysolution.cpp src/solution/randomkeysolution.h src/solution/solution.cpp src/solution/solution.h src/solution/solutionbase.cpp src/solution/solutionbase.h)
target_link_libraries(MasterThesis Threads::Threads)

install(TARGETS MasterThesis RUNTIME DESTINATION bin)
