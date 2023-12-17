import axios from 'axios'

export class InputCollector {
    constructor() {
        this.setup();
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
    }

    increment(input) {
        if (["north", "south", "east", "west"].indexOf(input) === -1) {
            switch(input) {
                case "up":
                    this.inputObject.north++
                    break
                case "right":
                    this.inputObject.east++
                    break
                case "down":
                    this.inputObject.south++
                    break
                case "left":
                    this.inputObject.west++
                    break
                case "a":
                    this.inputObject.a++
                    break
                case "b":
                    this.inputObject.b++
                    break
                case "start":
                    this.inputObject.start++
                    break
                case "select":
                    this.inputObject.select++
                    break
                case "l":
                    this.inputObject.lb++
                    break
                case "r":
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
           console.log("Sending: " + current);
           if (["north", "south", "east", "west"].indexOf(current) === -1) {
               await axios.post(`http://192.168.1.234/button/${current}`);
           } else {
               await axios.post(`http://192.168.1.234/dpad/${current}`);
           }
       } else {
       }
    }

    clear(){
        this.setup();
    }
}
