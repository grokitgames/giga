
#include <giga-engine.h>

void GigaObject::LockMutex() {
    m_mutex.lock();
}

void GigaObject::UnlockMutex() {
    m_mutex.unlock();
}
