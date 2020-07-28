cmake_minimum_required(VERSION 3.16)
project(learn_leetcode C)

set(CMAKE_C_STANDARD 99)
#  力扣网使用 AddressSanitizer 进行内存检查
#  gcc -O -g -fsanitize=address  main.c
#  ./a.out
add_executable(learn_leetcode main.c)