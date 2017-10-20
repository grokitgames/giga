
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
        TaskPool* taskPool = thread->m_threadPool->LockTaskPool();
        if(taskPool) {
			Task* task = taskPool->Pop();
			thread->m_threadPool->UnlockTaskPool();
			
            if(task) {
                bool executed = task->Execute(thread);
				if (executed == true) {
					thread->m_threadPool->MarkCompleted(0);
				} else {
                    taskPool->Push(task);
                }
            }
        }
		else {
			thread->m_threadPool->UnlockTaskPool();
		}
        
		if (thread->IsTerminated()) {
			break;
		}
        
        Timer::Sleep(1);
    }

	thread->Shutdown();
}
