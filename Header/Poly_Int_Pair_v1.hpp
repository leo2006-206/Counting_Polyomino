#pragma once
#ifndef POLY_INT_PAIR_V1_HPP
#define POLY_INT_PAIR_V1_HPP

// File IO header
#include <iostream>
#include <fstream>

// Typing header
#include <cstdint>

// Container header
// #include <array>
#include <vector>
#include <set>
// #include <queue>
#include <unordered_set>
#include <iterator>

// #include <boost/container/flat_set.hpp>

// Threading header
// #include <thread>
// #include <mutex>
// #include <condition_variable>
// #include <atomic>
// #include <functional>

// Timing header
// #include <chrono>

// Algorithm header
#include <algorithm>

#define MY_UINT32_MAX (UINT32_MAX - 1)

namespace int_pair_v1{

	struct Point32{
		int32_t _x;
		int32_t _y;

		Point32(): _x(0), _y(0){}
		Point32(int32_t in_x, int32_t in_y): _x(in_x), _y(in_y){}

		bool operator ==(const Point32 other) const{
			if(_x == other._x && _y == other._y){
				return true;
			}
			return false;
		}
		bool operator !=(const Point32 other)const{
			if(_x == other._x && _y == other._y){
				return false;
			}
			return true;
		}

		bool operator <(const Point32 other)const{
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

		Point32 operator +(const Point32 other) const{
			return Point32(_x + other._x, _y + other._y);
		}
		Point32 operator -(const Point32 other) const{
			return Point32(_x - other._x, _y - other._y);
		}
		Point32 operator *(const Point32 other) const{
			return Point32(_x * other._x, _y * other._y);
		}
		Point32 operator /(const Point32 other) const{
			return Point32(_x / other._x, _y / other._y);
		}

		Point32 move_up() const{
			return Point32(_x, _y + 1);
		}
		Point32 move_right() const{
			return Point32(_x + 1, _y);
		}
		Point32 move_down() const{
			return Point32(_x, _y - 1);
		}
		Point32 move_left() const{
			return Point32(_x - 1, _y);
		}

		Point32 rotate_clockwise_90() const{
			return Point32(_y, - _x);
		}
		Point32 rotate_clockwise_180() const{
			return Point32(- _x, - _y);
		}
		Point32 rotate_clockwise_270() const{
			return Point32(- _y, _x);
		}
		
		Point32 reflect_x_axis() const{
			return Point32(_x, - _y);
		}
		Point32 reflect_line_45() const{
			return Point32(_y, _x);
		}
		Point32 reflect_y_axis() const{
			return Point32(- _x, _y);
		}
		Point32 reflect_line_135() const{
			return Point32(- _y, - _x);
		}

		constexpr uint64_t pair_to_int() const{
			uint64_t a = static_cast<uint64_t>(static_cast<uint32_t>(_x));
			uint64_t b = static_cast<uint64_t>(static_cast<uint32_t>(_y));
			return (a << 32) | b;
		}

		static constexpr int32_t int_to_x(uint64_t hash){
			return (hash >> 32);
		}
		static constexpr int32_t int_to_y(uint64_t hash){
			return hash & 0xFFFFFFFF;
		}
		static Point32 int_to_pair(uint64_t hash){
			return Point32(
				static_cast<int32_t>(hash >> 32),
				static_cast<int32_t>(hash & 0xFFFFFFFF)
			);
		}
	};

	struct Point32_cmp{
		bool operator()(const Point32 a, const Point32 b) const{
			if(a._y < b._y){return true;}
			else if(a._y == b._y && a._x < b._x){return true;}
			else{return false;}
		}
	};

	struct vector_Polyomino{
		std::vector<Point32> _cells;

		vector_Polyomino(size_t size){
			_cells.reserve(size);
		}

		bool operator ==(const vector_Polyomino& other) const{
			for(size_t i=0; i<_cells.size(); ++i){
				if(_cells[i] != other._cells[i]){
					return false;
				}
			}
			return true;
		}

