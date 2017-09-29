
#include <giga-engine.h>

TaskThread::TaskThread() {
	threadID = 0;
}

void TaskThread::Start(int threadID) {
	this->threadID = threadID;
    m_thread = std::thread(Run, this);
}

void TaskThread::Run(TaskThread* thread) {
    TaskSystem* system = GetSystem<TaskSystem>();

	// Get list of systems
	Application* application = Application::GetInstance();
	application->LockMutex();
	std::vector<System*> systems = application->GetSystems();
	for (size_t i = 0; i < systems.size(); i++) {
		systems[i]->InitializeThread(thread->threadID);
	}
	//application->InitializeThread(thread->threadID);
	application->UnlockMutex();
    
    while(true) {
        // Check if there is a task available
        Task* task = system->GetNextTask();
        if(task) {
            task->Execute(thread->threadID);
        }
        
        Timer::Sleep(1);
    }
}
