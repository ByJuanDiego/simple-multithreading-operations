//
// Created by Juan Diego on 8/28/2022.
//

#ifndef PARALLEL_ALGORITHMS_PARALLEL_CONTAINER_H
#define PARALLEL_ALGORITHMS_PARALLEL_CONTAINER_H

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

// own libraries
#include "result_t.h"

template<typename T = int, template<typename ...> class Container = std::vector, typename Iterator = typename Container<T>::iterator>
class parallel_container {
    
private:

    typedef typename std::vector<T>::iterator v_Iterator;
    Container<T> data;
    std::size_t size;
    [[nodiscard]] static int get_number_of_threads(std::size_t size, int expected_range);
    auto* get_inserter_function();
    static void summarize(Iterator begin, int range, v_Iterator result);
    static result_t<Iterator> search_by(const std::function<bool(T&)>& function, Iterator first, Iterator last);
public:

    explicit parallel_container(const std::string& file_name);
    ~parallel_container() = default;

    [[nodiscard]] T parallel_sum_thread(int expected_range = 25);
    [[nodiscard]] T parallel_sum_thread(Iterator first, Iterator last, int expected_range = 25);


    [[nodiscard]] T parallel_sum_async(int expected_range = 25);
    [[nodiscard]] T parallel_sum_async(Iterator first, Iterator last, int expected_range = 25);

    static void create_file(const std::string& file_name, std::size_t size, int min, int max);

    Iterator begin();
    Iterator end();

    [[nodiscard]] result_t<Iterator> get_by_async(const std::function<bool(T&)>& function, int expected_range = 25);
};




#endif //PARALLEL_ALGORITHMS_PARALLEL_CONTAINER_H
