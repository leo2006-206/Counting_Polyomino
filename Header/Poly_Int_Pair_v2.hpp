#pragma once
#ifndef POLY_INT_PAIR_V2_HPP
#define POLY_INT_PAIR_V2_HPP

// File IO header
#include <iostream>
#include <fstream>

// Typing header
#include <cstdint>
#include <cmath>
#include <cstring>
// for hypot and memcmp

// Container header
#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <boost/container/flat_set.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/unordered/unordered_node_map.hpp>

#include <algorithm>

#define MY_UINT32_MAX (UINT32_MAX - 1)

namespace int_pair_v2{
	struct Point8{
		using self_t = Point8;
		using point_int_t = std::int8_t;
		using point_uint_t = std::uint8_t;
		using hash_t = std::uint16_t;
		// hash = x << 8 | y
		// shift = 8
		// x = hash >> 8
		// y = hash & 0xFF

		point_int_t _x;
		point_int_t _y;

		Point8(): _x(0), _y(0){}
		Point8(point_int_t in_x, point_int_t in_y): _x(in_x), _y(in_y){}
		Point8(hash_t hash):
		_x(static_cast<point_uint_t>(hash >> 8)),
		_y(static_cast<point_uint_t>(hash & 0xFF)) {}
		// Point8(const self_t&) = default;
		// Point8(self_t&&) = default;
		// ~Point8() = default;


		bool operator ==(const self_t other) const;
		bool operator !=(const self_t other)const;
		bool operator <(const self_t other)const;

		self_t operator +(const self_t other) const;
		self_t operator -(const self_t other) const;
		self_t operator *(const self_t other) const;
		self_t operator /(const self_t other) const;

		self_t get_up() const;
		self_t get_right() const;
		self_t get_down() const;
		self_t get_left() const;
		std::array<self_t, 4> get_adjacent_points() const;

		self_t get_rotate_clockwise_90() const;
		self_t get_rotate_clockwise_180() const;
		self_t get_rotate_clockwise_270() const;
		
		self_t get_reflect_line_0() const;
		self_t get_reflect_line_45() const;
		self_t get_reflect_line_90() const;
		self_t get_reflect_line_135() const;

		constexpr hash_t point_to_hash() const{
			hash_t a = static_cast<hash_t>(static_cast<point_uint_t>(_x));
			hash_t b = static_cast<hash_t>(static_cast<point_uint_t>(_y));
			return (a << 8) | b;
		}

		static constexpr point_int_t hash_to_x(hash_t hash){
			return (hash >> 8);
		}
		static constexpr point_int_t hash_to_y(hash_t hash){
			return hash & 0xFF;
		}
		static self_t hash_to_point(hash_t hash){
			return self_t(
				static_cast<point_uint_t>(hash >> 8),
				static_cast<point_uint_t>(hash & 0xFF)
			);
		}

		void print() const{
			std::cout << "\t(" << static_cast<int>(_x) << "," << static_cast<int>(_y) << "),";
		}
	};

	// constexpr auto Point8_cmp = [](const Point8 a, const Point8 b){
	// 	if(a._y < b._y){return true;}
	// 	else if(a._y == b._y && a._x < b._x){return true;}
	// 	else{return false;}
	// };

	struct Point8_cmp{
		bool operator()(const Point8 a, const Point8 b) const{
			if(a._y < b._y){return true;}
			else if(a._y == b._y && a._x < b._x){return true;}
			else{return false;}
		}
	};

	struct memo_Polyomino{
		using point_t = Point8;
		using self_t = memo_Polyomino;
		using set = boost::container::flat_set<point_t, Point8_cmp>;
		using set_size_t = std::uint8_t;
		using id_t = std::uint32_t;
		using hash_t = std::uint64_t;

		set     _non_empty_cells;
		set     _empty_cells;
		id_t    _parent_id;
		id_t    _num_child;

		memo_Polyomino() :
		_non_empty_cells(), _empty_cells(), _parent_id(MY_UINT32_MAX), _num_child(MY_UINT32_MAX){}

