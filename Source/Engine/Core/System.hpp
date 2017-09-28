
#ifndef system_hpp
#define system_hpp

class GIGA_API System : public ScriptableObject {
public:
    virtual ~System() = default;
    
    /**
     * Overridable initialization function
     */
    virtual void Initialize() { }

	/**
	 * Thread specific initialization
	 */
	virtual void InitializeThread(int threadID) { }
    
    /**
     * Overridable update function (called once per frame)
     */
    virtual void Update(float delta) { }
    
    /**
     * Overridable shutdown function (called at Application shutdown)
     */
    virtual void Shutdown() { }
    
protected:
    // Private constructor to prevent direct creation
    System() = default;
};

#endif
