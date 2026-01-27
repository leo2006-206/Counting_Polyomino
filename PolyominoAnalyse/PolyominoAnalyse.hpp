#include <cstdint>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cmath>

#include <vector>
#include <array>
#include <unordered_map>
#include <map>
#include <utility>

struct timer{
    std::chrono::time_point<std::chrono::high_resolution_clock> before;
    std::chrono::time_point<std::chrono::high_resolution_clock> after;
    void start(){before = std::chrono::high_resolution_clock::now();}
    void end(){
        after = std::chrono::high_resolution_clock::now();
        std::cout<<"\nDuration = "<<
        std::chrono::duration_cast<std::chrono::seconds>(after - before).count()<<
        "s\n";
    }
};

namespace int_pair{
    struct Point{
        int32_t _x;
        int32_t _y;

        Point(): _x(0), _y(0){}
        Point(int32_t in_x, int32_t in_y): _x(in_x), _y(in_y){}

        bool operator ==(const Point other) const{
            if(_x == other._x && _y == other._y){
                return true;
            }
            return false;
        }
        bool operator !=(const Point other)const{
            if(_x == other._x && _y == other._y){
                return false;
            }
            return true;
        }

        bool operator <(const Point other)const{
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

        Point operator +(const Point other) const{
            return Point(_x + other._x, _y + other._y);
        }
        Point operator -(const Point other) const{
            return Point(_x - other._x, _y - other._y);
        }
        Point operator *(const Point other) const{
            return Point(_x * other._x, _y * other._y);
        }
        Point operator /(const Point other) const{
            return Point(_x / other._x, _y / other._y);
        }

        Point move_up() const{
            return Point(_x, _y + 1);
        }
        Point move_right() const{
            return Point(_x + 1, _y);
        }
        Point move_down() const{
            return Point(_x, _y - 1);
        }
        Point move_left() const{
            return Point(_x - 1, _y);
        }

        Point rotate_clockwise_90() const{
            return Point(_y, - _x);
        }
        Point rotate_clockwise_180() const{
            return Point(- _x, - _y);
        }
        Point rotate_clockwise_270() const{
            return Point(- _y, _x);
        }
        
        Point reflect_x_axis() const{
            return Point(_x, - _y);
        }
        Point reflect_line_45() const{
            return Point(_y, _x);
        }
        Point reflect_y_axis() const{
            return Point(- _x, _y);
        }
        Point reflect_line_135() const{
            return Point(- _y, - _x);
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
        static Point int_to_pair(uint64_t hash){
            return Point(
                static_cast<int32_t>(hash >> 32),
                static_cast<int32_t>(hash & 0xFFFFFFFF)
            );
        }
    };

    inline void analysis_first_line(std::fstream& output){
        output<<"# cells -> N"<<","<<
                "Free group(N)"<<","<<
                "Index in Free group(N)"<<","<<
                "Parent index in Free group(N-1)"<<","<<
                "# Child from Enumeration"<<","<<
                "# Child from Analysis"<<","<<
                "Face = 4N"<<","<<
                "Bridge"<<","<<
                "Shared_Face_2"<<","<<
                "Shared_Face_3"<<","<<
                "Shared_Face_4"<<"\n";

        /*  
            [0] -> N,
            [1] -> Free group(N),
            [2] -> Index in Free group(N)
            [3] -> Parent index in Free group(N-1)
            [4] -> # Child from Enumeration
            [5] -> # Child from Analysis
            [6] -> Face = 4N
            [7] -> Bridge
            [8] -> Shared_Face_2
            [9] -> Shared_Face_3
            [10]-> Shared_Face_4
        */
    }

    inline void distribution_first_line(std::fstream& output, const char* name){
        output<<"# cells -> N"<<","<<
                name<<","<<
                "Frequency"<<"\n";
    }

    std::vector<std::string> split(std::string& line, const char delimiter);

    std::vector<uint32_t> str_to_unsigned(const std::vector<std::string>& tokens, size_t size);

    std::vector<Point> extract_point_from_string(std::string& line, unsigned size);

    void analyse_Polyomino(const std::vector<Point>& points, std::array<unsigned, 6>& result);

    void record_distribution(std::unordered_map<uint32_t, uint32_t>& table, uint32_t data);

    void record_distribution(std::map<uint32_t, uint32_t>& tree, uint32_t data);

    void record_distribution(std::map<uint64_t, uint32_t>& tree, uint64_t data);

    Point compute_distance_from_origin(const std::vector<Point>& points);
}