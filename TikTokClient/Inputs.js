import axios from 'axios'

export class Inputs{
    constructor(inputArray) {
        this.inputArray = inputArray;
        this.inputObject = {}
        this.setup();
    }

    setup(){
        //grab every input and bind them to a key/value pair with a starting value of zero
        for(let input of this.inputArray){
            this.inputObject[input] = 0;
        }
    }
    increment(input){
        this.inputObject[input] += 1;
    }
    async chooseFavorite(){
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
           await axios.post(`/button/${current}`);
           return;``
       }
    }
    clear(){
        this.inputObject = {};
        this.setup();
    }

}