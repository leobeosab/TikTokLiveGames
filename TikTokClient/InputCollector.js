import axios from 'axios'
import { SerialPort } from 'serialport';

export class InputCollector {
    constructor() {
        this.setup();
        this.sp = new SerialPort({ path: '/dev/cu.usbserial-2110', baudRate: 9600});
    }

    setup(){
        this.inputObject = {
            "start": 0,
            "select": 0,
            "a": 0,
            "b": 0,
            "north": 0,
            "east": 0,
            "south": 0,
            "west": 0,
            "lb": 0,
            "rb": 0,
        }

        this.queue = [];
    }

    increment(input) {
        if (["north", "south", "east", "west"].indexOf(input) === -1) {
            switch(input) {
                case "up":
                    this.queue.push("north");
                    this.inputObject.north++
                    break
                case "right":
                    this.queue.push("east");
                    this.inputObject.east++
                    break
                case "down":
                    this.queue.push("south");
                    this.inputObject.south++
                    break
                case "left":
                    this.queue.push("west");
                    this.inputObject.west++
                    break
                case "a":
                    this.queue.push("a");
                    this.inputObject.a++
                    break
                case "b":
                    this.queue.push("b");
                    this.inputObject.b++
                    break
                case "start":
                    this.queue.push("start");
                    this.inputObject.start++
                    break
                case "select":
                    this.queue.push("select");
                    this.inputObject.select++
                    break
                case "l":
                    this.queue.push("l");
                    this.inputObject.lb++
                    break
                case "r":
                    this.queue.push("r");
                    this.inputObject.rb++
                    break
            }
        } else {
            this.inputObject[input]++
        }
    }


    async sendActionWithMostVotes(){
        //start with the first key of the object
        let current = Object.keys(this.inputObject)[0];

        //loop through keys and compare values
        for(const key of Object.keys(this.inputObject)){
            //if value is higher set the current key
           if(this.inputObject[key] > this.inputObject[current]){
               current = key;
           }
       }

       if(this.inputObject[current] > 0){
           this.clear()
           console.log("Sending: " + current);
           if (["north", "south", "east", "west"].indexOf(current) === -1) {
               this.sp.write(`/button/${current}`);
           } else {
               this.sp.write(`/dpad/${current}`);
           }
       } else {
       }
    }

    async sendLatestQueueAction() {
        if (this.queue.length > 0) {
            let input = this.queue.shift();
            console.log(input)
            if (["north", "south", "east", "west"].indexOf(input) === -1) {
                this.sp.write(`/button/${input}`);
            } else {
                this.sp.write(`/dpad/${input}`);
            }
        }
    }

    clear(){
        this.setup();
    }
}
