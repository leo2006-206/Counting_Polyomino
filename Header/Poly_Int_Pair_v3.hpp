#pragma once
#ifndef POLY_INT_PAIR_V3_HPP
#define POLY_INT_PAIR_V3_HPP

// #include <immintrin.h>

// File IO header
#include <iostream>
#include <fstream>

// Typing header
#include <cstdint>
#include <cmath>
#include <string>
// for hypot and memcmp

#include <algorithm>
#include <bit>

// Container header
#include <array>
#include <vector>
#include <span>

#include <boost/container/flat_set.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_node_map.hpp>

#include "./../../Hash_Function/Hash_Function.hpp"

namespace int_pair_v3{
	struct Point8{
		using self_t    = Point8;
		using _int_t	= std::int8_t;
		using hash_t	= std::uint16_t;
		using _uint_t   = std::uint8_t;

		static constexpr std::size_t HASH_SHIFT = 8;
		static constexpr std::size_t HASH_MASK  = 0xFF;

		_int_t	_x;
		_int_t 	_y;
		
		constexpr Point8(): _x(0), _y(0){}
		constexpr Point8(_int_t in_x, _int_t in_y): _x(in_x), _y(in_y){}
		constexpr Point8(hash_t hash_v):
		_x(static_cast<_uint_t>(hash_v >> HASH_SHIFT)),
		_y(static_cast<_uint_t>(hash_v & HASH_MASK)){}

		// __m256i get_vector_mask() const;

		std::array<self_t, 4> adjacent_points() const;
		std::array<self_t, 8> transformed_points() const;

		self_t 	get_rotate_clockwise_90() const;
		self_t 	get_rotate_clockwise_180() const;
		self_t 	get_rotate_clockwise_270() const;
		
		self_t 	get_reflect_line_0() const;
		self_t 	get_reflect_line_45() const;
		self_t 	get_reflect_line_90() const;
		self_t 	get_reflect_line_135() const;

		hash_t point_to_hash() const;
		std::string str() const;

		self_t operator +(const self_t other) const{return self_t(_x + other._x, _y + other._y);}
		self_t operator -(const self_t other) const{return self_t(_x - other._x, _y - other._y);}

		static bool is_equal(const self_t a, const self_t b);
		static bool is_not_equal(const self_t a, const self_t b);
		static bool is_less_than(const self_t a, const self_t b);
		static bool is_great_than(const self_t a, const self_t b);
	};
	struct Point8_cmp{
		bool operator()(const Point8 a, const Point8 b) const{
			if(a._y < b._y){return true;}
			else if(a._y == b._y && a._x < b._x){return true;}
			else{return false;}
		}
	};

	struct alignas(64) memo_Polyomino{
		using self_t	= memo_Polyomino;
		using hash_t	= std::uint64_t;
		using point_t  	= Point8;
		using _set_t	= boost::container::flat_set<point_t, Point8_cmp>;
		using _id_t		= std::uint32_t;
		using _mask_t   = std::uint8_t;

		static constexpr _id_t ID_MAX = UINT32_MAX;

		_set_t	_cells;
		_set_t  _empty_cells;
		_id_t	_parent_id;
		_id_t	_num_child;
		_mask_t _mask[8];

		memo_Polyomino():
		_cells(), _empty_cells(), _parent_id(ID_MAX), _num_child(ID_MAX){
			constexpr std::size_t MIN_SIZE = 32 / sizeof(point_t);
			_cells.reserve(MIN_SIZE);
			_empty_cells.reserve(MIN_SIZE * 2);
		}

		memo_Polyomino(int cells_size, int empty_cells_size):
		_cells(), _empty_cells(), _parent_id(ID_MAX), _num_child(ID_MAX){
			_cells.reserve(cells_size);
			_empty_cells.reserve(empty_cells_size);
		}

		memo_Polyomino(const self_t&) =default;
		memo_Polyomino(self_t&&) = default;
		~memo_Polyomino() = default;

		void generate_adj_empty_cells();

		point_t shift_to_origin();

		self_t get_rotate_clockwise_90() const;
		self_t get_rotate_clockwise_180() const;
		self_t get_rotate_clockwise_270() const;

		self_t get_reflect_line_0() const;
		self_t get_reflect_line_45() const;
		self_t get_reflect_line_90() const;
		self_t get_reflect_line_135() const;

		hash_t poly_to_hash() const;
		
		void print_size() const;
		void print(const char* text = "") const;
		void print_py_image() const;

		static self_t base_case();
	};

