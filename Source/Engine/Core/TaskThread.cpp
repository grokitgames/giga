
#include <giga-engine.h>

void TaskThread::Start() {
    m_thread = std::thread(Run);
}

void TaskThread::Run() {
    TaskSystem* system = GetSystem<TaskSystem>();
    
    while(true) {
        // Check if there is a task available
        Task* task = system->GetNextTask();
        if(task) {
            task->Execute();
        }
        
        Timer::Sleep(1);
    }
}
