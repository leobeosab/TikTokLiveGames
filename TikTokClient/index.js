import {InputCollector} from './InputCollector.js';
import { WebcastPushConnection }  from 'tiktok-live-connector';
import { stdin } from 'process'

// new input collector
const inputCollector = new InputCollector();

// Username of someone who is currently live
let tiktokUsername = "convictcantaloupe";

// if the chat message contains any of these words select them and remove the rest
const regEx = /.*((a|b|up|down|left|right|start|select|l|r)(?: )?\d?)+/g
// Create a new wrapper object and pass the username
let tiktokLiveConnection = new WebcastPushConnection(tiktokUsername,{
    processInitialData: false,
    enableExtendedGiftInfo: true,
    enableWebsocketUpgrade: true,
    requestPollingIntervalMs: 500,
    clientParams: {
    "app_language": "en-US",
        "device_platform": "web"
    },
    requestHeaders: {
        "headerName": "headerValue"
    },
    websocketHeaders: {
        "headerName": "headerValue"
    },
    requestOptions: {
        timeout: 10000
    },
    websocketOptions: {
        timeout: 10000
    }
});

// Connect to the chat (await can be used as well)
tiktokLiveConnection.connect().then(state => {
    console.info(`Connected to roomId ${state.roomId}`);
    setInterval(() => {
        inputCollector.sendLatestQueueAction()
    }, 1250)
}).catch(err => {
    console.error('Failed to connect', err);
})

stdin.on("data", data => {
    try {
        const s = data.toString();
        let filter = regEx.exec(s.toLowerCase());
        if(filter){
            console.log("Adding: " + filter[0] + "       " + data.comment)
            console.log(filter)

            for (let i = 1; i < filter.length-1; i++) {
                if (i > 3) {
                    break;
                }
                console.log(i)
                let input = filter[i];
                let inputArr = input.split(" ");

                if (inputArr.length === 1 ) {
                    inputCollector.increment(inputArr[0]);
                } else {
                    for (let x = 0; x < parseInt(inputArr[1]); x++) {
                        inputCollector.increment(inputArr[0]);
                    }
                }

            }
        }
    } catch (e) {
        console.error(e)
    }
})

// Define the events that you want to handle
// In this case we listen to chat messages (comments)
tiktokLiveConnection.on('chat', data => {
  try {
        let filter = regEx.exec(data.comment.toLowerCase());
        if(filter){
            console.log("Adding: " + filter[0] + "       " + data.comment)

            for (let i = 1; i < filter.length-1; i++) {
                console.log(i)
                let input = filter[i];
                let inputArr = input.split(" ");

                if (inputArr.length === 1|| inputArr[0] === "start") {
                    inputCollector.increment(inputArr[0]);
                } else {
                    for (let x = 0; x < parseInt(inputArr[1]); x++) {
                        inputCollector.increment(inputArr[0]);
                    }
                }

            }
        }
  } catch {
        console.error(e)
    }
})

// And here we receive gifts sent to the streamer
tiktokLiveConnection.on('gift', data => {
    //console.log(`${data.uniqueId} (userId:${data.userId}) sends ${data.giftId}`);
})
