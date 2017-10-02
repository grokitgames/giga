
#include <giga-engine.h>

TaskThread::TaskThread() {
	threadID = 0;
	m_terminated = false;
    m_threadPool = 0;
}

void TaskThread::Start(int threadID) {
	this->threadID = threadID;
    m_thread = std::thread(Run, this);
}

void TaskThread::Run(TaskThread* thread) {
    TaskSystem* system = GetSystem<TaskSystem>();

	thread->Initialize();
    
    while(true) {
        // Check if there is a task pool
        if(thread->m_threadPool->m_taskPool) {
            TaskPool* taskPool = thread->m_threadPool->m_taskPool;
            
            if(taskPool->HasTasks() == false) {
                thread->m_threadPool->m_executing = false;
                break;
            }
            
            Task* task = taskPool->Pop();
            task->Execute(0);
        }
        
		if (thread->IsTerminated()) {
			break;
		}
        
        Timer::Sleep(1);
    }

	thread->Shutdown();
}
