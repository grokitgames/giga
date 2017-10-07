
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
        TaskPool* taskPool = thread->m_threadPool->GetTaskPool();
        if(taskPool) {
            if(taskPool->HasTasks() == false) {
                thread->m_threadPool->SetExecuting(false);
            }
            else {
                Task* task = taskPool->Pop();
                bool executed = task->Execute(thread);
                if(executed == false) {
                    taskPool->Push(task);
                }
            }
        }
        
		if (thread->IsTerminated()) {
			break;
		}
        
        Timer::Sleep(1);
    }

	thread->Shutdown();
}