	struct fixed_Polyomino{
		using self_t	= fixed_Polyomino;
		using point_t	= Point8;
		using point_cmp	= Point8_cmp;
		using hash_t	= std::uint64_t;
		using _set_t	= boost::container::flat_set<point_t, point_cmp>;
		
		_set_t	_cells;
		_set_t	_empty_cells;

		fixed_Polyomino():
		_cells(), _empty_cells(){
			constexpr std::size_t MIN_SIZE = (256 / 8) / sizeof(point_t);
			_cells.reserve(MIN_SIZE);
			_empty_cells.reserve(MIN_SIZE);
		}

		fixed_Polyomino(const self_t& rhs):
		_cells(rhs._cells),
		_empty_cells(rhs._empty_cells){}

		void	grow_with_child(point_t child);

		point_t shift_to_origin();

		point_t	grow_and_shift(point_t child);

		hash_t	poly_to_hash() const;

		void	base_case();

		void 	print_size() const;
		void 	print(const char* text = "") const;
		void 	print_py_image() const;
	};

	struct free_Polyomino{
		using self_t	= free_Polyomino;

		using _poly_t	= fixed_Polyomino;
		using _id_t		= std::uint32_t;
		using _mask_t	= std::uint8_t;

		using hash_t	= _poly_t::hash_t;
		using point_t	= _poly_t::point_t;

		static constexpr _id_t 		EMPTY_ID 	= UINT32_MAX;
		static constexpr _mask_t 	EMPTY_MASK 	= 0;

		_poly_t	_fixed_class[8];
		point_t	_offset[8];
		_id_t	_parent_id;
		_mask_t _num_child;
		_mask_t _sym_mask;


		free_Polyomino():
		_fixed_class(), 
		_offset(),
		_parent_id(EMPTY_ID),
		_num_child(EMPTY_MASK), 
		_sym_mask(EMPTY_MASK){}

		free_Polyomino(const self_t& rhs):
		_fixed_class(),
		_offset(),
		_parent_id(EMPTY_ID),
		_num_child(EMPTY_MASK),
		_sym_mask(EMPTY_MASK){
			for(std::size_t i = 0; i < 8; ++i){
				_fixed_class[i]._cells = rhs._fixed_class[i]._cells;
				_fixed_class[i]._empty_cells = rhs._fixed_class[i]._empty_cells;	
			}
		}

		free_Polyomino(const memo_Polyomino& rhs):	
		_fixed_class(),
		_offset(),
		_parent_id(EMPTY_ID),
		_num_child(EMPTY_MASK), 
		_sym_mask(EMPTY_MASK){

			for(const point_t p : rhs._cells){
				auto trans_array = p.transformed_points();
				for(std::size_t i = 0; i < 8; ++i){
					_fixed_class[i]._cells.emplace(trans_array[i]);
				}
			}

			for(const point_t p : rhs._empty_cells){
				auto trans_array = p.transformed_points();
				for(std::size_t i = 0; i < 8; ++i){
					_fixed_class[i]._empty_cells.emplace(trans_array[i]);
				}
			}

			for(int i = 0; i < 8; ++i){
				_offset[i] = _fixed_class[i].shift_to_origin();
			}
		}

		std::array<hash_t, 8> fixed_class_hash() const;

		void grow_and_shift(point_t child);

		void print() const;

	};

	struct symmetry_mask{
		using self_t	= symmetry_mask;
		using mask_t	= std::uint8_t;
		using text_t	= const char*;
		using index_t	= std::uint8_t;
		using poly_t	= free_Polyomino;
		using set_t		= poly_t::_poly_t::_set_t;
		using point_t	= poly_t::point_t;

		struct sym_action{
			static constexpr mask_t mask_r0     = 1U << 0;
			static constexpr mask_t mask_r90    = 1U << 1;
			static constexpr mask_t mask_r180   = 1U << 2;
			static constexpr mask_t mask_r270   = 1U << 3;

			static constexpr mask_t mask_rf0    = 1U << 4;
			static constexpr mask_t mask_rf45   = 1U << 5;
			static constexpr mask_t mask_rf90   = 1U << 6;
			static constexpr mask_t mask_rf135  = 1U << 7;

			static constexpr mask_t mask_array[7] = {
				mask_r90, mask_r180, mask_r270, mask_rf0, mask_rf45, mask_rf90, mask_rf135
			};

			static constexpr text_t text_r0     = "R0";
			static constexpr text_t text_r90    = "R90";
			static constexpr text_t text_r180   = "R180";
			static constexpr text_t text_r270   = "R270";

