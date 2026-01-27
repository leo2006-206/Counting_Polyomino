#include "Testcase_Int_Pair_v2.hpp"
#include "Poly_Int_Pair_v2.hpp"
#include "Timing.hpp"

namespace test_case{
	flat_set prepare_set_random(std::size_t size){
		flat_set result;
		result.reserve(size);
		for(std::size_t i = 0; i < size; ++i){
			result.emplace(rand_int(), rand_int());
		}
		return result;
	}

	int_pair_v2::memo_Polyomino     prepare_poly_cells(std::size_t set_size){
		int_pair_v2::memo_Polyomino result;
		result._non_empty_cells = std::move(prepare_set_random(set_size));
		return result;
	}
	int_pair_v2::memo_Polyomino     prepare_poly_empty_cells(std::size_t set_size){
		int_pair_v2::memo_Polyomino result;
		result._empty_cells = std::move(prepare_set_random(set_size));
		return result;
	}

	int_pair_v2::memo_Polyomino     prepare_product_poly_trivial(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 2){
			std::size_t max = size-2;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(i, 0);
			}
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();

		return result;
	}
	int_pair_v2::memo_Polyomino     prepare_product_poly_c2_1(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 5){
			std::size_t max = size - 3;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(0, i);
			}
			result._non_empty_cells.emplace(1, 0);
			result._non_empty_cells.emplace(1, max);
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();

		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_2(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 3){
			std::size_t max = size / 2;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(i, 0);
				result._non_empty_cells.emplace(0, i);
			}
			result._non_empty_cells.emplace(0, 0);
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();

		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_3(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 5){
			std::size_t max = size - 3;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(i, 0);
			}
			result._non_empty_cells.emplace(0, 1);
			result._non_empty_cells.emplace(1, max);
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();

		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_c2_4(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 3){
			std::size_t max = size / 2;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(-i, 0);
				result._non_empty_cells.emplace(0, i);
			}
			result._non_empty_cells.emplace(0, 0);
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();

		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_f2(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 4){
			std::size_t max = size / 2;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(0, i);
				result._non_empty_cells.emplace(1, -i);
			}
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();
		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_d2_1(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 2){
			std::size_t max = size;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(0, i);
			}
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();
		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_d2_2(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 2){
			std::size_t max = size;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(i, 0);
			}
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();
		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_c4(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 8){
			std::size_t max = size / 4;
			for(std::size_t i = 0; i < max; ++i){
				result._non_empty_cells.emplace(0, i + 1);
				result._non_empty_cells.emplace(i + 1, 1);
				result._non_empty_cells.emplace(1, -i);
				result._non_empty_cells.emplace(-i, 0);
			}
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();
		return result;
	}
    int_pair_v2::memo_Polyomino     prepare_product_poly_d4(std::size_t size){
		int_pair_v2::memo_Polyomino result;

		if(size >= 5){
			std::size_t max = ((size - 1) / 4) + 1;
			for(std::size_t i = 1; i < max; ++i){
				result._non_empty_cells.emplace(0, i);
				result._non_empty_cells.emplace(i, 0);
				result._non_empty_cells.emplace(0, -i);
				result._non_empty_cells.emplace(-i, 0);
			}
			result._non_empty_cells.emplace(0, 0);
		}
		else{
			result._non_empty_cells.emplace(0, 0);
		}

		result.generate_adj_empty_cells();
		result.shift_to_origin();
		return result;
	}

	void flip_set_nth(flat_set& set, std::size_t position){
		*set.nth(position) = set.nth(position)->get_reflect_line_45();
	}

	timing::timing_and_mask testing_template(){
		//Prepare data from outside
		timing::timing_and_mask result;

		//Start Timer
		timing::timer<unit_t> my_timer;
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

	timing::timing_and_mask generate_unique_child1(int_pair_v2::memo_Polyomino& poly){
		timing::timing_and_mask result;
		int_pair_v2::symmetry_Poly mask;

		timing::timer<unit_t> my_timer;
		my_timer.start();

		auto flat_set = mask.generate_unique_child(poly);

		result.timing = my_timer.end_time();

		for(const auto p : flat_set){
			result.mask ^= p._x;
		}

		return result;
	}
    timing::timing_and_mask generate_unique_child2(int_pair_v2::memo_Polyomino& poly){
		timing::timing_and_mask result;
		int_pair_v2::symmetry_Poly mask;

		timing::timer<unit_t> my_timer;
		my_timer.start();

		auto flat_set = mask.generate_unique_child2(poly);

		result.timing = my_timer.end_time();

		for(const auto p : flat_set){
			result.mask ^= p._x;
		}

		return result;
	}

	namespace tested{
		timing::timing_and_mask poly_rot_90_1(flat_set& non_empty_cells, flat_set& empty_cells){
			timing::timing_and_mask result;
			int_pair_v2::memo_Polyomino poly;

			//copying data
			poly._non_empty_cells = non_empty_cells;
			poly._empty_cells = empty_cells;

			timing::timer<unit_t> my_timer;
			my_timer.start();

			int_pair_v2::memo_Polyomino product(poly.get_rotate_clockwise_90());

			result.timing = my_timer.end_time();

			for(auto p : product._non_empty_cells){
				result.mask ^= p._x;
			}
			for(auto p : product._empty_cells){
				result.mask ^= p._y;
			}

			return result;
		}

		timing::timing_and_mask cmp_translation_loop(int_pair_v2::memo_Polyomino& A, int_pair_v2::memo_Polyomino& B){
			timing::timing_and_mask result;

			timing::timer<unit_t> my_timer;
			my_timer.start();

			int_pair_v2::compare_translation(A, B);

			result.timing = my_timer.end_time();

			for(auto p : A._non_empty_cells){
				result.mask ^= p._x;
			}
			for(auto p : B._non_empty_cells){
				result.mask ^= p._y;
			}

			return result;
		}
	}
}