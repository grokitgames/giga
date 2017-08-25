
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
        for(size_t i = 0; i < m_objects.size(); i++) {
            delete m_objects[i];
            m_objects[i] = 0;
        }
    }
    
    void AddObject(T* object) {
        for(size_t i = 0; i < m_objects.size(); i++) {
            if(m_objects[i] == object) {
                return;
            }
        }
        
        for(size_t i = 0; i < m_objects.size(); i++) {
            if(m_objects[i] == 0) {
                m_objects[i] = object;
                return;
            }
        }
        
        m_objects.push_back(object);
    }
    
    void RemoveObject(T* object) {
        for(size_t i = 0; i < m_objects.size(); i++) {
            if(m_objects[i] == object) {
                m_objects[i] = 0;
                return;
            }
        }
    }
    
    void Clear() {
        for(size_t i = 0; i < m_objects.size(); i++) {
            delete m_objects[i];
            m_objects[i] = 0;
        }
        
        m_objects.clear();
    }
    
    std::vector<T*>& GetList() { return m_objects; }
    
protected:
    std::vector<T*> m_objects;
};

#endif
