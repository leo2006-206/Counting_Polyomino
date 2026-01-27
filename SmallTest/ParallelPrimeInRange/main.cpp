
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <omp.h>
#include <cstdlib>

#include <utility>

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

// #define MY_ARRAY 1LL<<25
#define MY_ARRAY (1LL << 22)
#define NUM_THREAD (1 << 5)
#define SUB_TASK (1 << 15)

struct timer{
    std::chrono::time_point<std::chrono::high_resolution_clock> before;
    std::chrono::time_point<std::chrono::high_resolution_clock> after;
    void start(){before = std::chrono::high_resolution_clock::now();}
    void end(const char* text = ""){
        after = std::chrono::high_resolution_clock::now();
        std::cout<<"\nDuration Point "<<text<<" = "<<
        std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count()<<
        " ms\n\n";
    }
};

namespace Independent{
    void prime_in_range(long long start, long long end, long long& result){
        result = end - start;
        for(long long i = start; i < end; ++i){
            for(long long j = 2; j <= static_cast<long long>(std::sqrt(i)); ++j){
                if(i % j == 0){
                    result--;
                    break;
                }
            }
        }
        // return result;
    }

    void multiple(){
        long long result[NUM_THREAD];
        std::vector<std::thread> v;
        long long midd = MY_ARRAY/NUM_THREAD;
        for(int i = 0; i < NUM_THREAD ; ++i){
            // std::cout<<midd * i<<"\t"<<midd * (i + 1)<<"\n";
            v.push_back(std::thread(prime_in_range, midd * i, midd * (i + 1), std::ref(result[i])));
        }

        for(int i = 0; i < NUM_THREAD ; ++i){
            v[i].join();
            // std::cout<<result[i] <<"\t";
        }
        long long sum = 0;
        for(auto& i : result){
            sum += i;
        }
        std::cout<<"\n"<<sum;
    }

    void multiple_omp(){
        long long result = MY_ARRAY;
        
        // opm_set_num_threads(NUM_THREAD);

        #pragma omp parallel for reduction(-:result) schedule(dynamic, 1000)
        for(long long i = 0; i < MY_ARRAY; ++i){
            for(long long j = 2; j <= static_cast<long long>(std::sqrt(i)); ++j){
                if(i % j == 0){
                    result--;
                    break;
                }
            }
        }

        std::cout<<"\n"<<result;
    }
    void single(){
        long long result[2];

        long long midd = MY_ARRAY/2;
        prime_in_range(0, midd, std::ref(result[0]));
        prime_in_range(midd+1, MY_ARRAY, std::ref(result[1]));
        std::cout<<(result[0] + result[1]);
    }
}

namespace Dependent{
    void prime_in_list_single(long long start, long long end, std::vector<long long>& result){
        for(long long i = start; i < end; ++i){
            bool is_prime = true;
            for(long long j = 2; j <= static_cast<long long>(std::sqrt(i)); ++j){
                if(i % j == 0){
                    is_prime = false;
                    break;
                }
            }
            if(is_prime){
                result.emplace_back(i);
            }
        }
    }

    void single(){
        std::vector<long long> v;
        prime_in_list_single(2, MY_ARRAY, v);
        std::cout<<v.size();
    }

    std::mutex gLock;
    std::vector<long long> shared_v;

    void list_add(long long item){
        gLock.lock();
            shared_v.emplace_back(item);
        gLock.unlock();
    }

    void prime_in_list_mutex(long long start, long long end){
        for(long long i = start; i < end; ++i){
            bool is_prime = true;
            for(long long j = 2; j <= static_cast<long long>(std::sqrt(i)); ++j){
                if(i % j == 0){
                    is_prime = false;
                    break;
                }
            }
            if(is_prime){
                list_add(i);
            }
        }
    }

    void multiple_mutex(){
        std::vector<std::thread> v;
        long long midd = MY_ARRAY/NUM_THREAD;
        for(int i = 0; i < NUM_THREAD ; ++i){
            // std::cout<<midd * i<<"\t"<<midd * (i + 1)<<"\n";
            v.push_back(std::thread(prime_in_list_mutex, midd * i, midd * (i + 1)));
        }

        for(int i = 0; i < NUM_THREAD ; ++i){
            v[i].join();
            // std::cout<<result[i] <<"\t";
        }
        // for(auto i = 2000; i < 2100; ++i){
        //     std::cout<<shared_v[i]<<"\n";
        // }
        std::cout<<"\n\n"<<NUM_THREAD<<"<= # threads\t"<<shared_v.size()<<"\n\n";
    }

