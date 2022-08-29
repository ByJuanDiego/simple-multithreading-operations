//
// Created by Juan Diego on 8/28/2022.
//

#ifndef SUMA_VECTOR_PARALELO_PARALLEL_CONTAINER_H
#define SUMA_VECTOR_PARALELO_PARALLEL_CONTAINER_H

// STL libraries
#include <vector>
#include <forward_list>
#include <list>
#include <deque>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <thread>
#include <numeric>
#include <functional>
#include <random>
#include <iostream>
#include <future>

//#define FORWARD_LIST
//#define VECTOR
#define DEQUE

template<typename T = int, template<typename ...> class Container = std::vector, typename Iterator = typename Container<T>::iterator>
class parallel_container {
private:

    typedef typename std::vector<T>::iterator v_Iterator;
    Container<T> data;
    std::size_t size;
    [[nodiscard]] static int get_number_of_threads(std::size_t size, int expected_range);
    static void summarize(Iterator begin, int range, v_Iterator result);

public:
    explicit parallel_container(const std::string& file_name);
    ~parallel_container() = default;

    [[nodiscard]] T parallel_sum_thread(int expected_range = 25);
    [[nodiscard]] T parallel_sum_thread(Iterator first, Iterator last, int expected_range = 25);


    [[nodiscard]] T parallel_sum_async(int expected_range = 25);
    [[nodiscard]] T parallel_sum_async(Iterator first, Iterator last, int expected_range = 25);


    [[nodiscard]] T sequential_sum();
    static void create_file(const std::string& file_name, std::size_t size, int min, int max);

    Iterator begin();
    Iterator end();
};




#endif //SUMA_VECTOR_PARALELO_PARALLEL_CONTAINER_H
