import {InputCollector} from './InputCollector.js';
import { WebcastPushConnection }  from 'tiktok-live-connector';

// new input collector
const inputCollector = new InputCollector();

// Username of someone who is currently live
let tiktokUsername = "oldskoldj";

// if the chat message contains any of these words select them and remove the rest
const regEx = /(?:\b|')(a|b|up|down|left|right|start|select|l|r)(?:\b|')/
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
        inputCollector.sendActionWithMostVotes().then(() => {
            inputCollector.clear();
        });
    }, 2000)
}).catch(err => {
    console.error('Failed to connect', err);
})

// Define the events that you want to handle
// In this case we listen to chat messages (comments)
tiktokLiveConnection.on('chat', data => {
    let filter = regEx.exec(data.comment.toLowerCase());
    if(filter){
        console.log("Adding: " + filter[0] + "       " + data.comment)
       inputCollector.increment(filter[0])
    }
})

// And here we receive gifts sent to the streamer
tiktokLiveConnection.on('gift', data => {
    //console.log(`${data.uniqueId} (userId:${data.userId}) sends ${data.giftId}`);
})