#ifndef POLYOMINO_HPP
#define POLYOMINO_HPP

// File IO header
#include <iostream>
#include <fstream>

// Typing header
#include <cstdint>

// Container header
#include <array>
#include <vector>
#include <set>
#include <queue>
#include <unordered_set>
#include <iterator>

#include <boost/container/flat_set.hpp>

// Threading header
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

// Timing header
#include <chrono>

// Algorithm header
#include <algorithm>

// #include <omp.h>
// #include <malloc.h>

#define MY_UINT32_MAX (UINT32_MAX - 1)

struct timer{
	std::chrono::time_point<std::chrono::high_resolution_clock> before;
	std::chrono::time_point<std::chrono::high_resolution_clock> after;
	void start(){before = std::chrono::high_resolution_clock::now();}
	void end(){
		after = std::chrono::high_resolution_clock::now();
		std::cout<<"\n\nDuration = "<<
		std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count()<<
		"ms\n\n";
	}
};

namespace threading{
	struct thread_pool{
		private:
		std::condition_variable _cv_queue;
		std::condition_variable _cv_finish;
		std::mutex _lock;
		std::vector<std::thread> _threads;
		std::queue<std::function<void()>> _queue;
		std::atomic_uint32_t _working{0};
		bool _shutdown;

		void thread_loop(){
			std::function<void (void)> job;
			while(1){
				{
					std::unique_lock<std::mutex> unique_l(_lock);
					while(!_shutdown && _queue.empty()){
						_cv_queue.wait(unique_l);
					}
					if(_shutdown && _queue.empty()){
						// std::cerr<<"Thread "<<i<<" terminates\n";
						return;
					}
					// std::cerr<<"Thread "<<i<<" does a job\t";
					job = std::move(_queue.front());
					_queue.pop();
				}
				++_working;
				job();
				--_working;
				{
					std::unique_lock<std::mutex> unique_l(_lock);
					if(_working == 0 && _queue.empty()){
						_cv_finish.notify_all();
					}
				}
			}
		}
		public:
		thread_pool(
			unsigned num_thread = std::thread::hardware_concurrency()
		) : _shutdown(false){
			_threads.reserve(num_thread);
			for(unsigned i = 0; i < num_thread; ++i){
				_threads.emplace_back(std::thread(&thread_pool::thread_loop, this));
			}
		}
		~thread_pool(){
			{
				std::unique_lock<std::mutex> unique_l(_lock);
				_shutdown = true;
				_cv_queue.notify_all();
			}
			for(auto& thread : _threads){
				thread.join();
			}
		}
		void enqueue_task(std::function<void(void)> task){
			std::unique_lock<std::mutex> unique_l(_lock);
			_queue.emplace(std::move(task));
			_cv_queue.notify_one();
		}
		bool is_working(){
			return (_working || !_queue.empty());
		}
		void until_finish(){
			std::unique_lock<std::mutex> unique_l(_lock);
			_cv_finish.wait(unique_l, [this]{
				return (_working == 0 && _queue.empty());
			});
		}
	};
}

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
}

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

		bool operator ==(const self_t other) const{
			if(_x == other._x && _y == other._y){
				return true;
			}
			return false;
		}
		bool operator !=(const self_t other)const{
			if(_x == other._x && _y == other._y){
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
			return self_t(_y, - _x);
		}
		self_t rotate_clockwise_180() const{
			return self_t(- _x, - _y);
		}
		self_t rotate_clockwise_270() const{
			return self_t(- _y, _x);
		}
		
		self_t reflect_line_0() const{
			return self_t(_x, - _y);
		}
		self_t reflect_line_45() const{
			return self_t(_y, _x);
		}
		self_t reflect_line_90() const{
			return self_t(- _x, _y);
		}
		self_t reflect_line_135() const{
			return self_t(- _y, - _x);
		}

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

		memo_Polyomino(const self_t&) =default;
		memo_Polyomino(self_t&&) = default;
		~memo_Polyomino() = default;

		self_t rotate_clockwise_90() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_90());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_90());
			}
			return result;
		}

		self_t rotate_clockwise_180() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_180());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_180());
			}
			return result;
		}

		self_t rotate_clockwise_270() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_270());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_270());
			}
			return result;
		}

		self_t reflect_line_0() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_0());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_0());
			}
			return result;
		}

		self_t reflect_line_45() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_45());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_45());
			}
			return result;
		}

		self_t reflect_line_90() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_90());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_90());
			}
			return result;
		}

		self_t reflect_line_135() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
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



		void print_non_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Non Empty Cells: "<<
						_non_empty_cells.size()<<"\t";
			for(point_t p : _non_empty_cells){
				std::cout<<"("<<static_cast<int>(p._x)<<", "<<static_cast<int>(p._y)<<")  ";
			}
		}

		void print_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Empty Cells: "<<
						_empty_cells.size()<<"\t";
			for(point_t p : _empty_cells){
				std::cout<<"("<<static_cast<int>(p._x)<<", "<<static_cast<int>(p._y)<<")  ";
			}
		}

		void print(const char* text = "") const{
			print_non_empty_cells(text);
			print_empty_cells(text);
		}

		static self_t base_case(){
			self_t result(1, 4);
			result._non_empty_cells.emplace(0, 0);
			result._empty_cells.emplace(0, 1);
			result._empty_cells.emplace(1, 0);
			result._empty_cells.emplace(0, -1);
			result._empty_cells.emplace(-1, 0);
			return result;
		}
	};

}

