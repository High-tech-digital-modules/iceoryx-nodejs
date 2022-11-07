#include <napi.h>
#include "iceoryx_subscriber.h"
#include "iceoryx_publisher.h"

void SetIoxProcessName(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int length = info.Length();

    if (length <= 0 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return;
    }

    std::string lIoxProcessName = info[0].ToString();
    iox::runtime::PoshRuntime::initRuntime(iox::capro::IdString_t{iox::cxx::TruncateToCapacity, lIoxProcessName});
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("setIoxProcessName", Napi::Function::New(env, SetIoxProcessName)); 
  return IceoryxSubscriber::Init(env, IceoryxPublisher::Init(env, exports));
}

NODE_API_MODULE(addon, InitAll)
