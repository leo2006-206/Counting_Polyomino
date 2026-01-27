#pragma once
#ifndef TESTCASE_INT_PAIR_V2_HPP
#define TESTCASE_INT_PAIR_V2_HPP

#include "Poly_Int_Pair_v2.hpp"
#include "Timing.hpp"

#include <random>

namespace test_case{
    using   int_t = int_pair_v2::memo_Polyomino::point_t::point_int_t;
    using   flat_set = boost::container::flat_set
            <int_pair_v2::Point8, int_pair_v2::Point8_cmp>;
    using   unit_t = std::chrono::nanoseconds;

    static constexpr int_t MIN_INT = -128;
    static constexpr int_t MAX_INT = 127;
    static std::random_device rd;
    static std::mt19937 mt(rd());
    static std::uniform_int_distribution<int_t> dist(MIN_INT, MAX_INT);
    
    inline int_t rand_int(){
        return dist(mt);
    }

    flat_set                        prepare_set_random(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_poly_cells(std::size_t set_size);
    int_pair_v2::memo_Polyomino     prepare_poly_empty_cells(std::size_t set_size);

    int_pair_v2::memo_Polyomino     prepare_product_poly_trivial(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_1(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_2(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_3(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_4(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_f2(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_d2_1(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_d2_2(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_c4(std::size_t size);
    int_pair_v2::memo_Polyomino     prepare_product_poly_d4(std::size_t size);

    void                    flip_set_nth(flat_set& set, std::size_t position);

    timing::timing_and_mask testing_template();

    timing::timing_and_mask generate_unique_child1(int_pair_v2::memo_Polyomino& poly);
    timing::timing_and_mask generate_unique_child2(int_pair_v2::memo_Polyomino& poly);

    namespace tested{
        timing::timing_and_mask poly_rot_90_1(flat_set& non_empty_cells, flat_set& empty_cells);
        timing::timing_and_mask poly_rot_90_2(flat_set& non_empty_cells, flat_set& empty_cells);

        timing::timing_and_mask cmp_translation_loop(int_pair_v2::memo_Polyomino& A, int_pair_v2::memo_Polyomino& B);

        timing::timing_and_mask cmp_translation_memcmp(int_pair_v2::memo_Polyomino& A, int_pair_v2::memo_Polyomino& B);

        timing::timing_and_mask cmp_translation_equal(int_pair_v2::memo_Polyomino& A, int_pair_v2::memo_Polyomino& B);
    }
}

#endif