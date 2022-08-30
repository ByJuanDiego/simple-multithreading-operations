//
// Created by Juan Diego on 8/28/2022.
//

#include "parallel_container.h"

template<typename T, template<typename ...> class Container, typename Iterator>
int parallel_container<T, Container, Iterator>::get_number_of_threads(std::size_t size, int range) {
    int max_threads = static_cast<int>(size + range -1)/range;
    int k_threads = static_cast<int>(std::thread::hardware_concurrency());
    return std::min(k_threads!=0? k_threads : 2, max_threads);
}

template<typename T, template<typename ...> class Container, typename Iterator>
void parallel_container<T, Container, Iterator>::summarize(Iterator begin, int range, v_Iterator result){
    *result = std::accumulate(begin, std::next(begin, range), 0);
}

template<typename T, template<typename ...> class Container, typename Iterator>
auto* parallel_container<T, Container, Iterator>::get_inserter_function(){
    if constexpr (std::is_same_v<Container<T>, std::vector<T>> || std::is_same_v<Container<T>, std::deque<T>>){
        return &std::back_inserter<Container<T>>;
    } else{
        return &std::front_inserter<Container<T>>;
    }
}

template<typename T, template<typename ...> class Container, typename Iterator>
parallel_container<T, Container, Iterator>::parallel_container(const std::string &file_name) {
    auto* inserter_function = this->get_inserter_function();
    std::ifstream file(file_name, std::ios::in);
    std::copy(
        std::istream_iterator<T>(file),
        std::istream_iterator<T>(),
        inserter_function(data)
    );
    size = std::distance(std::begin(data), std::end(data));
}

template<typename T, template<typename ...> class Container, typename Iterator>
T parallel_container<T, Container, Iterator>::parallel_sum_thread(int expected_range) {
    int number_of_threads = this->get_number_of_threads(size, expected_range);
    size_t range =  ceil((size * 1.0)/ number_of_threads);
    std::vector<std::thread> threads(number_of_threads);
    std::vector<T> subtotal(number_of_threads);

    Iterator data_iter = std::begin(data);
    v_Iterator subtotal_iter = std::begin(subtotal);

    for (std::thread& thread: threads){
        if (std::distance(data_iter, std::end(data)) < range){
            range = std::distance(data_iter, std::end(data));
        }

        thread = std::thread(summarize, data_iter, range, subtotal_iter);
        std::advance(data_iter, range);
        subtotal_iter++;
    }

    for (std::thread& thread:threads){
        thread.join();
    }

    return std::accumulate(std::begin(subtotal), std::end(subtotal), 0);
}

template<typename T, template<typename ...> class Container, typename Iterator>
T parallel_container<T, Container, Iterator>::parallel_sum_thread(Iterator first, Iterator last, int expected_range) {
    std::size_t sz = std::distance(first, last);
    int number_of_threads = this->get_number_of_threads(sz, expected_range);
    size_t range = ceil((sz * 1.0)/ number_of_threads);
    std::vector<std::thread> threads(number_of_threads);
    std::vector<T> subtotal(number_of_threads);

    Iterator data_iter = first;
    v_Iterator subtotal_iter = std::begin(subtotal);

    for (std::thread& thread: threads){
        if (std::distance(data_iter, last) < range){
            range = std::distance(data_iter, last);
        }

        thread = std::thread(summarize, data_iter, range, subtotal_iter);
        std::advance(data_iter, range);
        subtotal_iter++;
    }

    for (std::thread& thread:threads){
        thread.join();
    }

    return std::accumulate(std::begin(subtotal), std::end(subtotal), 0);
}

template<typename T, template<typename ...> class Container, typename Iterator>
T parallel_container<T, Container, Iterator>::parallel_sum_async(int expected_range) {
    int number_of_futures = get_number_of_threads(size, expected_range);
    int range = std::ceil(size * 1.0 / number_of_futures);
    std::vector<std::future<T>> asyncs(number_of_futures);
    Iterator data_iter = std::begin(data);

    for (std::future<T>& f: asyncs){
        if (std::distance(data_iter, std::end(data)) < range){
            range = std::distance(data_iter, std::end(data));
        }
        f = std::async(std::accumulate<Iterator, T>, data_iter, std::next(data_iter, range), 0);
        std::advance(data_iter, range);
    }

    T total = T{};
    for (std::future<T>& f: asyncs){
        total += f.get();
    }
    return total;
}

template<typename T, template<typename ...> class Container, typename Iterator>
T parallel_container<T, Container, Iterator>::parallel_sum_async(Iterator first, Iterator last, int expected_range) {
    std::size_t sz = std::distance(first, last);
    int number_of_futures = get_number_of_threads(sz, expected_range);
    int range = std::ceil(size * 1.0 / number_of_futures);
    std::vector<std::future<T>> asyncs(number_of_futures);
    Iterator data_iter = first;

    for (std::future<T>& f: asyncs){
        if (std::distance(data_iter, last) < range){
            range = std::distance(data_iter, std::end(data));
        }
        f = std::async(std::accumulate<Iterator, T>, data_iter, std::next(data_iter, range), 0);
        std::advance(data_iter, range);
    }

    T total = T{};
    for (std::future<T>& f: asyncs){
        total += f.get();
    }
    return total;
}

template<typename T, template<typename ...> class Container, typename Iterator>
T parallel_container<T, Container, Iterator>::sequential_sum() {
    return std::accumulate(std::begin(data), std::end(data), 0);
}

template<typename T, template<typename ...> class Container, typename Iterator>
void parallel_container<T, Container, Iterator>::create_file(const std::string& file_name, std::size_t size, int min, int max){
    std::ofstream file(file_name, std::ios::out);

    if (file.fail()){
        std::cout << "Error al abrir el archivo" << std::endl;
    }

    std::random_device rd;
    std::uniform_int_distribution<int> dis(min, max);
    for (size_t i = 0; i < size; ++i){
        file << dis(rd) << std::endl;
    }
}

template<typename T, template<typename ...> class Container, typename Iterator>
Iterator parallel_container<T, Container, Iterator>::begin(){
    return data.begin();
}

template<typename T, template<typename ...> class Container, typename Iterator>
Iterator parallel_container<T, Container, Iterator>::end(){
    return data.end();
}

template<typename T, template<typename ...> class Container, typename Iterator>
result_t<Iterator> parallel_container<T, Container, Iterator>::get_by_async(const std::function<bool(T&)>& function, int expected_range) {
    int number_of_futures = this->get_number_of_threads(size, expected_range);
    int range =  ceil((size * 1.0)/ number_of_futures);
    std::vector<std::future<result_t<Iterator>>> asyncs(number_of_futures);
    Iterator data_iter = std::begin(data);

    for (std::future<result_t<Iterator>>& f: asyncs){
        if (std::distance(data_iter, std::end(data)) < range){
            range = std::distance(data_iter, std::end(data));
        }

        f = std::async(search_by, function, data_iter, std::next(data_iter, range));
        std::advance(data_iter, range);
    }

    result_t<Iterator> result;
    for (std::future<result_t<Iterator>>& f: asyncs){
        result = f.get();
        if (!result.not_found){
            return result;
        }
    }
    result.iterator = std::end(data);
    result.not_found = true;
    return result;
}

template<typename T, template<typename ...> class Container, typename Iterator>
result_t<Iterator> parallel_container<T, Container, Iterator>::search_by(const std::function<bool(T&)>& function, Iterator first, Iterator last) {
    result_t<Iterator> result;
    for (Iterator it = first; it != last; it++){
        if (function(*it)){
            result.not_found = false;
            result.iterator = it;
            break;
        }
    }
    return result;
}
