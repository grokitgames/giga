
#include <giga-engine.h>

ScriptableVariant::ScriptableVariant(v8::Local<v8::Value> value) {
    *this = value;
}

Variant& ScriptableVariant::operator =(v8::Local<v8::Value> rhs) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::String::Utf8Value type(rhs->TypeOf(isolate));
    
    if (rhs->IsInt32() || rhs->IsUint32()) {
        Variant((int)rhs->NumberValue());
    }
    
    if (rhs->IsNumber() && rhs->IsInt32() == false && rhs->IsUint32() == false) {
        Variant((float)rhs->NumberValue());
    }
    
    if (rhs->IsString()) {
        v8::String::Utf8Value val(rhs.As<v8::String>());
        Variant(std::string(*val));
    }
    
    if (rhs->IsFunction()) {
        v8::Local<v8::Function> func = rhs.As<v8::Function>();
        v8::String::Utf8Value name(func->GetName());
        Variant(std::string(*name));
        m_type = VAR_FUNCTION;
    }
    
    if (rhs->IsObject() && (rhs->IsFunction() == false)) {
        v8::Local<v8::Object> val = rhs->ToObject();
        void* valptr = val->GetAlignedPointerFromInternalField(0);
        Variant((GigaObject*)valptr);
    }
    
    return(*this);
}

v8::Local<v8::Value> ScriptableVariant::GetValue() {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::Value> ret;
    
    // Catch any errors the script might throw
    v8::TryCatch try_catch(isolate);
    
    if (IsInt()) {
        ret = v8::Number::New(isolate, AsInt());
    }
    if (IsFloat()) {
        ret = v8::Number::New(isolate, AsFloat());
    }
    if (IsString()) {
        ret = v8::String::NewFromUtf8(isolate, AsString().c_str());
    }
    if (IsObject()) {
        ScriptableObject* obj = AsObject<ScriptableObject>();
        ret = obj->GetJSObject();
    }
    if (IsFunction()) {
        v8::Local<v8::Object> globalSpace = isolate->GetCurrentContext()->Global();
        ret = globalSpace->Get(v8::String::NewFromUtf8(isolate, AsString().c_str()));
    }
    
    // Attempt to get an error message (need a better way to do this)
    v8::String::Utf8Value error(try_catch.Exception());
    if (*error) {
        // Bad
        assert(false);
    }
    
    return(handle_scope.Escape(ret));
}
