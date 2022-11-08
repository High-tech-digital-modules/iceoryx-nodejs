const iceoryx = require('bindings')('iceoryx.node')
const appArgs = process.argv.slice(2);

iceoryx.setIoxProcessName(appArgs[0]);

const canTopicCallback = (data, data2) => {
    console.log(data.data.toString());
}

if(appArgs[1] === "subscriber") {
    const service = new iceoryx.IceoryxSubscriber(["Test-app", "Test-group", "topic"], canTopicCallback);
    process.on("exit", () => {
        console.log(service);
    })
} else {
    const publisher = new iceoryx.IceoryxPublisher(["Test-app", "Test-group", "topic"], canTopicCallback);
    let iteration = 0;
    setInterval(() => {
        const obj = {
            message: "Hello from the nodejs thru iceoryx",
            iteration
        }
        const str = JSON.stringify(obj);
        publisher.loan(str.length)
            .then((data) => {
                data.fill(str);
                iteration++;
                publisher.publish(data);
            })
            .catch(err => console.log(err))
    }, 100);
}

const BlockThread = async () => {
    while(1) {
        await new Promise(resolve => setTimeout(resolve, 5000));
    }   
}

BlockThread();
