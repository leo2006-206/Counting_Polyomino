# pragma once
#ifndef CONCURRENT_PARALLEL_HPP
#define CONCURRENT_PARALLEL_HPP

//Int Type
#include <cstdint>

//Threading
#include <thread>
#include <atomic>


//Sleeping
#include <time.h>

// memory_order_acquire: guarantees that subsequent loads are not moved before the current load or any preceding loads.
// memory_order_release: preceding stores are not moved past the current store or any subsequent stores.
// memory_order_acq_rel: combines the two previous guarantees.
// memory_order_consume: potentially weaker form of memory_order_acquire that enforces ordering of the current load before other operations that are data-dependent on it (for instance, when a load of a pointer is marked memory_order_consume, subsequent operations that dereference this pointer won’t be moved before it (yes, even that is not guaranteed on all platforms!).
// memory_order_relaxed: all reorderings are okay.

namespace concurrent_parallel{

	//From 
	class spinlock{
		private:
		std::atomic_uint32_t _flag;

		public:
		spinlock() : _flag(0){}

		void lock(){
			static const timespec ns_sleep = {0, 1};

			for(int counter = 0;
				_flag.load(std::memory_order_relaxed)
				||
				_flag.exchange(1, std::memory_order_acquire);
				++counter){
				
				if(counter > 8){
					nanosleep(&ns_sleep, NULL);
					counter = 0;
				}
			}
		}

		void unlock(){
			_flag.store(0, std::memory_order_release);
		}
	};
}

#endif