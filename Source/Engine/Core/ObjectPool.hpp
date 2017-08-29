
#ifndef objectpool_hpp
#define objectpool_hpp

/**
 * Abstract pool/vector of objects that does de-duplication
 */
template<class T>
class GIGA_API ObjectPool {
public:
    ObjectPool() = default;
    ~ObjectPool() {
		typename std::list<T*>::reverse_iterator i = m_objects.rbegin();
		for (i; i != m_objects.rend(); i++) {
            delete (*i);
        }
    }
    
    void AddObject(T* object) {
        m_objects.push_back(object);
    }
    
    void RemoveObject(T* object) {
		typename std::list<T*>::iterator i = m_objects.begin();
		for (i; i != m_objects.end(); i++) {
			if ((*i) == object) {
				m_objects.erase(i);
				return;
			}
		}
    }
    
    void Clear() {
		typename std::list<T*>::reverse_iterator i = m_objects.rbegin();
		for (i; i != m_objects.rend(); i++) {
			delete (*i);
		}
        
        m_objects.clear();
    }
    
    std::list<T*>& GetList() { return m_objects; }
    
protected:
    std::list<T*> m_objects;
};

#endif
