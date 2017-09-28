
#include <giga-engine.h>

TaskSystem::TaskSystem() {
    m_executing = false;
    m_taskPool = 0;
}

void TaskSystem::Initialize(int threads) {
    for(int i = 0; i < threads; i++) {
        TaskThread* thread = new TaskThread();
        thread->Start();
        
        m_threads.push_back(thread);
    }
}

Task* TaskSystem::GetNextTask() {
    m_mutex.lock();
    
    if(m_executing && m_taskPool) {
        if(m_taskPool->HasTasks()) {
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

void TaskSystem::Execute(TaskPool* pool) {
    m_taskPool = pool;
    m_executing = true;

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
    while(m_executing) {
        Timer::Sleep(1);
    }
}
