
#include <giga-engine.h>

TaskThread::TaskThread() {
	threadID = 0;
}

void TaskThread::Start() {
    m_thread = std::thread(Run, this);
}

void TaskThread::Run(TaskThread* thread) {
    TaskSystem* system = GetSystem<TaskSystem>();
    
    while(true) {
        // Check if there is a task available
        Task* task = system->GetNextTask();
        if(task) {
            task->Execute(thread->threadID);
        }
        
        Timer::Sleep(1);
    }
}
