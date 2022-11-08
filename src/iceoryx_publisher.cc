
#include "iceoryx_publisher.h"

Napi::Object IceoryxPublisher::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env,
                  "IceoryxPublisher",
                  {
                      InstanceMethod("loan", &IceoryxPublisher::GetLoanBuffer),
                      InstanceMethod("publish", &IceoryxPublisher::PublishData)
                  });
      

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

 
  exports.Set("IceoryxPublisher", func);
  
  return exports;
}

IceoryxPublisher::IceoryxPublisher(const Napi::CallbackInfo& info) : Napi::ObjectWrap<IceoryxPublisher>(info)
{
    Napi::Env env = info.Env();
    
    int length = info.Length();
    
    if (length <= 0 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Wrong paramemters").ThrowAsJavaScriptException();
        return;
    }
    
    Napi::Array lIoxConfStrings = info[0].As<Napi::Array>();
    
    m_publisher = new iox::popo::UntypedPublisher({
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)0).ToString()},
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)1).ToString()},
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)2).ToString()}
    });
}

Napi::Value IceoryxPublisher::GetLoanBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int length = info.Length();
    
    Napi::Promise::Deferred lDeferred = Napi::Promise::Deferred::New(env);
    
    if (length <= 0 || !info[0].IsNumber()) {
        lDeferred.Reject(Napi::String::New(info.Env(), "Wrong paramemters"));
        return lDeferred.Promise();
    }
    
    double lSize = info[0].As<Napi::Number>().DoubleValue();
    m_publisher->loan(lSize)
        .and_then([&](auto& userPayload) {
            Napi::Buffer<uint8_t> lData = Napi::Buffer<uint8_t>::New(env, (uint8_t*)userPayload, lSize);
            lDeferred.Resolve(lData);
        })
        .or_else([&](auto& error) {
            lDeferred.Reject(Napi::String::New(info.Env(), "Unable to get loan"));
        });
    return lDeferred.Promise();
    
}

void IceoryxPublisher::PublishData(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int length = info.Length();
    
    if (length <= 0 || !info[0].IsBuffer()) {
        Napi::TypeError::New(env, "Wrong paramemters must be buffer").ThrowAsJavaScriptException();
        return;
    }
    
    Napi::Buffer<uint8_t> lData = info[0].As<Napi::Buffer<uint8_t>>();
    m_publisher->publish(lData.Data());
    // Important to release it here otherwise with same iceoryx pointer is crashing
    lData.ArrayBuffer().Detach();
    
}

IceoryxPublisher::~IceoryxPublisher() {
    delete m_publisher;
}
