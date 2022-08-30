// own libraries
#include "parallel_container.cpp"

// macros
#define CREATE_FILE

int main() {

#if defined(CREATE_FILE)
    parallel_container<>::create_file("datos3_100000.txt", 100000, 0, 100);
#endif

    const std::string file_name = "datos3_100000.txt";
    auto* parallelContainer = new parallel_container<int, std::vector>(file_name);

    std::cout << parallelContainer->parallel_sum_thread() << std::endl;
    std::cout << parallelContainer->parallel_sum_async() << std::endl;
    std::cout << parallelContainer->sequential_sum() << std::endl;

    std::cout << parallelContainer->parallel_sum_thread(std::next(parallelContainer->begin(), 10), parallelContainer->end()) << std::endl;
    std::cout << parallelContainer->parallel_sum_async(std::next(parallelContainer->begin(), 10), parallelContainer->end()) << std::endl;

    return 0;
}
