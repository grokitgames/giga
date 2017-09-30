
#include <giga-engine.h>

ScriptThread::ScriptThread() {
	m_isolate = 0;
}

ScriptThread::~ScriptThread() {

}

void ScriptThread::Initialize() {
	// Create an isolate
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	m_isolate = v8::Isolate::New(create_params);

	m_isolate->Enter();

	// Get all scriptable object type definitions and initialize in our isolate environment
	ScriptingSystem* scriptingSystem = GetSystem<ScriptingSystem>();

	std::vector<ScriptableObjectType*> types = scriptingSystem->GetRegisteredTypes();
	for (size_t i = 0; i < types.size(); i++) {
		ScriptableObjectImpl* impl = new ScriptableObjectImpl();
		impl->Create(types[i]);

		m_types.push_back(impl);
	}

	m_isolate->Exit();
}

void ScriptThread::Shutdown() {
	if (m_isolate) {
		m_isolate->Dispose();
	}
}