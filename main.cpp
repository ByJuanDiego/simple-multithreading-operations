// own libraries
#include "parallel_container.cpp"

// macros
#define FILE_MACRO

int main() {
#ifndef FILE_MACRO
    parallel_container<>::create_file("datos3_1000.txt", 1000, 0, 100);
#endif
    auto* parallelContainer = new parallel_container<int, std::vector, std::vector<int>::iterator>("datos1_1000.txt");
    std::cout << parallelContainer->parallel_sum_thread() << std::endl;
    std::cout << parallelContainer->parallel_sum_async() << std::endl;

    std::cout << parallelContainer->parallel_sum_thread(parallelContainer->begin()+100, parallelContainer->end()) << std::endl;
    std::cout << parallelContainer->parallel_sum_async(parallelContainer->begin()+100, parallelContainer->end()) << std::endl;

    std::cout << parallelContainer->sequential_sum() << std::endl;
    return 0;
}
