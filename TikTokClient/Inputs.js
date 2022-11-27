export class Inputs{
    constructor(inputArray) {
        this.inputArray = inputArray;
        this.inputObject = {}
        this.setup();
    }

    setup(){
        for(let input of this.inputArray){
            this.inputObject[input] = 0;
        }
    }
    increment(input){
        this.inputObject[input] += 1;
    }
    chooseFavorite(){
        let current = Object.keys(this.inputObject)[0];
        for(const key of Object.keys(this.inputObject)){
           if(this.inputObject[key] > this.inputObject[current]){
               current = key;
           }
       }
       if(this.inputObject[current] > 0){
           return current;``
       }
    }
    clear(){
        this.inputObject = {};
        this.setup();
    }

}