#include "../../Header/Concurrent_Parallel.hpp"
#include "../../Header/Timing.hpp"

#include <vector>
#include <functional>

#include <mutex>

struct threading_condition{
    using cmp_t = std::uint64_t;

    std::uint32_t _NUM_THREAD = 1;
    cmp_t _FINAL_CMP = INT64_MAX;

    constexpr threading_condition(std::uint32_t i, cmp_t cmp)
    : _NUM_THREAD(i) , _FINAL_CMP(cmp){}
};

void single_increment(){
    volatile std::uint64_t i = 0;

    while(i < (INT64_MAX >> 29)){
        i = i + 1;
    }
}

void mutex_increment(const threading_condition tc){
    threading_condition::cmp_t i = 0;

    auto task = [&](threading_condition::cmp_t& i, std::mutex& my_lock){

        while(1){
            std::lock_guard<std::mutex> guard(my_lock);

            if(i > tc._FINAL_CMP){
                break;
            }
            i = i + 1;     
        }

        return;

    };

    std::vector<std::thread> v;
    std::mutex lock;

    for(std::uint32_t num = 0; num < tc._NUM_THREAD; ++num){
        v.emplace_back(task, std::ref(i), std::ref(lock));
    }

    for(auto& t : v){
        t.join();
    }
}

void spin_increment(const threading_condition tc){
    threading_condition::cmp_t i = 0;

    auto task = [&](threading_condition::cmp_t& i, concurrent_parallel::spinlock& my_lock){

        while(1){
            my_lock.lock();

            if(i > tc._FINAL_CMP){
                my_lock.unlock();
                break;
            }
            i = i + 1;

            my_lock.unlock();
        }

        return;

    };

    std::vector<std::thread> v;
    concurrent_parallel::spinlock lock;

    for(std::uint32_t num = 0; num < tc._NUM_THREAD; ++num){
        v.emplace_back(task, std::ref(i), std::ref(lock));
    }

    for(auto& t : v){
        t.join();
    }
}

int main(void){
    threading_condition tc(32, (INT64_MAX >> 29));


    timing::measure("\tSingle Increment", single_increment);
    // timing::measure("\tmutex Increment",mutex_increment, tc);
    timing::measure("\tSpin lock Increment", spin_increment, tc);
}