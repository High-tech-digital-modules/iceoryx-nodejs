#ifndef ICEORYX_PUBLISHER_H
#define ICEORYX_PUBLISHER_H

#include "iceoryx_hoofs/cxx/optional.hpp"
#include "iceoryx_posh/popo/listener.hpp"
#include "iceoryx_posh/popo/untyped_subscriber.hpp"
#include "iceoryx_posh/popo/untyped_publisher.hpp"
#include "iceoryx_posh/popo/user_trigger.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <napi.h>

class IceoryxPublisher : public Napi::ObjectWrap<IceoryxPublisher>
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    
    IceoryxPublisher(const Napi::CallbackInfo& info);
    
    ~IceoryxPublisher();
    
    Napi::Value GetLoanBuffer(const Napi::CallbackInfo& info);
    
    void PublishData(const Napi::CallbackInfo& info);
    
  private:
    iox::popo::UntypedPublisher *m_publisher;
    Napi::Buffer<uint8_t> m_loan_buffer;
};

#endif
