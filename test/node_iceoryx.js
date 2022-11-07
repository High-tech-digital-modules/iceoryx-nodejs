const iceoryx = require('bindings')('iceoryx.node')
const appArgs = process.argv.slice(2);

iceoryx.setIoxProcessName(appArgs[0]);

const canTopicCallback = (data, data2) => {
    console.log(data.data.toString());
}

if(appArgs[1] === "subscriber") {
    const service = new iceoryx.IceoryxSubscriber(["CanP3comBridge", "listener", "can-topic"], canTopicCallback);
    process.on("exit", () => {
        console.log(service);
    })
} else {
    const publisher = new iceoryx.IceoryxPublisher(["CanP3comBridge", "listener", "can-topic"], canTopicCallback);
    let iteration = 0;
    setInterval(() => {
        const obj = {
            message: "Hello from the nodejs thru iceoryx",
            iteration
        }
        const str = JSON.stringify(obj);
        publisher.getLoanBuffer(str.length, (data) => {
            data.fill(str);
            iteration++;
            publisher.publish(data);
        })
    }, 100);
}

const BlockThread = async () => {
    while(1) {
        await new Promise(resolve => setTimeout(resolve, 5000));
    }   
}

BlockThread();
