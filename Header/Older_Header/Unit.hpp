#include <chrono>
#include <iostream>
#include <cmath>

#include <cstdint>
#include <functional>

#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_map>

#define MEASURE_UNIT std::chrono::microseconds
#define UNIT_STR    "US"

namespace timing{
    template <typename T>
    struct timer_unit<std::chrono::high_resolution_clock>{
        using unit = void;
        static constexpr const char* unit_text = "UNKNOW";
    };
    template <>
    struct timer_unit<std::chrono::high_resolution_clock>{
        using unit = std::chrono::high_resolution_clock;
        static constexpr const char* unit_text = "CLOCK";
    };
    template <>
    struct timer_unit<std::chrono::nanoseconds>{
        using unit = std::chrono::nanoseconds;
        static constexpr const char* unit_text = "NS";
    };
    template <>
    struct timer_unit<std::chrono::microseconds>{
        using unit = std::chrono::microseconds;
        static constexpr const char* unit_text = "US";
    };
    template <>
    struct timer_unit<std::chrono::milliseconds>{
        using unit = std::chrono::milliseconds;
        static constexpr const char* unit_text = "MS";
    };
    template <>
    struct timer_unit<std::chrono::seconds>{
        using unit = std::chrono::seconds;
        static constexpr const char* unit_text = "S";
    };
    
    template <typename T = std::chrono::milliseconds>
    struct timer{
        std::chrono::time_point<std::chrono::high_resolution_clock> before;
        std::chrono::time_point<std::chrono::high_resolution_clock> after;
        void start(){before = std::chrono::high_resolution_clock::now();}
        void finish(){after = std::chrono::high_resolution_clock::now();}
        void end_print(const char* text = ""){
            finish();
            std::cout<<"\nDuration "<<text<<" = "<<
            std::chrono::duration_cast<T>(after - before).count()<<
            timer_unit<T>::unit_text<<"\n\n";
        }
        std::int64_t end_time(){
            finish();
            return std::chrono::duration_cast<T>(after - before).count();
        }
    };
    struct benchmark_result{
        //mean -> average, mode -> most common, median -> middle, range (max - min), min, max
        int64_t mean = 0;
        int64_t mode = 0;
        int64_t percentile_50 = 0;
        int64_t percentile_90 = 0;
        int64_t percentile_95 = 0;
        int64_t min = 0;
        int64_t max = 0;
        int64_t range = 0;
        uint64_t result_mask = 0;
        std::vector<int64_t> sorted_record;

        void find_mean(const std::vector<int64_t>& record){
            mean = static_cast<int64_t>(std::accumulate(record.cbegin(), record.cend(), __int128_t(0)) / record.size());
        }
        void find_mode(const std::vector<int64_t>& record){
            int64_t item = 0;
            int64_t max = 0;
            std::unordered_map<int64_t, uint32_t> table;
            table.reserve(record.size() << 1);
            for(const auto i : record){
                auto search = table.find(i);
                if(search != table.end()){
                    ++search->second;
                }
                else{
                    table.insert({i, 1});
                }
            }
            for(const auto i : table){
                if(i.second > max){
                    item = i.first;
                    max = i.second;
                }
            }
            this->mode = item;
        }
        int64_t compute_percentile(const std::vector<int64_t>& record, const unsigned position){
            size_t rank = (position * (record.size() / 100)) - 1;
            return record[rank];
        }
        void find_min(const std::vector<int64_t>& record){
            min = *record.begin();
        }
        void find_max(const std::vector<int64_t>& record){
            max = *record.rbegin();
        }
        void find_range(const std::vector<int64_t>& record){
            range = *record.rbegin() - *record.begin();
        }
        void get_record(std::vector<int64_t>&& record){
            sorted_record = std::move(record);
        }
        void take_record_and_compute_result(std::vector<int64_t> record, uint64_t in_mask){
            std::sort(record.begin(), record.end());
            get_record(std::move(record));
            find_mean(sorted_record);
            find_mode(sorted_record);
            percentile_50 = compute_percentile(sorted_record, 50);
            percentile_90 = compute_percentile(sorted_record, 90);
            percentile_95 = compute_percentile(sorted_record, 95);
            find_min(sorted_record);
            find_max(sorted_record);
            find_range(sorted_record);
            result_mask = in_mask;
        }
        void result_print(bool print_record = false){
            std::cout <<"\nMeasure in "<<UNIT_STR<<
                        "\n\tMean (Total time / # Record) = " << mean <<
                        "\n\tMode (Most common pattern) = " << mode <<
                        "\n\tPercentile_X (Item >= X% of Item)\n\t\tP50 = " << percentile_50 <<
                        "\tP90 = " << percentile_90 <<
                        "\tP95 = " << percentile_95 <<
                        "\n\tRange -> (Max - Min) = " << range <<
                        " -> ("<< max<<" - "<< min <<")" << 
                        "\n\tResult Mask = " << result_mask << "\n";
            
            if(!print_record) return;

            std::cout<<std::endl<<"Result record: [";
            for(auto i : sorted_record){
                std::cout<<i<<", ";
            }
            std::cout<<"]\n\n";
        }
        void histogram(int64_t bins = 26){
            int64_t width = range / bins;

            std::vector<int64_t> counts(bins);
            for (int64_t x : sorted_record) {
                int idx = std::min<int>((x - min) / width, bins - 1);
                counts[idx]++;
            }

            int maxcount = *std::max_element(counts.begin(), counts.end());
            for (int i = 0; i < bins; ++i) {
                int low = min + i * width;
                int high = low + width;
                int barlen = (100.0 * counts[i]) / maxcount; // 50 chars wide
                std::cout << "[" << low << "," << high << ") ";
                std::cout << std::string(barlen, '#') << " " << counts[i] << "\n";
            }
            std::cout<<std::endl;
        }

    };
    struct timing_and_mask{
        int64_t timing;
        uint64_t mask;
        timing_and_mask(): timing(0), mask(0){}
        timing_and_mask(int64_t t, uint64_t m): timing(t), mask(m){}
    };
    