		memo_Polyomino(set_size_t nonEmpty_cells_size, set_size_t Empty_cells_size) :
		_non_empty_cells(), _empty_cells(), _parent_id(MY_UINT32_MAX), _num_child(MY_UINT32_MAX) {
			_non_empty_cells.reserve(nonEmpty_cells_size);
			_empty_cells.reserve(Empty_cells_size);
		}

		memo_Polyomino(set_size_t nonEmpty_cells_size, set_size_t Empty_cells_size, id_t parent) :
		_non_empty_cells(), _empty_cells(), _parent_id(parent), _num_child(MY_UINT32_MAX) {
			_non_empty_cells.reserve(nonEmpty_cells_size);
			_empty_cells.reserve(Empty_cells_size);
		}

		memo_Polyomino(set_size_t nonEmpty_cells_size, set_size_t Empty_cells_size, id_t parent, id_t child) :
		_non_empty_cells(), _empty_cells(), _parent_id(parent), _num_child(child) {
			_non_empty_cells.reserve(nonEmpty_cells_size);
			_empty_cells.reserve(Empty_cells_size);
		}

		memo_Polyomino(const self_t&) =default;
		memo_Polyomino(self_t&&) = default;
		~memo_Polyomino() = default;

		point_t get_min_cell() const;

		point_t shift_to_origin();

		void generate_adj_empty_cells();

		self_t get_rotate_clockwise_90() const;
		self_t get_rotate_clockwise_180() const;
		self_t get_rotate_clockwise_270() const;

		self_t get_reflect_line_0() const;
		self_t get_reflect_line_45() const;
		self_t get_reflect_line_90() const;
		self_t get_reflect_line_135() const;

		std::vector<self_t> generate_duplicated_child() const;

		std::vector<self_t> generate_unique_child() const;

		self_t generate_new_child(point_t p) const;

		hash_t poly_to_hash(){
			// auto mix_hash = [](hash_t x){
			// 	x += 0x9e3779b97f4a7c15ull;
			// 	x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
			// 	x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
			// 	return x ^ (x >> 31);
			// };

			// hash_t hash_value = mix_hash(_non_empty_cells.size() + _empty_cells.size());

			// hash_t i = 0;
			// for(const point_t p : _non_empty_cells){
			// 	hash_value = mix_hash(p.point_to_hash() + i);
			// 	++i;
			// }
			// for(const point_t p : _empty_cells){
			// 	hash_value ^= mix_hash(p.point_to_hash() + i);
			// 	++i;
			// }
			// return hash_value;

			auto big_mix = [](hash_t x){
				x += 0x9e3779b97f4a7c15ull;
				x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
				x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
				return x ^ (x >> 31);
			};
			// auto small_mix = [](hash_t x, hash_t y){
			//     return std::rotl(x, 47) + y;
			// };

			hash_t acc1 = big_mix(_non_empty_cells.size());
			hash_t acc2 = big_mix(_empty_cells.size());

			auto* cell_ptr = &*_non_empty_cells.begin();
			std::size_t cell_n = _non_empty_cells.size() - 1;
			for(std::size_t i = 0; i < cell_n; i += 2){
				acc1 = big_mix(acc1 + cell_ptr[i].point_to_hash() + i);
				acc2 = big_mix(acc2 + cell_ptr[i+1].point_to_hash() + i);
			}

			auto* empty_ptr = &*_empty_cells.begin();
			std::size_t empty_n = _empty_cells.size() - 1;
			for(std::size_t i = 0; i < empty_n; i += 2){
				acc1 = big_mix(acc1 + empty_ptr[i].point_to_hash() + i);
				acc2 = big_mix(acc2 + empty_ptr[i+1].point_to_hash() + i);
			}

			return big_mix(acc1 ^ acc2);
		}

		void print_non_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Non Empty Cells: "<<
						_non_empty_cells.size()<<"\t";

			int count = 5;
			for(point_t p : _non_empty_cells){
				if(count == 5){
					std::cout<<"\n\t\t";
					count = 0;
				}
				count++;
				std::cout<<"("<<static_cast<int>(p._x)<<", "<<static_cast<int>(p._y)<<")  ";
			}
		}

