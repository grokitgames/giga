
#include <giga-engine.h>

Task::Task() {
    m_obj = 0;
}

void Task::Create(Variant *obj, TaskCallbackFn func) {
    m_obj = obj;
    m_func = func;
}

void Task::Execute() {
    m_func(m_obj, m_args.size(), m_args.data());
}

void Task::AddArgument(Variant* arg) {
    m_args.push_back(arg);
}
