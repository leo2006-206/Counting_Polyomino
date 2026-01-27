#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <vector>
#include <span>
#include <cstdlib>

#include <immintrin.h>
#include "../../Header/Timing.hpp"

std::int64_t sum_v(std::span<std::int64_t> v){
    const std::size_t size = v.size();
    const std::int64_t* ptr = v.data();

    __m256i v_sum[4];
    for(int i = 0; i < 4; ++i){
        v_sum[i] = _mm256_set1_epi64x(0);
    }

    for(std::size_t i = 0; (i + 16) < size; i += 16){
        __m256i v_i[4];
        for(int j = 0; j < 4; ++j){
            v_i[j] = _mm256_lddqu_si256(
                reinterpret_cast<const __m256i*>(&ptr[i + (j * 4)])
            );
        }
        for(int j = 0; j < 4; ++j){
            v_sum[j] = _mm256_add_epi64(v_sum[j], v_i[j]);
        }
    }

    v_sum[0] = _mm256_add_epi64(v_sum[0], v_sum[1]);
    v_sum[2] = _mm256_add_epi64(v_sum[2], v_sum[3]);
    v_sum[0] = _mm256_add_epi64(v_sum[0], v_sum[2]);    

    std::int64_t* int_sum = reinterpret_cast<std::int64_t*>(&v_sum[0]);
    std::int64_t result = 0;

    for(int i = 0; i < 4; ++i){
        result += int_sum[i];
    }

    return result;
}

std::int64_t sum_s(std::span<std::int64_t> v){
    std::int64_t result = 0;

    for(const std::int64_t item : v){
        result += item;
    }

    return result;
}



void test1(){
    const std::size_t n = 1 << 26;
    const std::size_t align = 256;

    using data_t = std::int64_t;

    data_t* int_array = reinterpret_cast<data_t*>(std::aligned_alloc(align, n * sizeof(std::int64_t)));

    for(std::size_t i = 0; i < n; ++i){
        int_array[i] = i;
    }

    std::span<data_t> int_span({int_array, n});

    std::cout << sum_v(int_span);
    //std::cout << "\n" << sum_s(int_span);

    std::free(int_array);
}

void test2(){
    const std::size_t n = 1 << 26;
    const std::size_t align = 256;

    using data_t = std::int64_t;

    data_t* int_array = reinterpret_cast<data_t*>(std::aligned_alloc(align, n * sizeof(std::int64_t)));

    for(std::size_t i = 0; i < n; ++i){
        int_array[i] = i;
    }

    std::span<data_t> int_span({int_array, n});

    //std::cout << sum_v(int_span);
    std::cout << sum_s(int_span);

    std::free(int_array);
}

int main(void){
    timing::timer<std::chrono::milliseconds> my_timer;
    my_timer.start();

    test1();

    my_timer.end_print();

    my_timer.start();

    test2();

    my_timer.end_print();
}