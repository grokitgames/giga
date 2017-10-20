
#include <giga-engine.h>

ThreadPool::ThreadPool() {
	m_completed = 0;
	m_poolSize = 0;
    m_taskPool = 0;
}

void ThreadPool::AddThread(TaskThread* thread) {
    thread->m_threadPool = this;
	m_threads.push_back(thread);
}

void ThreadPool::Execute(TaskPool* pool) {
	m_taskPool = pool;
	m_poolSize = pool->Size();
	m_completed = 0;

	// Otherwise, process on our threads
	while (m_completed < m_poolSize) {
		Timer::Sleep(1);
	}

	m_taskPool = 0;
}

void ThreadPool::MarkCompleted(Task* task) {
    m_mutex.lock();
	m_completed++;
    m_mutex.unlock();
}

TaskPool* ThreadPool::LockTaskPool() {
	m_mutex.lock();
    return(m_taskPool);
}


void ThreadPool::UnlockTaskPool() {
	m_mutex.unlock();
}