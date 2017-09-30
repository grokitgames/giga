
#ifndef scriptableobjectimpl_hpp
#define scriptableobjectimpl_hpp

/**
 * The implementation of a ScriptableObjectType definition
 */
class ScriptableObjectImpl {
public:
	ScriptableObjectImpl();
	~ScriptableObjectImpl() = default;

	/**
	 * Create an implementation from an object type definition
	 */
	void Create(ScriptableObjectType* type);

	/**
	* Called from JS to create a new version of our object to pass back
	*/
	static void New(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	* Add this template to a new context
	*/
	void AddToContext(v8::Local<v8::Context> context);

	/**
	* Handle static function callbacks (ie. Time.GetTime())
	*/
	static void HandleStaticFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	* Handle object function callbacks (ie. this.DoSomething())
	*/
	static void HandleObjectFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

	/**
	* Handle object getters (call internal callbacks based on name)
	*/
	static void HandleObjectGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

	/**
	* Handle object setters (call internal callbacks based on name)
	*/
	static void HandleObjectSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

	/**
	* Handle static getter calls (call internal callback based on name)
	*/
	static void HandleStaticGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);

	/**
	* Create a JavaScript object to pass back to V8
	*/
	v8::Local<v8::Value> CreateJSObject();

protected:
	// The scriptable object typ def we are based on
	ScriptableObjectType* m_type;

	// The function template
	v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>> m_functionTemplate;

	// Our constructor to create new objects of this type
	v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> m_constructor;
};

#endif