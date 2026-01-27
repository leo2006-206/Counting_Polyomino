// #include "Unit.hpp"
#include "../Header/Testcase_Int_Pair_v2.hpp"

constexpr std::size_t DATA_SIZE = 10000;
constexpr std::size_t REPEAT = 10000;
constexpr std::size_t NTH_DATA = (DATA_SIZE / 100) * 50;

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // std::srand(std::time(NULL));

    auto p = test_case::prepare_product_poly_d2_1(DATA_SIZE);
    auto testing = [&](){
        return test_case::generate_unique_child2(p);
    };

    timing::benchmark_result<test_case::unit_t> result 
        = timing::benchmark<test_case::unit_t>(testing, REPEAT);
    result.result_print(false, DATA_SIZE);
    result.histogram();
    return 0;
}