		vector_Polyomino rotate_clockwise_90() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].rotate_clockwise_90());
			}
			//result.sort();
			return result;
		}
		vector_Polyomino rotate_clockwise_180() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].rotate_clockwise_180());
			}
			//result.sort();
			return result;
		}
		vector_Polyomino rotate_clockwise_270() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].rotate_clockwise_270());
			}
			//result.sort();
			return result;
		}

		vector_Polyomino reflect_x_axis() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].reflect_x_axis());
			}
			//result.sort();
			return result;
		}
		vector_Polyomino reflect_line_45() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].reflect_line_45());
			}
			//result.sort();
			return result;
		}
		vector_Polyomino reflect_y_axis() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].reflect_y_axis());
			}
			//result.sort();
			return result;
		}
		vector_Polyomino reflect_line_135() const{
			vector_Polyomino result(_cells.size());
			for(size_t i=0; i<_cells.size(); ++i){
				result._cells.push_back(_cells[i].reflect_line_135());
			}
			//result.sort();
			return result;
		}
		

		void sort(){
			std::sort(_cells.begin(), _cells.end(), [](const Point32 a, const Point32 b){
				if(a._y < b._y){
					return true;
				}
				else if(a._y == b._y && a._x < b._x){
					return true;
				}
				else{
					return false;
				}
			});
		}
		void print() const{
			std::cout<<"\n      # cells: "<<_cells.size()<<"  ";
			for(size_t i = 0; i < _cells.size(); ++i){
				std::cout<<"("<<_cells[i]._x<<", "<<_cells[i]._y<<")  ";
			}
		}
	};

	struct BST_Polyomino{
		std::set<Point32, Point32_cmp>_cells;
		std::uint32_t _parent;
		std::uint32_t _child;
		
		BST_Polyomino(): _cells(), _parent(MY_UINT32_MAX), _child(MY_UINT32_MAX){}
		BST_Polyomino(std::uint32_t parent_ID): _cells(), _parent(parent_ID), _child(MY_UINT32_MAX){}

		BST_Polyomino rotate_clockwise_90() const{
			BST_Polyomino result;
			for(auto i = _cells.cbegin(); i != _cells.cend(); ++i){
				result._cells.emplace(i->rotate_clockwise_90());
			}
			return result;
		}
		BST_Polyomino rotate_clockwise_180() const{
			BST_Polyomino result;
			for(auto i = _cells.cbegin(); i != _cells.cend(); ++i){
				result._cells.emplace(i->rotate_clockwise_180());
			}
			return result;
		}
		BST_Polyomino rotate_clockwise_270() const{
			BST_Polyomino result;
			for(auto i = _cells.cbegin(); i != _cells.cend(); ++i){
				result._cells.emplace(i->rotate_clockwise_270());
			}
			return result;
		}

		BST_Polyomino reflect_x_axis() const{
			BST_Polyomino result;
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				result._cells.emplace(i->reflect_x_axis());
			}
			return result;
		}
		BST_Polyomino reflect_line_45() const{
			BST_Polyomino result;
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				result._cells.emplace(i->reflect_line_45());
			}
			return result;
		}
		BST_Polyomino reflect_y_axis() const{
			BST_Polyomino result;
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				result._cells.emplace(i->reflect_y_axis());
			}
			return result;
		}
		BST_Polyomino reflect_line_135() const{
			BST_Polyomino result;
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				result._cells.emplace(i->reflect_line_135());
			}
			return result;
		}

		void print(const char* string) const{
			std::cout<<"\n      "<<string<<"    # cells: "<<_cells.size()<<"  ";
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				std::cout<<"("<<i->_x<<", "<<i->_y<<")  ";
			}
		}
		void print() const{
			std::cout<<"\n      # cells: "<<_cells.size()<<"  ";
			for(auto i = _cells.begin(); i != _cells.end(); ++i){
				std::cout<<"("<<i->_x<<", "<<i->_y<<")  ";
			}
		}

		static BST_Polyomino base_case(){
			BST_Polyomino result;
			result._cells.insert(Point32(0, 0));
			return result;
		}
	};

	struct vector_Group{
		std::vector<std::vector<vector_Polyomino>> _array;

		vector_Group(size_t size);
		void print(){
			for(size_t i = 0; i < _array.size(); ++i){
				std::cout<<"\nnumber of cell = "<<_array[i].size();
				for(auto j = _array[i].begin(); j != _array[i].end(); ++j){
					j->print();
				}
				std::cout<<"\n";
			}
		}
	};

	struct BST_Group{
		std::vector<std::vector<BST_Polyomino>> _array;

		BST_Group() : _array() {}

		BST_Group(size_t num_cell);

		void print(){
			for(size_t i = 0; i < _array.size(); ++i){
				std::cout<<"\nnumber of cell = "<<i+1<<"\tnumber of Polyomino = "<<_array[i].size();
				// for(size_t j = 0; j != _array[i].size(); ++j){
				//     _array[i][j].print();
				// }
				// for(auto j = _array[i].cbegin(); j != _array[i].cend(); ++j){
				//     j->print();
				// }
				std::cout<<"\n";
			}
		}
		void file_write(const char* in_file_name){
			std::fstream csv_file(in_file_name);
			if(csv_file.is_open()){
				std::cout<<"file opened";
				csv_file<<"# cells -> N"<<","<<
						"Free group(N)"<<","<<
						"Index in Free group(N)"<<","<<
						"Parent index in Free group(N-1)"<<","<<
						"# Child with N + 1 cells"<<","<<
						"Polyomino Point32 Set"<<"\n";

				for(size_t i = 0; i < _array.size(); ++i){
					for(size_t j = 0; j != _array[i].size(); ++j){
						csv_file<<i+1<<","<<
							_array[i].size()<<","<<
							j<<","<<
							_array[i][j]._parent<<","<<
							_array[i][j]._child<<",";
						for(auto k = _array[i][j]._cells.cbegin(); k != _array[i][j]._cells.cend(); ++k){
							csv_file<<"("<<k->_x<<". "<<k->_y<<") ";
						}
						csv_file<<"\n";
					}
				}
			}
			else{std::cout<<"fuck";}
			csv_file.close();
		}
	};
	
	inline vector_Polyomino base_case(){
		vector_Polyomino base(1);
		base._cells.push_back(Point32(0, 0));
		return base;
	}

	constexpr uint64_t pairing(uint32_t a, uint32_t b){
		return ((uint64_t) a << 32 | (uint64_t) b);
	}

	//vector_Polyomino version function

	bool find_cell(const uint64_t, const std::vector<uint64_t>& body);

	std::vector<vector_Polyomino> generate_vector(const vector_Polyomino& root);

	std::vector<vector_Polyomino> generate_hash_table(const vector_Polyomino& root);

	std::vector<vector_Polyomino> remove_duplication(std::vector<vector_Polyomino>& body);

	int compare_translation(const vector_Polyomino& first, const vector_Polyomino& second);

	bool compare_rotation(const vector_Polyomino& first, const vector_Polyomino& second);

	bool compare_reflection(const vector_Polyomino& first, const vector_Polyomino& second);

	//BST Polyomino version function

	std::vector<BST_Polyomino> generate_hash_table(const BST_Polyomino& root, const std::uint32_t parent_ID);

	std::vector<BST_Polyomino> remove_duplication(std::vector<BST_Polyomino>& body);

	int compare_translation(const BST_Polyomino& first, const BST_Polyomino& second);

	bool compare_rotation(const BST_Polyomino& first, const BST_Polyomino& second);

	bool compare_reflection(const BST_Polyomino& first, const BST_Polyomino& second);

	namespace test_case{
		inline void int_pair_testing1(){
			using namespace int_pair_v1;

			std::vector<BST_Polyomino> v;
			BST_Polyomino a[4], b[4];
			for(size_t i=0; i<5; ++i){
				a[0]._cells.insert(Point32(i, 0));
				b[0]._cells.insert(Point32(i, 5));
			}
			a[1] = a[0].rotate_clockwise_90();
			a[2] = a[0].rotate_clockwise_180();
			a[3] = a[0].rotate_clockwise_270();
			b[1] = b[0].rotate_clockwise_90();
			b[2] = b[0].rotate_clockwise_180();
			b[3] = b[0].rotate_clockwise_270();
			
			for(size_t i=0; i<4; ++i){
				a[i].print();
				v.emplace_back(a[i]);
			}
			std::cout<<"\n123\n";
			for(size_t i=0; i<4; ++i){
				b[i].print();
				v.emplace_back(b[i]);
			}
			for(size_t i=0; i<4; ++i){
				std::cout<<std::endl<<compare_rotation(a[0], b[i]);
			}
			// v = remove_duplication(v);
			for(auto i : v){
				i.print();
			}
			std::cout<<"\n123\n";
			std::vector<BST_Polyomino> result;
			for(size_t i = 0; i<v.size(); ++i){
				v[i].print("i");
				for(size_t j = i+1; j < v.size(); ++j){
					v[j].print("\tj");
				}
			}
			std::cout<<"\n123\n";
			for (auto it_i = v.begin(); it_i != v.end(); ++it_i) {
				it_i->print("i");

				for (auto it_j = std::next(it_i); it_j != v.end();) {
					it_j->print("\tj");
					if(compare_rotation(*it_i, *it_j) || compare_reflection(*it_i, *it_j)){
						it_j = v.erase(it_j);
					}
					else{++it_j;}
				}
			}

			v = remove_duplication(v);
			for(auto i : v){
				i.print("end");
			}
		}

		inline void Generate_Free_Polyomino(std::size_t num_cells, const char* file_path = ""){
			BST_Group group(num_cells);
			group.print();
			if(file_path && file_path[0] != 0){
				group.file_write(file_path);
			}
		}
	}
}


#endif