
#include <giga-engine.h>

TaskPool::~TaskPool() {
	for (size_t i = 0; i < m_tasks.size(); i++) {
		delete m_tasks[i];
	}
}

void TaskPool::Push(Task* task) {
	m_tasks.insert(m_tasks.begin(), task);
}

Task* TaskPool::Pop() {
	if (m_tasks.size() == 0) {
		return(0);
	}

    Task* task = *(m_tasks.end() - 1);
    m_tasks.pop_back();
    return(task);
}

bool TaskPool::HasTasks() {
    return(m_tasks.size() > 0);
}