		void print_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Empty Cells: "<<
						_empty_cells.size()<<"\t";

			int count = 5;
			for(point_t p : _empty_cells){
				if(count == 5){
					std::cout<<"\n\t\t";
					count = 0;
				}
				count++;
				std::cout<<"("<<static_cast<int>(p._x)<<", "<<static_cast<int>(p._y)<<")  ";
			}
		}

		void print(const char* text = "") const{
			std::cout << "\n\tparent = " << _parent_id << "\t# child = " << _num_child;
			print_non_empty_cells(text);
			print_empty_cells(text);
			std::cout << "\n";
		}

		void py_image_print(){
			std::cout<<"\nnon_empty = [";
			for(point_t& p : _non_empty_cells){
				std::cout<<"\n";
				p.print();
			}
			std::cout<<"\n]";

			std::cout<<"\nempty = [";
			for(point_t& p : _empty_cells){
				std::cout<<"\n";
				p.print();
			}
			std::cout<<"\n]\n";
		}

		static self_t base_case(){
			self_t result(1, 4);
			result._non_empty_cells.emplace(0, 0);
			result.generate_adj_empty_cells();
			return result;
		}
	};

	struct symmetry_Poly{
		using self_t = symmetry_Poly;
		using poly_t = memo_Polyomino;
		using point_t = poly_t::point_t;
		using mask_t = std::uint8_t;
		using text_t = const char*;

		struct sym_mask{
			static constexpr mask_t r0     = 1U << 0;
			static constexpr mask_t r90    = 1U << 1;
			static constexpr mask_t r180   = 1U << 2;
			static constexpr mask_t r270   = 1U << 3;

			static constexpr mask_t rf0    = 1U << 4;
			static constexpr mask_t rf45   = 1U << 5;
			static constexpr mask_t rf90   = 1U << 6;
			static constexpr mask_t rf135  = 1U << 7;

			static constexpr mask_t sym_action[7] = {
				r90, r180, r270, rf0, rf45, rf90, rf135
			};
		};
		struct sym_text{
			static constexpr text_t text_r0     = "R0";
			static constexpr text_t text_r90    = "R90";
			static constexpr text_t text_r180   = "R180";
			static constexpr text_t text_r270   = "R270";

			static constexpr text_t text_rf0    = "RF0";
			static constexpr text_t text_rf45   = "RF45";
			static constexpr text_t text_rf90   = "RF90";
			static constexpr text_t text_rf135  = "RF135";
			static constexpr text_t sym_action[7] = {
				text_r90, text_r180, text_r270, text_rf0, text_rf45, text_rf90, text_rf135
			};
		};
		struct group_mask{
			static constexpr mask_t trivial_g8	= sym_mask::r0;

			static constexpr mask_t c2_1_g4	= sym_mask::r0 | sym_mask::rf0;
			static constexpr mask_t c2_2_g4	= sym_mask::r0 | sym_mask::rf45;
			static constexpr mask_t c2_3_g4	= sym_mask::r0 | sym_mask::rf90;
			static constexpr mask_t c2_4_g4	= sym_mask::r0 | sym_mask::rf135;
			static constexpr mask_t f2_g4	= sym_mask::r0 | sym_mask::r180;

			static constexpr mask_t d2_1_g2
				= sym_mask::r0 | sym_mask::r180 | sym_mask::rf0 | sym_mask::rf90;
			static constexpr mask_t d2_2_g2
				= sym_mask::r0 | sym_mask::r180 | sym_mask::rf45 | sym_mask::rf135;
			static constexpr mask_t c4_g2
				= sym_mask::r0 | sym_mask::r90 | sym_mask::r180 | sym_mask::r270;
			
			static constexpr mask_t d4_g1
				= sym_mask::r0 | sym_mask::r90 | sym_mask::r180 | sym_mask::r270 |
				  sym_mask::rf0| sym_mask::rf45| sym_mask::rf90 | sym_mask::rf135;
		};
		struct group_text{
			static constexpr text_t trivial_g8	= "\tThe Trivial G {R0}\n\t: 1 Free -> 8 Fixed";

			static constexpr text_t c2_1_g4	= "C2_1 {R0, RF0}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t c2_2_g4	= "C2_2 {R0, RF45}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t c2_3_g4	= "C2_3 {R0, RF90}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t c2_4_g4	= "C2_4 {R0, RF135}\n\t: 1 Free -> 4 Fixed";
			static constexpr text_t f2_g4	= "F2 {R0, R180}\n\t: 1 Free -> 4 Fixed";

			static constexpr text_t d2_1_g2 = "D2_1 {R0, R180, RF0, RF90}\n\t: 1 Free -> 2 Fixed";
			static constexpr text_t d2_2_g2 = "D2_2 {R0, R180, RF45, RF135}\n\t: 1 Free -> 2 Fixed";
			static constexpr text_t c4_g2 	= "C4 {R0, R90, R180, R270}\n\t: 1 Free -> 2 Fixed";
			
			static constexpr text_t d4_g1 	= "D4 {R0, R90, R180, R270, RF0, RF45, RF90, RF135}\n\t: 1 Free -> 1 Fixed";
		};

		mask_t _mask = 1;

		symmetry_Poly(): _mask(sym_mask::r0) {}
		symmetry_Poly(const self_t&) =default;
		symmetry_Poly(self_t&&) = default;
		~symmetry_Poly() = default;

		void set_symmetry(const poly_t& poly);

		void print_sym_action() const{
			mask_t temp = _mask;
			std::cout << "\nAction\t" << static_cast<int>(_mask);
			for(int i = 6; i > -1; --i){
				if(sym_mask::sym_action[i] & temp){
					std::cout << "\n\t" << sym_text::sym_action[i]; 
				}
			}
		}
		void print_nonsym_action() const{
			mask_t temp = ~_mask;
			std::cout << "\nNon Action\t" << static_cast<int>(_mask);
			for(int i = 6; i > -1; --i){
				if(sym_mask::sym_action[i] & temp){
					std::cout << "\n\t" << sym_text::sym_action[i]; 
				}
			}
		}
		void print_group() const{
			mask_t temp = _mask;
			std::cout<<"\nGroup mask = " << static_cast<int>(_mask);
			switch(temp){
				case group_mask::trivial_g8:
					std::cout<<"\n"<<group_text::trivial_g8<<"\n";
					break;

				case group_mask::c2_1_g4:
					std::cout<<"\n"<<group_text::c2_1_g4<<"\n";
					break;
				case group_mask::c2_2_g4:
					std::cout<<"\n"<<group_text::c2_2_g4<<"\n";
					break;
				case group_mask::c2_3_g4:
					std::cout<<"\n"<<group_text::c2_3_g4<<"\n";
					break;
				case group_mask::c2_4_g4:
					std::cout<<"\n"<<group_text::c2_4_g4<<"\n";
					break;
				case group_mask::f2_g4:
					std::cout<<"\n"<<group_text::f2_g4<<"\n";
					break;
				
				case group_mask::d2_1_g2:
					std::cout<<"\n"<<group_text::d2_1_g2<<"\n";
					break;
				case group_mask::d2_2_g2:
					std::cout<<"\n"<<group_text::d2_2_g2<<"\n";
					break;
				case group_mask::c4_g2:
					std::cout<<"\n"<<group_text::c4_g2<<"\n";
					break;
				
				case group_mask::d4_g1:
					std::cout<<"\n"<<group_text::d4_g1<<"\n";
					break;

				default:
					std::cout<<"\nUnknown";
			}
		}

		poly_t::set generate_unique_child(const poly_t& poly);
		poly_t::set generate_unique_child2(const poly_t& poly);
	};

	struct Polyomino_class{
		using poly_t = memo_Polyomino;
		using point_t = memo_Polyomino::point_t;
		using hash_t = memo_Polyomino::hash_t;
		using hash_table_t = boost::unordered::unordered_node_map<hash_t, std::size_t>;
		using hash_container_t = std::vector<hash_table_t>;
		using poly_container_t = std::vector<std::vector<poly_t>>;

		hash_container_t hash_container;
		poly_container_t poly_container;

		Polyomino_class(std::size_t num_cell);

		void print_poly() const;
		void print_hash() const{
			std::size_t n = 1;
			for(auto& sub_hash_table : hash_container){
				std::cout << "\nNum cell = " << n << "\t# hash = " << sub_hash_table.size();
				n++;

				using my_pair = std::pair<hash_t, std::size_t>;
				std::vector<my_pair> temp_v;
				for(auto [hash, index] : sub_hash_table){
					temp_v.emplace_back(hash, index);
				}
				std::sort(temp_v.begin(), temp_v.end(), [](my_pair& a, my_pair& b){
					return (a.second < b.second);
				});

				for(auto pair : temp_v){
					std::cout << "\n\thash = " << pair.first << "\tindex = " << pair.second;
				}
			}
		}
		void print() const{
			print_poly();
			print_hash();
		}
		void print_num() const{
			std::size_t n = 1;
			for(auto& sub_v : poly_container){
				std::cout << "\nNum cell = " << n << "\t# Poly = " << sub_v.size();
				n++;
			}
			std :: cout << "\n";
			std::size_t m = 1;
			for(auto& sub_hash_table : hash_container){
				std::cout << "\nNum cell = " << m << "\t# hash = " << sub_hash_table.size();
				m++;
			}
		}
	};



	std::array<memo_Polyomino::point_t, 7> find_transformed_min(const memo_Polyomino::set& flat_set);

	void sort_and_insert_set(memo_Polyomino::set& flat_set, std::vector<memo_Polyomino::point_t>& vector);

	bool compare_translation(const memo_Polyomino& first, const memo_Polyomino& second);

	std::array<memo_Polyomino::hash_t, 8> hash_free_poly(memo_Polyomino& poly);

	std::uint64_t distance_of_point_set(const memo_Polyomino::set& point_set);
	std::pair<std::uint64_t, std::uint64_t> norms_of_point_set(const memo_Polyomino::set& point_set);

	namespace pair_float{
		struct Pointfloat{
			using self_t = Pointfloat;
			using point_int_t = float;
			using id_t = std::int32_t;

			point_int_t _x;
			point_int_t _y;
			point_int_t _distance;
			id_t _id;

			Pointfloat()
				: _x(0), _y(0), _distance(0), _id(0){}
			Pointfloat(point_int_t in_x, point_int_t in_y)
				: _x(in_x), _y(in_y), _distance(0), _id(0){}
			Pointfloat(point_int_t in_x, point_int_t in_y, point_int_t in_dis, id_t in_id)
				: _x(in_x), _y(in_y), _distance(in_dis), _id(in_id){}
			Pointfloat(int_pair_v2::Point8 p)
				: _x(p._x), _y(p._y), _distance(0), _id(0){}
			Pointfloat(int_pair_v2::Point8 p, id_t in_id)
				: _x(p._x), _y(p._y), _distance(0), _id(in_id){}

			bool operator ==(const self_t other) const{
				if(_x == other._x && _y == other._y && _distance == other._distance){
					return true;
				}
				return false;
			}
			bool operator !=(const self_t other)const{
				if(_x == other._x && _y == other._y && _distance == other._distance){
					return false;
				}
				return true;
			}

			bool operator <(const self_t other)const{
				if(_y < other._y){
					return true;
				}
				else if(_y == other._y && _x < other._x){
					return true;
				}
				else{
					return false;
				}
			}

			void shift_by(const self_t& other){
				_x = _x - other._x;
				_y = _y - other._y;
				_distance = std::hypot(other._x, other._y);
			}

			self_t get_shift_by(const self_t& other) const{
				return self_t(
					_x - other._x,
					_y - other._y,
					std::hypot(_x - other._x, _y - other._y),
					_id
				);
			}

			self_t operator +(const self_t other) const{
				return self_t(_x + other._x, _y + other._y);
			}
			self_t operator -(const self_t other) const{
				return self_t(_x - other._x, _y - other._y);
			}
			self_t operator *(const self_t other) const{
				return self_t(_x * other._x, _y * other._y);
			}
			self_t operator /(const self_t other) const{
				return self_t(_x / other._x, _y / other._y);
			}

			self_t move_up() const{
				return self_t(_x, _y + 1);
			}
			self_t move_right() const{
				return self_t(_x + 1, _y);
			}
			self_t move_down() const{
				return self_t(_x, _y - 1);
			}
			self_t move_left() const{
				return self_t(_x - 1, _y);
			}

			std::array<self_t, 4> get_adjacent_points() const{
				return {
					move_up(),
					move_right(),
					move_down(),
					move_left()
				};
			}

			self_t rotate_clockwise_90() const{
				return self_t(_y, - _x, _distance, _id);
			}
			self_t rotate_clockwise_180() const{
				return self_t(- _x, - _y, _distance, _id);
			}
			self_t rotate_clockwise_270() const{
				return self_t(- _y, _x, _distance, _id);
			}
			
			self_t reflect_line_0() const{
				return self_t(_x, - _y, _distance, _id);
			}
			self_t reflect_line_45() const{
				return self_t(_y, _x, _distance, _id);
			}
			self_t reflect_line_90() const{
				return self_t(- _x, _y, _distance, _id);
			}
			self_t reflect_line_135() const{
				return self_t(- _y, - _x, _distance, _id);
			}

			static self_t compute_controid(const int_pair_v2::memo_Polyomino& other){
				self_t result;
				for(int_pair_v2::memo_Polyomino::point_t p : other._non_empty_cells){
					result._x += static_cast<point_int_t>(p._x);
					result._y += static_cast<point_int_t>(p._y);
				}
				// std::cout<<result._x<<"\t"<<result._y;
				result._x = result._x / other._non_empty_cells.size();
				result._y = result._y / other._non_empty_cells.size();
				result._distance = 0;
				return result;
			}
		};

		struct Pointfloat_cmp{
			bool operator()(const Pointfloat a, const Pointfloat b) const{
				if(a._y < b._y){return true;}
				else if(a._y == b._y && a._x < b._x){return true;}
				else{return false;}
			}
		};

		struct float_Polyomino{
			using point_t = Pointfloat;
			using self_t = float_Polyomino;
			using set = boost::container::flat_set<point_t, Pointfloat_cmp>;
			using set_size_t = std::uint8_t;
			using id_t = std::uint32_t;

			set     _non_empty_cells;
			set     _empty_cells;
			point_t _centroid;
			id_t    _parent_id;
			id_t    _num_child;

			float_Polyomino() :
			_non_empty_cells(), _empty_cells(), _parent_id(MY_UINT32_MAX), _num_child(MY_UINT32_MAX){}

			float_Polyomino(set_size_t nonEmpty_cells_size, set_size_t Empty_cells_size) :
			_non_empty_cells(), _empty_cells(), _parent_id(MY_UINT32_MAX), _num_child(MY_UINT32_MAX) {
				_non_empty_cells.reserve(nonEmpty_cells_size);
				_empty_cells.reserve(Empty_cells_size);
			}

			float_Polyomino(set_size_t nonEmpty_cells_size, set_size_t Empty_cells_size, id_t parent) :
			_non_empty_cells(), _empty_cells(), _parent_id(parent), _num_child(MY_UINT32_MAX) {
				_non_empty_cells.reserve(nonEmpty_cells_size);
				_empty_cells.reserve(Empty_cells_size);
			}

			float_Polyomino(const int_pair_v2::memo_Polyomino& other){
				_centroid = point_t::compute_controid(other);
				point_t::id_t id = 1;
				for(int_pair_v2::memo_Polyomino::point_t p : other._non_empty_cells){
					this->_non_empty_cells.emplace(point_t(p, id).get_shift_by(_centroid));
					++id;
				}

				id = -1;
				for(int_pair_v2::memo_Polyomino::point_t p : other._empty_cells){
					this->_empty_cells.emplace(point_t(p, id).get_shift_by(_centroid));
					--id;
				}
				this->_parent_id = other._parent_id;
				this->_num_child = other._num_child;
			}

			float_Polyomino(const self_t&) =default;
			float_Polyomino(self_t&&) = default;
			~float_Polyomino() = default;

			self_t get_rotate_clockwise_90() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.rotate_clockwise_90();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.rotate_clockwise_90());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.rotate_clockwise_90());
				}
				return result;
			}

			self_t get_rotate_clockwise_180() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.rotate_clockwise_180();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.rotate_clockwise_180());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.rotate_clockwise_180());
				}
				return result;
			}

			self_t get_rotate_clockwise_270() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.rotate_clockwise_270();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.rotate_clockwise_270());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.rotate_clockwise_270());
				}
				return result;
			}

			self_t get_reflect_line_0() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.reflect_line_0();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.reflect_line_0());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.reflect_line_0());
				}
				return result;
			}

			self_t get_reflect_line_45() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.reflect_line_45();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.reflect_line_45());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.reflect_line_45());
				}
				return result;
			}

			self_t get_reflect_line_90() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.reflect_line_90();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.reflect_line_90());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.reflect_line_90());
				}
				return result;
			}

			self_t get_reflect_line_135() const{
				self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
				result._centroid = _centroid.reflect_line_135();
				for(point_t p : _non_empty_cells){
					result._non_empty_cells.emplace(p.reflect_line_135());
				}
				for(point_t p : _empty_cells){
					result._empty_cells.emplace(p.reflect_line_135());
				}
				return result;
			}

			std::vector<self_t> generate_duplicated_child() const{
				std::vector<self_t> result;
				result.reserve(_non_empty_cells.size());

				for(const point_t p : _empty_cells){
					self_t copy(*this);
					std::array<point_t, 4> adjacents = p.get_adjacent_points();

					copy._empty_cells.erase(p);
					copy._non_empty_cells.emplace(p);

					for(const point_t new_empty_p : adjacents){
						if(!copy._non_empty_cells.contains(new_empty_p)){
							copy._empty_cells.emplace(new_empty_p);
						}
					}
					result.emplace_back(copy);
				}

				return result;
			}



			void print_non_empty_cells(const char* text){
				std::cout<<	"\n\t"<<text<<"\t# Non Empty Cells: "<<
							_non_empty_cells.size()<<"\t";
				for(point_t p : _non_empty_cells){
					std::cout<<"\n\t\t\t(id = "<<p._id<<";\t"<<p._x<<",\t"<<p._y<<":\t"<<p._distance<<")  ";
				}
				std::vector<point_t> temp = sort_by_distance(_non_empty_cells);
				std::cout<<"\nsorted distance";
				for(point_t p : temp){
					std::cout<<"\n\t\t\t\t(id = "<<p._id<<";\t"<<p._x<<",\t"<<p._y<<":\t"<<p._distance<<")  ";
				}
			}

			void print_empty_cells(const char* text){
				std::cout<<	"\n\t"<<text<<"\t# Empty Cells: "<<
							_empty_cells.size()<<"\t";
				for(point_t p : _empty_cells){
					std::cout<<"\n\t\t\t(id = "<<p._id<<";\t"<<p._x<<",\t"<<p._y<<":\t"<<p._distance<<")  ";
				}
			}

			void print(const char* text = ""){
				std::cout<< "\n\tCentroid = ("<<_centroid._x<<","<<_centroid._y<<")";
				print_non_empty_cells(text);
				print_empty_cells(text);
			}

			void py_image_print(){
				std::cout<<"\ncentroid = ("<<_centroid._x<<", "<<_centroid._y<<")";

				std::cout<<"\nnon_empty = [";
				for(point_t& p : _non_empty_cells){
					std::cout<<"\n\t("<<p._id<<",\t"<<p._x<<",\t"<<p._y<<"),";
				}
				std::cout<<"\n]";

				std::cout<<"\nempty = [";
				for(point_t& p : _empty_cells){
					std::cout<<"\n\t("<<p._id<<",\t"<<p._x<<",\t"<<p._y<<"),";
				}
				std::cout<<"\n]";
			}

			static self_t base_case(){
				self_t result(1, 4);
				point_t p;
				result._non_empty_cells.emplace(p);
				auto adj = p.get_adjacent_points();
				for(auto ep : adj){
					result._empty_cells.emplace(ep);
				}
				return result;
			}

			static std::vector<point_t> sort_by_distance(set& flat_set){
				std::vector<point_t> temp;
				for(point_t& p : flat_set){
					temp.emplace_back(p);
				}
				std::sort(temp.begin(), temp.end(), [](point_t& a, point_t& b){
					return (a._distance < b._distance);
				});
				return temp;
			}
		};
	}

}