namespace pair_float{
	struct Pointfloat{
		using self_t = Pointfloat;
		using point_int_t = float;
		// using point_uint_t = std::uint8_t;
		// using hash_t = std::uint16_t;
		// // hash = x << 8 | y
		// // shift = 8
		// // x = hash >> 8
		// // y = hash & 0xFF

		point_int_t _x;
		point_int_t _y;

		Pointfloat(): _x(0), _y(0){}
		Pointfloat(point_int_t in_x, point_int_t in_y): _x(in_x), _y(in_y){}
		Pointfloat(int_pair_v2::Point8 p): _x(p._x), _y(p._y){}

		bool operator ==(const self_t other) const{
			if(_x == other._x && _y == other._y){
				return true;
			}
			return false;
		}
		bool operator !=(const self_t other)const{
			if(_x == other._x && _y == other._y){
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
			return self_t(_y, - _x);
		}
		self_t rotate_clockwise_180() const{
			return self_t(- _x, - _y);
		}
		self_t rotate_clockwise_270() const{
			return self_t(- _y, _x);
		}
		
		self_t reflect_line_0() const{
			return self_t(_x, - _y);
		}
		self_t reflect_line_45() const{
			return self_t(_y, _x);
		}
		self_t reflect_line_90() const{
			return self_t(- _x, _y);
		}
		self_t reflect_line_135() const{
			return self_t(- _y, - _x);
		}

		static self_t compute_controid(const int_pair_v2::memo_Polyomino& other){
			self_t result(0, 0);
			for(int_pair_v2::memo_Polyomino::point_t p : other._non_empty_cells){
				result._x += static_cast<point_int_t>(p._x);
				result._y += static_cast<point_int_t>(p._y);
			}
			// std::cout<<result._x<<"\t"<<result._y;
			result._x = result._x / other._non_empty_cells.size();
			result._y = result._y / other._non_empty_cells.size();
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
			for(int_pair_v2::memo_Polyomino::point_t p : other._non_empty_cells){
				this->_non_empty_cells.emplace(point_t(p) - _centroid);
			}
			for(int_pair_v2::memo_Polyomino::point_t p : other._empty_cells){
				this->_empty_cells.emplace(point_t(p) - _centroid);
			}
			this->_parent_id = other._parent_id;
			this->_num_child = other._num_child;
		}

		float_Polyomino(const self_t&) =default;
		float_Polyomino(self_t&&) = default;
		~float_Polyomino() = default;

		self_t rotate_clockwise_90() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_90());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_90());
			}
			return result;
		}

		self_t rotate_clockwise_180() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_180());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_180());
			}
			return result;
		}

		self_t rotate_clockwise_270() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.rotate_clockwise_270());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.rotate_clockwise_270());
			}
			return result;
		}

		self_t reflect_line_0() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_0());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_0());
			}
			return result;
		}

		self_t reflect_line_45() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_45());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_45());
			}
			return result;
		}

		self_t reflect_line_90() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
			for(point_t p : _non_empty_cells){
				result._non_empty_cells.emplace(p.reflect_line_90());
			}
			for(point_t p : _empty_cells){
				result._empty_cells.emplace(p.reflect_line_90());
			}
			return result;
		}

		self_t reflect_line_135() const{
			self_t result(_non_empty_cells.capacity(), _empty_cells.capacity());
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



		void print_non_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Non Empty Cells: "<<
						_non_empty_cells.size()<<"\t";
			for(point_t p : _non_empty_cells){
				std::cout<<"("<<p._x<<", "<<p._y<<")  ";
			}
		}

		void print_empty_cells(const char* text) const{
			std::cout<<	"\n\t"<<text<<"\t# Empty Cells: "<<
						_empty_cells.size()<<"\t";
			for(point_t p : _empty_cells){
				std::cout<<"("<<p._x<<", "<<p._y<<")  ";
			}
		}

		void print(const char* text = "") const{
			std::cout<< "\n\tCentroid = ("<<_centroid._x<<","<<_centroid._y<<")";
			print_non_empty_cells(text);
			print_empty_cells(text);
		}

		static self_t base_case(){
			self_t result(1, 4);
			result._non_empty_cells.emplace(0, 0);
			result._empty_cells.emplace(0, 1);
			result._empty_cells.emplace(1, 0);
			result._empty_cells.emplace(0, -1);
			result._empty_cells.emplace(-1, 0);
			return result;
		}
	};


}

#endif