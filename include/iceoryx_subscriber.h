#ifndef ICEORYX_SUBSCRIBER_H
#define ICEORYX_SUBSCRIBER_H

#include "iceoryx_hoofs/cxx/optional.hpp"
#include "iceoryx_posh/popo/listener.hpp"
#include "iceoryx_posh/popo/untyped_subscriber.hpp"
#include "iceoryx_posh/popo/untyped_publisher.hpp"
#include "iceoryx_posh/popo/user_trigger.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <linux/can.h>
#include <napi.h>

class IceoryxSubscriber : public Napi::ObjectWrap<IceoryxSubscriber>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    
    IceoryxSubscriber(const Napi::CallbackInfo& info);
    
    ~IceoryxSubscriber();
    Napi::ThreadSafeFunction m_subscriber_callback;

  private:
    static void onSampleReceivedCallback(iox::popo::UntypedSubscriber* subscriber, IceoryxSubscriber* self);
    iox::popo::UntypedSubscriber *m_subscriber;
    iox::popo::Listener *m_listener;
};

#endif