// void testing2_int_pair_v2(){
//     using namespace int_pair_v2;
//     memo_Polyomino b = memo_Polyomino::base_case();
//     b.print();
//     auto v = b.generate_duplicated_child();
//     for(auto& p : v){
//         p.print();
//     }
// }

// void testing3_int_pair_v2(){
//     using namespace int_pair_v2;
//     memo_Polyomino base(4, 8);

//     // static constexpr std::array<std::pair<int,int>, 4> non_empty = {{
//     //     {0, 0},
//     //     {0, 1},
//     //     {1, 0},
//     //     {1, 1}
//     // }};

//     // static constexpr std::array<std::pair<int,int>, 8> non_empty = {{
//     //     {1, 0},
//     //     {0, 0},
//     //     {0, -1},
//     //     {0, -2},
//     //     {0, -3},
//     //     {0, -4},
//     //     {0, -5},
//     //     {0, -6}
//     // }};

//     // static constexpr std::array<std::pair<int,int>, 8> non_empty = {{
//     //     {0,0},
//     //     {-1,0},
//     //     {-2,0},
//     //     {-3,0},
//     //     {-2,1},
//     //     {-3,1},
//     //     {-2,-1},
//     //     {-3,-1}
//     // }};

//     static constexpr std::array<std::pair<int,int>, 8> non_empty = {{
//         {0, 0},
//         {0, 1}
//     }};


