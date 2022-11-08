# iceoryx-nodejs
this is bindning of [iceoryx](https://iceoryx.io/) library for nodejs

## Usage

There must be iceoryx installed in your system follow [instructions](https://iceoryx.io/v2.0.2/getting-started/installation/)

```shell
# Those variables are used to build addon, will find later way how to ged rid of this
export ICEORYX_NODEJS_LIB=/usr/local/lib
export ICEORYX_NODEJS_INCLUDE=/usr/local/include/iceoryx/<VERSION>
npm install iceoryx-nodejs
```

## Environment
 - There is only linux environment tested right now
 
## Example
```javascript
const iceoryx = require('iceoryx-nodejs')

// There must be name of iox process
// Second param - if "publisher" then application will push some JSON
const appArgs = process.argv.slice(2);

// Unique name of iox process
iceoryx.setIoxProcessName(appArgs[0]);

// Callback for subscriber
const canTopicCallback = (data, data2) => {
    console.log(data.data.toString());
}

if(appArgs[1] === "subscriber") {
    const service = new iceoryx.IceoryxSubscriber(["Test-app", "Test-group", "topic"], canTopicCallback);
    // Little hack by holding reference like that if not Garbage collector will destroy object
    process.on("exit", () => {
        console.log(service);
    })
} else {
    const publisher = new iceoryx.IceoryxPublisher(["Test-app", "Test-group", "topic"], canTopicCallback);
    let iteration = 0;
    setInterval(() => {
        // Sample JSON data
        const obj = {
            message: "Hello from the nodejs thru iceoryx",
            iteration
        }
        const str = JSON.stringify(obj);
        publisher.loan(str.length)
            .then((data) => {
                // We must fill data into received buffer
                data.fill(str);
                iteration++;
                // This can't be called without data received by function getLoanBuffer
                // There is buffer which has memory address from iox shared memory
                // Multiple call of loan without pusblish will lead to not getting buffer
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
```

### Running example
```shell
# first console (of course iox-roudi must running)
node iceoryx.js process_sub subscriber

# second console
node iceoryx.js process_pub publisher
```
