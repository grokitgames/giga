
#include <giga-engine.h>

TaskSystem::TaskSystem() {
	m_threadPool = 0;
}

void TaskSystem::Initialize(int threads) {
	for (int i = 0; i < threads; i++) {
		TaskThread* thread = new TaskThread();
		thread->Start(i);

		m_threadPool->AddThread(thread);
	}
}

void TaskSystem::Execute(TaskPool* taskPool, ThreadPool* threadPool) {
	if (threadPool == 0) {
		m_threadPool->Execute(taskPool);
		return;
	}

	threadPool->Execute(taskPool);
}
