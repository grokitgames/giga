
#include <giga-engine.h>

TaskPool::~TaskPool() {
	for (size_t i = 0; i < m_tasks.size(); i++) {
		delete m_tasks[i];
	}
}

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
