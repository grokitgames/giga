
#include <giga-engine.h>

Task::Task() {
    m_obj = 0;
}

Task::~Task() {
	for (size_t i = 0; i < m_args.size(); i++) {
		delete m_args[i];
	}

	delete m_obj;
}

void Task::Create(GigaObject *obj, TaskCallbackFn func) {
    m_obj = new Variant(obj);
    m_func = func;
}

bool Task::Execute(TaskThread* thread) {
    return(m_func(thread, m_obj, m_args.size(), m_args.data()));
}

void Task::AddArgument(Variant* arg) {
    m_args.push_back(arg);
}
