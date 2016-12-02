/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 14/5/2014
 */
#ifndef THREADSEMAPHORE_H
#define THREADSEMAPHORE_H

#include <pthread.h>
#include <semaphore.h>
#pragma comment(lib,"x86\\pthreadVC2.lib")

class ThreadSem_t
{
private:
	sem_t _semaphore;
public:
	ThreadSem_t(int value)
	{
		sem_init(&_semaphore, 0, value);
	}
	ThreadSem_t()
	{
		sem_init(&_semaphore, 0, 0);
	}
	~ThreadSem_t()
	{
		sem_destroy(&_semaphore);
	}
	int wait()
	{
		return sem_wait(&_semaphore);
	}
	int signal()
	{
		return sem_post(&_semaphore);
	}

};

#endif