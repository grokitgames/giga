
#include <giga-engine.h>

void TaskPool::Push(Task* task) {
    m_tasks.push_back(task);
}

Task* TaskPool::Pop() {
    Task* task = *m_tasks.begin();
    m_tasks.pop_back();
    return(task);
}

bool TaskPool::HasTasks() {
    return(m_tasks.size() > 0);
}
