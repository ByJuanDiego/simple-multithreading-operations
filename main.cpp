// own libraries
#include "parallel_container.cpp"

// macros
#define FILE_MACRO

int main() {
#ifndef FILE_MACRO
    parallel_container<>::create_file("datos3_1000.txt", 1000, 0, 100);
#endif
    auto* parallelContainer = new parallel_container<int, std::deque, std::deque<int>::iterator>("datos1_1000.txt");

    std::cout << parallelContainer->parallel_sum_thread() << std::endl;
    std::cout << parallelContainer->parallel_sum_async() << std::endl;
    std::cout << parallelContainer->sequential_sum() << std::endl;

    std::cout << parallelContainer->parallel_sum_thread(std::next(parallelContainer->begin(), 10), parallelContainer->end()) << std::endl;
    std::cout << parallelContainer->parallel_sum_async(std::next(parallelContainer->begin(), 10), parallelContainer->end()) << std::endl;

    return 0;
}