//     for (auto [x, y] : non_empty) {
//         base._non_empty_cells.emplace(x, y);
//     }

//     memo_Polyomino next = base.get_reflect_line_45();
//     next.generate_adj_empty_cells();
//     next.shift_to_origin();

//     memo_Polyomino array[8] = {
//         next,
//         next.get_rotate_clockwise_90(),
//         next.get_rotate_clockwise_180(),
//         next.get_rotate_clockwise_270(),
//         next.get_reflect_line_0(),
//         next.get_reflect_line_45(),
//         next.get_reflect_line_90(),
//         next.get_reflect_line_135()
//     };

//     for(memo_Polyomino& p : array){
//         p.print("Before shift");
//         p.shift_to_origin();

//         symmetry_Poly mask;
//         mask.set_symmetry(p);
//         mask.print_group();
//         p.py_image_print();
//         auto v = mask.generate_unique_child2(next);
//         for(auto p : v){
//             std::cout << "\n V = ";
//             p.print();
//         }
//         std::cout <<"\n" << p.poly_to_hash() << "\n\n\n";
//     }

// }

// void testing4_int_pair_v2(){
//     using namespace int_pair_v2;
//     static std::array<std::pair<int, int>, 3> cells = {{
//         {0, 0},
//         {1, 0},
//         {0, 1}
//     }};

//     memo_Polyomino base;
//     memo_Polyomino next;

//     for(auto [a, b] : cells){
//         base._non_empty_cells.emplace(a, b);
//     }
//     for(auto [a, b] : cells){
//         next._non_empty_cells.emplace(a+1, b+1);
//     }

//     base.generate_adj_empty_cells();
//     base.shift_to_origin();
//     base.print();

//     next.generate_adj_empty_cells();
//     next.print();

//     std::cout   << "\nbase = " << distance_of_point_set(base._non_empty_cells)
//                 << "\nnext = " << distance_of_point_set(next._non_empty_cells);
// }

// void generate(){
//     using namespace int_pair_v2;

//     Polyomino_class all(7);
//     all.print();
// }

#endif