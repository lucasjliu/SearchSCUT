/**
 * created by lucaslau ( jiahuan.liu@outlook.com )
 * date 13/5/2014
 */
#include "ThreadMutex.h"

const pthread_mutex_t MUTEX_INIT = PTHREAD_MUTEX_INITIALIZER;

ThreadMutex::ThreadMutex()
{
	//m_mutex = MUTEX_INIT;
	pthread_mutex_init(&m_mutex, NULL);
}

ThreadMutex::~ThreadMutex()
{
	pthread_mutex_destroy(&m_mutex);
}

int ThreadMutex::lock()
{
	return pthread_mutex_lock(&m_mutex);
}

int ThreadMutex::unlock()
{
	return pthread_mutex_unlock(&m_mutex);
}

int ThreadMutex::trylock()
{
	return pthread_mutex_trylock(&m_mutex);
}