			static constexpr text_t text_rf0    = "RF0";
			static constexpr text_t text_rf45   = "RF45";
			static constexpr text_t text_rf90   = "RF90";
			static constexpr text_t text_rf135  = "RF135";
			static constexpr text_t text_array[7] = {
				text_r90, text_r180, text_r270, text_rf0, text_rf45, text_rf90, text_rf135
			};
		};
		struct sym_group{
			static constexpr mask_t mask_trivial_g8	= sym_action::mask_r0;

			static constexpr mask_t mask_c2_1_g4	= sym_action::mask_r0 | sym_action::mask_rf0;
			static constexpr mask_t mask_c2_2_g4	= sym_action::mask_r0 | sym_action::mask_rf45;
			static constexpr mask_t mask_c2_3_g4	= sym_action::mask_r0 | sym_action::mask_rf90;
			static constexpr mask_t mask_c2_4_g4	= sym_action::mask_r0 | sym_action::mask_rf135;
			static constexpr mask_t mask_f2_g4		= sym_action::mask_r0 | sym_action::mask_r180;

			static constexpr mask_t mask_d2_1_g2
				= sym_action::mask_r0 | sym_action::mask_r180 | sym_action::mask_rf0 | sym_action::mask_rf90;
			static constexpr mask_t mask_d2_2_g2
				= sym_action::mask_r0 | sym_action::mask_r180 | sym_action::mask_rf45 | sym_action::mask_rf135;
			static constexpr mask_t mask_c4_g2
				= sym_action::mask_r0 | sym_action::mask_r90 | sym_action::mask_r180 | sym_action::mask_r270;
			
			static constexpr mask_t mask_d4_g1
				= sym_action::mask_r0 | sym_action::mask_r90 | sym_action::mask_r180 | sym_action::mask_r270 |
				sym_action::mask_rf0| sym_action::mask_rf45| sym_action::mask_rf90 | sym_action::mask_rf135;
		
			static constexpr text_t text_trivial_g8	= "\tThe Trivial G {R0}\n\t: 1 Free -> 8 Fixed";

			static constexpr text_t text_c2_1_g4	= "C2_1 {R0, RF0}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t text_c2_2_g4	= "C2_2 {R0, RF45}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t text_c2_3_g4	= "C2_3 {R0, RF90}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t text_c2_4_g4	= "C2_4 {R0, RF135}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t text_f2_g4		= "F2 {R0, R180}\n\t: 1 Free -> 4 Fixed";

			static constexpr text_t text_d2_1_g2 	= "D2_1 {R0, R180, RF0, RF90}\n\t: 1 Free -> 2 Fixed";
			static constexpr text_t text_d2_2_g2 	= "D2_2 {R0, R180, RF45, RF135}\n\t: 1 Free -> 2 Fixed";
			static constexpr text_t text_c4_g2 		= "C4 {R0, R90, R180, R270}\n\t: 1 Free -> 2 Fixed";
			
			static constexpr text_t text_d4_g1 		= "D4 {R0, R90, R180, R270, RF0, RF45, RF90, RF135}\n\t: 1 Free -> 1 Fixed";
		};

		struct sym_group_transformed_mapping{
			static constexpr index_t mapping_c2_1_g4[]	= {4};
			static constexpr index_t mapping_c2_2_g4[]	= {5};
			static constexpr index_t mapping_c2_3_g4[]	= {6};
			static constexpr index_t mapping_c2_4_g4[]	= {7};
			static constexpr index_t mapping_f2_g4[]	= {2};

			static constexpr index_t mapping_d2_1_g2[]	= {2, 4, 6};
			static constexpr index_t mapping_d2_2_g2[]	= {2, 5, 7};
			static constexpr index_t mapping_c4_g2[]	= {1, 2, 3};

			static constexpr index_t mapping_d4_g1[]	= {1 ,2, 3, 4, 5, 6, 7};
		};

		static mask_t 	set_sym(const poly_t& target);
		static set_t 	get_unique_child(const poly_t& target);
	};

	struct unified_container{
		using	poly_t = free_Polyomino;
		
	};

	void sort_and_insert_set(memo_Polyomino::_set_t& flat_set, std::vector<memo_Polyomino::point_t>& vector);

	bool compare_translation(const memo_Polyomino& first, const memo_Polyomino& second);

	bool compare_fixed_poly(const fixed_Polyomino& first, const fixed_Polyomino& second);

}

#endif