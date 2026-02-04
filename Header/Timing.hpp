#pragma once
#ifndef TIMING_HPP
#define TIMING_HPP

#include <cstdint>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>

namespace timing{
    template <typename T>
    struct timer_unit{
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
            std::cout<<"\n\n\x1b[36mDuration\t"<<text<<" = "<<
            std::chrono::duration_cast<T>(after - before).count()<<
            timer_unit<T>::unit_text<<"\x1b[0m\n\n";
        }
        std::int64_t end_time(){
            finish();
            return std::chrono::duration_cast<T>(after - before).count();
        }
    };

    template <typename Unit = std::chrono::milliseconds, typename F, typename... Args>
    void measure(F&& function, Args&&... argument){
        timer<Unit> my_timer;
        my_timer.start();

        std::forward<F>(function)(std::forward<Args>(argument)...);

        my_timer.end_print();
    }


    template <typename T = std::chrono::milliseconds>
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
        void find_mode(const std::vector<int64_t>& record) {
            int64_t best_value = record[0];
            std::size_t best_count = 1;

            int64_t current_value = record[0];
            std::size_t current_count = 1;

            for (std::size_t i = 1; i < record.size(); ++i) {
                if (record[i] == current_value) {
                    ++current_count;
                } else {
                    // end of current run
                    if (current_count > best_count) {
                        best_count = current_count;
                        best_value = current_value;
                    }
                    current_value = record[i];
                    current_count = 1;
                }
            }

            // check the final run
            if (current_count > best_count) {
                best_count = current_count;
                best_value = current_value;
            }

            this->mode = best_value;
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
        void result_print(bool print_record, std::size_t data_size){
            std::cout <<"\nMeasure in "<<timer_unit<T>::unit_text<<
                        "\tData size = "<<data_size<<
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
        void histogram(int64_t bins = 26) {
            if (bins <= 0) return;
            if (sorted_record.empty()) return;

            // All values the same → single spike
            if (range == 0) {
                std::vector<int64_t> counts(bins, 0);
                counts[bins / 2] = sorted_record.size();  // put everything in middle bin

                // int64_t maxcount = counts[bins / 2];
                for (int i = 0; i < bins; ++i) {
                    int64_t low  = min;
                    int64_t high = min;
                    int barlen = 100;  // whole bar
                    std::cout << "[" << low << "," << high << "] ";
                    std::cout << std::string(barlen, '#') << " " << counts[i] << "\n";
                }
                std::cout << std::endl;
                return;
            }

            // Use double width
            double width = double(range) / bins;

            std::vector<int64_t> counts(bins);
            for (int64_t x : sorted_record) {
                double pos = double(x - min) / width;
                int idx = int(pos);
                if (idx < 0) idx = 0;
                if (idx >= bins) idx = bins - 1;
                counts[idx]++;
            }

            int64_t maxcount = *std::max_element(counts.begin(), counts.end());
            if (maxcount == 0) return; // just in case

            for (int i = 0; i < bins; ++i) {
                double low  = min + i * width;
                double high = min + (i + 1) * width;
                int barlen = int(100.0 * counts[i] / maxcount);
                std::cout << "[" << low << "," << high << ") ";
                std::cout << std::string(barlen, '#') << " " << counts[i] << "\n";
            }
            std::cout << std::endl;
        }


    };
    struct timing_and_mask{
        int64_t timing;
        uint64_t mask;
        timing_and_mask(): timing(0), mask(0){}
        timing_and_mask(int64_t t, uint64_t m): timing(t), mask(m){}
    };
    
    template<typename T = std::chrono::milliseconds, typename F>
    inline benchmark_result<T> benchmark(F&& in_testing_function, std::size_t repeat){
        //F&& in_testing_function = callable function object with no arguments
        //repeat = number of time for repeat testing_function
        std::vector<int64_t> record;
        record.reserve(repeat);

        int64_t sum = 0;
        int64_t mask = 0;

        for(size_t i = 0; i < repeat; ++i){
            timing_and_mask temp = std::forward<F>(in_testing_function)();
            record.emplace_back(temp.timing);
            sum += temp.timing;
            mask ^= temp.mask;
        }

        std::cout<<"\nTotal runtime = "<<sum<<"\tRepeat time = "<<repeat<<"\tMask = "<<mask;

        benchmark_result<T> result;
        result.take_record_and_compute_result(record, mask);
        return result;
    }
}

#endif