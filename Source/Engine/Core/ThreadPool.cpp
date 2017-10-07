
#include <giga-engine.h>

ThreadPool::ThreadPool() {
	m_executing = false;
    m_taskPool = 0;
}

void ThreadPool::AddThread(TaskThread* thread) {
    thread->m_threadPool = this;
	m_threads.push_back(thread);
}

void ThreadPool::Execute(TaskPool* pool) {
	m_taskPool = pool;
    m_executing = true;

	// Otherwise, process on our threads
	while (m_executing) {
		Timer::Sleep(1);
	}
}

void ThreadPool::SetExecuting(bool executing) {
    m_mutex.lock();
    m_executing = executing;
    m_taskPool = 0;
    m_mutex.unlock();
}

TaskPool* ThreadPool::GetTaskPool() {
    return(m_taskPool);
}