    void multiple_BST_leaf(long long start, long long end, std::vector<long long>& parent_v){
        for(long long i = start; i < end; ++i){
            bool is_prime = true;
            // for(long long j = 2; j <= static_cast<long long>(std::sqrt(i)); ++j){
            //     if(i % j == 0){
            //         is_prime = false;
            //         break;
            //     }
            // }
            for(long long j = 2; j * j <= i; ++j){
                if(i % j == 0){
                    is_prime = false;
                    break;
                }
            }
            if(is_prime){
                parent_v.emplace_back(i);
            }
        }
    }

    void multiple_BST_divide(long long start, long long end, std::vector<long long>& parent_v, unsigned depth){
        if(depth == 0){
            multiple_BST_leaf(start, end, parent_v);
        }
        else{
            long long midd = (start + end) / 2;
            std::vector<long long> child1;
            std::vector<long long> child2;

            std::thread worker(multiple_BST_divide, start, midd, std::ref(child1), depth - 1);
            multiple_BST_divide(midd + 1, end, child2, depth - 1);

            worker.join();

            parent_v.insert(parent_v.end(), child1.begin(), child1.end());
            parent_v.insert(parent_v.end(), child2.begin(), child2.end());
        }
    }

    void multiple_BST_root(unsigned depth){
        std::vector<long long> vector;
        multiple_BST_divide(0, MY_ARRAY, vector, depth);
        std::cout<<"\n\n"<<vector.size()<<"\n";
        // for(auto i : vector){
        //     std::cout<<i<<"\n";
        // }
    }
}

namespace cv{
    std::mutex LOCK;
    std::condition_variable CV;
    bool notified = false;
    // int result = 0;

    void reporter(int i){
        std::unique_lock<std::mutex> Ulock(LOCK);
        std::cout<<"Reporter id = "<<std::this_thread::get_id()<<"\t"<<i<<"\n\n";
        if(!notified){
            CV.wait(Ulock);
        }
        std::cout<<"Reporter id = "<<std::this_thread::get_id()<<"\t"<<i<<"\n\n";
    }
    void worker(){
        std::unique_lock<std::mutex> Ulock(LOCK);
        notified = true;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout<<"Work complete\n";
        CV.notify_all();
    }
    void cv(){
        std::vector<std::thread> v;
        for(int i = 0; i < 10; ++i){
            v.push_back(std::thread(reporter, i + 1));
        }
        std::thread work(worker);

        for(int i = 0; i < 10; ++i){
            v[i].join();
        }
        work.join();
    }
}

namespace pool{
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

    void pool(timer my_timer){
        auto task = [&](int i, timer my_timer){
            std::cout<<"Sleeping for "<<i<<" s\n";
            std::this_thread::sleep_for(std::chrono::seconds(i));
            std::cout<<"Finish at time "<<
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::high_resolution_clock::now() - my_timer.before).count()
                    <<" ms\n\n";
        };
        thread_pool my_pool(2);

        for(int i = 0; i < 4; ++i){
        my_pool.enqueue_task(std::bind(task, i+1, my_timer));
        std::cout<<"Enqueue task "<<i+1<<"\n\n";
        }
        
        // my_pool.~thread_pool();
    }

    void prime_in_range(long long start, long long end, long long& result){
        result = end - start;
        for(long long i = start; i < end; ++i){
            for(long long j = 2; j * j <= i; ++j){
                if(i % j == 0){
                    result--;
                    break;
                }
            }
        }
        // return result;
    }

    void test(){
        thread_pool my_pool;
        long long array[SUB_TASK];

        long long midd = MY_ARRAY/SUB_TASK;
        for(size_t i = 0; i <  SUB_TASK; ++i){
            my_pool.enqueue_task(
                std::bind(
                    prime_in_range, midd * i, midd * (i + 1), std::ref(array[i])
                ));
        }
        // while (my_pool.is_working()){
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        my_pool.until_finish();
        
        // my_timer.end("work finish");

        long long sum = 0;
        for(size_t i = 0; i < SUB_TASK; ++i){
            sum = sum + array[i];
        }

        std::cout<<"\n"<<"SubTask = "<<SUB_TASK<<"\tNum Thread = "
                <<NUM_THREAD<<"\n"<<sum<<"\n";
    }
}

template <typename F, typename... Args>
void measure(const char* text, F&& function, Args&&... argument){
    timer my_timer;
    my_timer.start();

    std::forward<F>(function)(std::forward<Args>(argument)...);

    my_timer.end(text);
}

int main(void){
    // measure("\t\tSingle thread", Independent::single);
    // measure("\t\tMannal threading", Independent::multiple);
    // measure("\t\tOMP threading", Independent::multiple_omp);

    // measure("\t\tSingle thread", Dependent::single);
    // measure( "\t\tMutex base", Dependent::multiple_mutex);
    // measure("\t\tBST reduction", Dependent::multiple_BST_root, 8);

    measure("\t\tThread pool", pool::test);
    measure("\t\tOMP threading",Independent::multiple_omp);
    measure("\t\tMannal threading", Independent::multiple);
}