{
  "targets": [
    {
      "target_name": "iceoryx",
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "sources": [ "src/iceoryx.cc", "src/iceoryx_subscriber.cc", "src/iceoryx_publisher.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!(echo $ICEORYX_NODEJS_INCLUDE)",
        "include"
      ],
      "libraries": [
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_binding_c.a",  
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_posh.a",
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_posh_gateway.a",
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_hoofs.a",
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_platform.a",
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_posh_config.a",
        "<!(echo $ICEORYX_NODEJS_LIB)/libiceoryx_posh_roudi.a",
        "-lrt", "<!(echo $ICEORYX_NODEJS_LIB_SPEC)"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
