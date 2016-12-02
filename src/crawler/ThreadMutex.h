/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2014
 */
#ifndef THREADMUTEX_H
#define THREADMUTEX_H

#include <pthread.h>
#pragma comment(lib,"x86\\pthreadVC2.lib")

const pthread_mutex_t MUTEX_INIT = PTHREAD_MUTEX_INITIALIZER;

class ThreadMutex
{
	private:
		pthread_mutex_t _mutex;
	public:
		ThreadMutex()
		{
			//_mutex = MUTEX_INIT;
			pthread_mutex_init(&_mutex, NULL);
		}
		~ThreadMutex()
		{
			pthread_mutex_destroy(&_mutex);
		}

		int lock()
		{
			return pthread_mutex_lock(&_mutex);
		}
		int unlock()
		{
			return pthread_mutex_unlock(&_mutex);
		}
		int trylock()
		{
			return pthread_mutex_trylock(&_mutex);
		}
};
#endif