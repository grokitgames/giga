
#include <giga-engine.h>

TaskSystem::TaskSystem() {
	m_threadPool = 0;
}

void TaskSystem::Initialize(int threads) {
	for (int i = 0; i < threads; i++) {
		TaskThread* thread = new TaskThread();
		thread->Start(i);

		m_threadPool->AddThread(thread);
	}
}

void TaskSystem::Execute(TaskPool* taskPool, ThreadPool* threadPool) {
	if (threadPool) {
		threadPool->Execute(taskPool);
		return;
	}
    
    if(m_threadPool) {
        m_threadPool->Execute(taskPool);
        return;
    }

    // If we haven't created a thread pool, process on the main thread
    while(true) {
        if(taskPool->HasTasks() == false) {
            break;
        }
            
        Task* task = taskPool->Pop();
        task->Execute(0);
    }
}
