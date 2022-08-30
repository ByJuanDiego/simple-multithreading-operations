//
// Created by Juan Diego on 8/30/2022.
//

#ifndef PARALLEL_ALGORITHMS_RESULT_T_H
#define PARALLEL_ALGORITHMS_RESULT_T_H

#include <vector>

template<typename Iterator = typename std::vector<int>::iterator>
struct result_t{
    bool not_found;
    Iterator iterator;
    result_t(): not_found(true), iterator(nullptr){}
    ~result_t() = default;
};

#endif //PARALLEL_ALGORITHMS_RESULT_T_H