    template<typename F>
    inline benchmark_result benchmark(F&& in_testing_function, size_t repeat){
        //F&& in_testing_function = callable function object with no arguments
        //repeat = number of time for repeat testing_function
        std::vector<int64_t> record;
        record.reserve(repeat);

        int64_t sum = 0;
        int64_t mask = 0;

        for(size_t i = 0; i < repeat; ++i){
            timing_and_mask temp = in_testing_function();
            record.emplace_back(temp.timing);
            sum += temp.timing;
            mask ^= temp.mask;
        }

        std::cout<<"\nTotal runtime = "<<sum<<"\tRepeat time = "<<repeat<<"\tMask = "<<mask;

        benchmark_result result;
        result.take_record_and_compute_result(record, mask);
        return result;
    }
}

namespace point_test{
    struct Point_int32{
        int32_t _x;
        int32_t _y;

        Point_int32(): _x(0), _y(0){}
        Point_int32(int32_t in_x, int32_t in_y): _x(in_x), _y(in_y){}

        bool operator ==(const Point_int32 other) const{
            if(_x == other._x && _y == other._y){
                return true;
            }
            return false;
        }
        bool operator !=(const Point_int32 other)const{
            if(_x == other._x && _y == other._y){
                return false;
            }
            return true;
        }

        bool operator <(const Point_int32 other)const{
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

        Point_int32 operator +(const Point_int32 other) const{
            return Point_int32(_x + other._x, _y + other._y);
        }
        Point_int32 operator -(const Point_int32 other) const{
            return Point_int32(_x - other._x, _y - other._y);
        }
        Point_int32 operator *(const Point_int32 other) const{
            return Point_int32(_x * other._x, _y * other._y);
        }
        Point_int32 operator /(const Point_int32 other) const{
            return Point_int32(_x / other._x, _y / other._y);
        }

        Point_int32 move_up() const{
            return Point_int32(_x, _y + 1);
        }
        Point_int32 move_right() const{
            return Point_int32(_x + 1, _y);
        }
        Point_int32 move_down() const{
            return Point_int32(_x, _y - 1);
        }
        Point_int32 move_left() const{
            return Point_int32(_x - 1, _y);
        }

        Point_int32 rotate_clockwise_90() const{
            return Point_int32(_y, - _x);
        }
        Point_int32 rotate_clockwise_180() const{
            return Point_int32(- _x, - _y);
        }
        Point_int32 rotate_clockwise_270() const{
            return Point_int32(- _y, _x);
        }
        
        Point_int32 reflect_x_axis() const{
            return Point_int32(_x, - _y);
        }
        Point_int32 reflect_line_45() const{
            return Point_int32(_y, _x);
        }
        Point_int32 reflect_y_axis() const{
            return Point_int32(- _x, _y);
        }
        Point_int32 reflect_line_135() const{
            return Point_int32(- _y, - _x);
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
        static Point_int32 int_to_pair(uint64_t hash){
            return Point_int32(
                static_cast<int32_t>(hash >> 32),
                static_cast<int32_t>(hash & 0xFFFFFFFF)
            );
        }
    };

    struct Point_int8{
        int8_t _x;
        int8_t _y;

        Point_int8(): _x(0), _y(0){}
        Point_int8(int8_t in_x, int8_t in_y): _x(in_x), _y(in_y){}

        bool operator ==(const Point_int8 other) const{
            if(_x == other._x && _y == other._y){
                return true;
            }
            return false;
        }
        bool operator !=(const Point_int8 other)const{
            if(_x == other._x && _y == other._y){
                return false;
            }
            return true;
        }

        bool operator <(const Point_int8 other)const{
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

        Point_int8 operator +(const Point_int8 other) const{
            return Point_int8(_x + other._x, _y + other._y);
        }
        Point_int8 operator -(const Point_int8 other) const{
            return Point_int8(_x - other._x, _y - other._y);
        }
        Point_int8 operator *(const Point_int8 other) const{
            return Point_int8(_x * other._x, _y * other._y);
        }
        Point_int8 operator /(const Point_int8 other) const{
            return Point_int8(_x / other._x, _y / other._y);
        }

        Point_int8 move_up() const{
            return Point_int8(_x, _y + 1);
        }
        Point_int8 move_right() const{
            return Point_int8(_x + 1, _y);
        }
        Point_int8 move_down() const{
            return Point_int8(_x, _y - 1);
        }
        Point_int8 move_left() const{
            return Point_int8(_x - 1, _y);
        }

        Point_int8 rotate_clockwise_90() const{
            return Point_int8(_y, - _x);
        }
        Point_int8 rotate_clockwise_180() const{
            return Point_int8(- _x, - _y);
        }
        Point_int8 rotate_clockwise_270() const{
            return Point_int8(- _y, _x);
        }
        
        Point_int8 reflect_x_axis() const{
            return Point_int8(_x, - _y);
        }
        Point_int8 reflect_line_45() const{
            return Point_int8(_y, _x);
        }
        Point_int8 reflect_y_axis() const{
            return Point_int8(- _x, _y);
        }
        Point_int8 reflect_line_135() const{
            return Point_int8(- _y, - _x);
        }

        constexpr uint16_t pair_to_int() const{
            uint16_t a = static_cast<uint16_t>(static_cast<uint8_t>(_x));
            uint16_t b = static_cast<uint16_t>(static_cast<uint8_t>(_y));
            return (a << 8) | b;
        }

        static constexpr int8_t int_to_x(uint16_t hash){
            return (hash >> 8);
        }
        static constexpr int8_t int_to_y(uint16_t hash){
            return hash & 0xFF;
        }
        static Point_int8 int_to_pair(uint16_t hash){
            return Point_int8(
                static_cast<int8_t>(hash >> 8),
                static_cast<int8_t>(hash & 0xFF)
            );
        }
    };

    namespace test_case{
        static volatile int64_t store = 0;
        std::vector<Point_int32> prepare_empty_v32(size_t size);
        std::vector<Point_int8> prepare_empty_v8(size_t size);
        std::vector<Point_int32> prepare_random_point_v32(
            size_t size,
            std::uniform_int_distribution<std::mt19937::result_type> dist,
            std::mt19937 mt
        );
        std::vector<Point_int8> prepare_random_point_v8(
            size_t size,
            std::uniform_int_distribution<std::mt19937::result_type> dist,
            std::mt19937 mt
        );

        timing::timing_and_mask testing_template();
        uint64_t masking_point_32(std::vector<Point_int32>& v);
        uint64_t masking_point_8(std::vector<Point_int8>& v);

        timing::timing_and_mask constructor_adjacent_point_32(std::vector<Point_int32>& v, std::vector<Point_int32>& result);
        timing::timing_and_mask constructor_adjacent_point_8(std::vector<Point_int8>& v, std::vector<Point_int8>& result);
        
    }
}