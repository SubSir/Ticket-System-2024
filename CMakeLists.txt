cmake_minimum_required(VERSION 3.12)

project(TicketSystem)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# 添加可执行文件
add_executable(TicketSystem ${SOURCES})