// own libraries
#include "parallel_container.cpp"

// macros
// #define CREATE_FILE

int main() {

    const std::string file_name = "data.txt";

#if defined(CREATE_FILE)
    parallel_container<>::create_file(file_name, 100000, 0, 100);
#endif

    parallel_container<int, std::vector> Container(file_name);

    std::cout << Container.parallel_sum_thread() << std::endl;
    std::cout << Container.parallel_sum_async() << std::endl;

    const int num = 10;
    std::function<bool(int&)> identifier = [&](int& x)->bool{
        return x == num;
    };

    result_t result = Container.get_by_async(identifier);
    if (!result.not_found){
        std::cout << "valor buscado: " << *result.iterator << std::endl;
        std::cout << "idx: " << std::distance(Container.begin(), result.iterator) << std::endl;
        std::cout << Container.parallel_sum_thread(result.iterator, Container.end()) << std::endl;
        std::cout << Container.parallel_sum_async(result.iterator, Container.end()) << std::endl;
    } else {
        std::cout << "No se encontro un valor con ese identificador" << std::endl;
    }

    return 0;
}
