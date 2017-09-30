
#include <giga-engine.h>

TaskThread::TaskThread() {
	threadID = 0;
	m_terminated = false;
}

void TaskThread::Start(int threadID) {
	this->threadID = threadID;
    m_thread = std::thread(Run, this);
}

void TaskThread::Run(TaskThread* thread) {
    TaskSystem* system = GetSystem<TaskSystem>();

	thread->Initialize();
    
    while(true) {
        // Check if there is a task available
        Task* task = system->GetNextTask();
        if(task) {
            bool result = task->Execute(thread->threadID);
			if (result == false) {
				system->AddTask(task);
			}
        }

		if (thread->IsTerminated()) {
			break;
		}
        
        Timer::Sleep(1);
    }

	thread->Shutdown();
}
