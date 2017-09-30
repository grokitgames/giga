
#include <giga-engine.h>

ThreadPool::ThreadPool() {
	m_executing = false;
	m_taskPool = 0;
}

void ThreadPool::AddThread(TaskThread* thread) {
	m_threads.push_back(thread);
}

void ThreadPool::Execute(TaskPool* pool) {
	m_taskPool = pool;

	// If we haven't created a thread pool, process on the main thread
	if (m_threads.size() == 0) {
		Task* task = GetNextTask();
		while (task) {
			task->Execute(0);
			task = GetNextTask();
		}

		return;
	}

	// Otherwise, process on our threads
	while (m_executing) {
		Timer::Sleep(1);
	}
}

Task* ThreadPool::GetNextTask() {
	m_mutex.lock();

	if (m_executing && m_taskPool) {
		if (m_taskPool->HasTasks()) {
			Task* task = m_taskPool->Pop();
			m_mutex.unlock();
			return(task);
		}
		else {
			m_executing = false;
		}
	}

	m_mutex.unlock();
	return(0);
}

void ThreadPool::AddTask(Task* task) {
	m_mutex.lock();
	m_taskPool->Push(task);
	m_mutex.unlock();
}