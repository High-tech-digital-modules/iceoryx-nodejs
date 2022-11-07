
#include "iceoryx_subscriber.h"

Napi::ThreadSafeFunction threadSafeCallback;

Napi::Object IceoryxSubscriber::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func =
      DefineClass(env, "IceoryxSubscriber", {});

  Napi::FunctionReference* constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  env.SetInstanceData(constructor);

 
  exports.Set("IceoryxSubscriber", func);
  
  return exports;
}

IceoryxSubscriber::IceoryxSubscriber(const Napi::CallbackInfo& info) : Napi::ObjectWrap<IceoryxSubscriber>(info)
{
    Napi::Env env = info.Env();
    
    int length = info.Length();
    
    if (length <= 1 || !info[0].IsArray() || !info[1].IsFunction()) {
        Napi::TypeError::New(env, "Wrong paramemters").ThrowAsJavaScriptException();
        return;
    }
    
    Napi::Function lCallback = info[1].As<Napi::Function>();
    
    Napi::Array lIoxConfStrings = info[0].As<Napi::Array>();
    
    if(lIoxConfStrings.Length() != 3) {
        Napi::TypeError::New(env, "First parameters should be array of string with length = 3").ThrowAsJavaScriptException();
        return;
    }
    
    m_subscriber_callback = Napi::ThreadSafeFunction::New(env, lCallback, "Callback", 0, 1);
    
    m_subscriber = new iox::popo::UntypedSubscriber({
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)0).ToString()},
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)1).ToString()},
        iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxConfStrings.Get((uint32_t)2).ToString()}
    });
    
    m_listener = new iox::popo::Listener();
    m_listener
        ->attachEvent(*m_subscriber,
                        iox::popo::SubscriberEvent::DATA_RECEIVED,
                        iox::popo::createNotificationCallback(onSampleReceivedCallback, *this))
        .or_else([](auto) {
            std::exit(EXIT_FAILURE);
        });
}

IceoryxSubscriber::~IceoryxSubscriber() {
    delete m_subscriber;
    delete m_listener;
}

void IceoryxSubscriber::onSampleReceivedCallback(iox::popo::UntypedSubscriber* subscriber, IceoryxSubscriber* self)
{
    subscriber->take().and_then([&](const void* userPayload) {
        auto lPayloadSize = iox::mepoo::ChunkHeader::fromUserPayload(userPayload)->userPayloadSize();
        auto callback = [userPayload, lPayloadSize, subscriber](Napi::Env env, Napi::Function jsCallback) {
            Napi::Buffer<uint8_t> lData = Napi::Buffer<uint8_t>::New(env, (uint8_t*)userPayload, lPayloadSize, [subscriber](void *h, void *d) {
                    // important for iceoryx
                    subscriber->release(d);
            });
            Napi::Object lFrame = Napi::Object::New(env);
            lFrame.Set("len", lPayloadSize);
            lFrame.Set("data", lData);
            jsCallback.Call({lFrame});
            // Important to release it becase finalizer lambda is called immediatelly
            lData.ArrayBuffer().Detach();
        };
        self->m_subscriber_callback.NonBlockingCall(callback);
    });
}
