#include "Unit.hpp"

namespace point_test{
    namespace test_case{
        std::vector<Point_int32> prepare_empty_v32(size_t size){
            std::vector<Point_int32> v;
            v.reserve(size);
            // std::cout<<v.capacity()<<"\n";
            return v;
        }
        std::vector<Point_int8> prepare_empty_v8(size_t size){
            std::vector<Point_int8> v;
            v.reserve(size);
            return v;
        }
        std::vector<Point_int32> prepare_random_point_v32(
            size_t size,
            std::uniform_int_distribution<std::mt19937::result_type> dist,
            std::mt19937 mt
        ){
            std::vector<Point_int32> v;
            v.reserve(size);
            for(size_t i = 0; i < size; ++i){
                v.emplace_back(dist(mt), dist(mt));
                // std::cout<<"\n"<<v.back().pair_to_int();
            }
            return v;
        }
        std::vector<Point_int8> prepare_random_point_v8(
            size_t size,
            std::uniform_int_distribution<std::mt19937::result_type> dist,
            std::mt19937 mt
        ){
            std::vector<Point_int8> v;
            v.reserve(size);
            for(size_t i = 0; i < size; ++i){
                v.emplace_back(dist(mt), dist(mt));
            }
            return v;
        }
        timing::timing_and_mask testing_template(){
            //Prepare data from outside
            timing::timing_and_mask result;

            //Start Timer
            timing::timer my_timer;
            my_timer.start();

            //Action loop
            for(int i = 0; i < 10; ++i){
                //Action
            }

            //End Timer
            result.timing = my_timer.end_time();
            
            //Touch data loop
            for(int i = 0; i < 10; ++i){
                //Touch data
                //like result.mask ^= vector[i];
            }

            //Reset data loop
            for(int i = 0; i < 10; ++i){
                //like v[i] = 0;
            }

            //Return result
            return result;
        }

        uint64_t masking_point_32(std::vector<Point_int32>& v){
            uint64_t result = 0;
            for(auto it = v.cbegin(); it != v.cend(); ++it){
                result ^= it->pair_to_int();
            }
            return result;
        }
        uint64_t masking_point_8(std::vector<Point_int8>& v){
            uint64_t result = 0;
            for(auto it = v.cbegin(); it != v.cend(); ++it){
                result ^= it->pair_to_int();
            }
            return result;
        }

        timing::timing_and_mask constructor_adjacent_point_32(std::vector<Point_int32>& v, std::vector<Point_int32>& result){
            timing::timing_and_mask timing_result;
            timing::timer my_timer;

            my_timer.start();

            size_t n = v.size();
            for(size_t i = 0; i < n; ++i){
                result[i*4] = v[i].move_up();
                result[i*4 + 1] = v[i].move_right();
                result[i*4 + 2] = v[i].move_down();
                result[i*4 + 3] = v[i].move_left();
            }
            
            timing_result.timing = my_timer.end_time();

            timing_result.mask = masking_point_32(result);
            
            std::fill(result.begin(), result.end(), point_test::Point_int32());
            return timing_result;

            // auto source = point_test::test_case::prepare_random_point_v8(VECTOR_SIZE, dis, mt);
            // auto dest = point_test::test_case::prepare_empty_v8(VECTOR_SIZE * 4);
            // auto testing = [&](){
            //     return point_test::test_case::constructor_adjacent_point_8(source, dest);
            // };
        }
        timing::timing_and_mask constructor_adjacent_point_8(std::vector<Point_int8>& v, std::vector<Point_int8>& result){
            timing::timing_and_mask timing_result;
            timing::timer my_timer;

            my_timer.start();

            size_t n = v.size();
            for(size_t i = 0; i < n; ++i){
                result[i*4] = v[i].move_up();
                result[i*4 + 1] = v[i].move_right();
                result[i*4 + 2] = v[i].move_down();
                result[i*4 + 3] = v[i].move_left();
            }
            
            timing_result.timing = my_timer.end_time();

            timing_result.mask = masking_point_8(result);
            
            std::fill(result.begin(), result.end(), point_test::Point_int8());
            return timing_result;

            // auto source = point_test::test_case::prepare_random_point_v8(VECTOR_SIZE, dis, mt);
            // auto dest = point_test::test_case::prepare_empty_v8(VECTOR_SIZE * 4);
            // auto testing = [&](){
            //     return point_test::test_case::constructor_adjacent_point_8(source, dest);
            // };
        }
    }
}