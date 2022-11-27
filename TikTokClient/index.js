import {Inputs} from './Inputs.js';
import { WebcastPushConnection }  from 'tiktok-live-connector';

const inputs = new Inputs(['up','down','ls','rs','left','right','start','select']);

// Username of someone who is currently live
let tiktokUsername = "unstoutable";

const regEx = /(?:\b|')(ls|rs|up|down|left|right|start|select)(?:\b|')/
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
}).catch(err => {
    console.error('Failed to connect', err);
})

// Define the events that you want to handle
// In this case we listen to chat messages (comments)
tiktokLiveConnection.on('chat', data => {
    let filter = regEx.exec(data.comment.toLowerCase());
    if(filter){
       inputs.increment(filter[0])
    }
})

// And here we receive gifts sent to the streamer
tiktokLiveConnection.on('gift', data => {
    console.log(`${data.uniqueId} (userId:${data.userId}) sends ${data.giftId}`);
})