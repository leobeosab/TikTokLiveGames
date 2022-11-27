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
        console.log(this.inputObject)
    }
    increment(input){
        this.inputObject[input] += 1;
        console.log(this.inputObject);
    }

}