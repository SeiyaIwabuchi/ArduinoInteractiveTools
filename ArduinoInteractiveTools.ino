enum Mode{
    setPinMode,
    setPinValue,
    readPinValue,
    UnknownCommand
};
void setup(){
    Serial.begin(9600);
    Serial.println("setPinMode [pinNumber] [input/inputPullUp/output]:ピンの入出力方向を設定します。");
    Serial.println("setPinValue [pinNumber] [Value] [analog/digital]:ピンの値を設定します。");
    Serial.println("readPinValue [pinNumber] [analog/digital]:ピンの値を読みます。");
}
void loop(){
    if(Serial.available()){
        String inputStrs[4] = {"","",""};
        int inputStrsIndex = 0;
        char pinNumberText[4];
        char pinValueText[4];
        int pinNumber,pinValue,direction;
        while(Serial.available()){
            char inputChar = Serial.read();
            if(inputChar == ' '){
                inputStrsIndex++;
            }else{
                inputStrs[inputStrsIndex].concat(inputChar);
            }
            if(inputStrsIndex == 4) break;
            delay(2);
        }
        inputStrs[1].toCharArray(pinNumberText,4);
        inputStrs[2].toCharArray(pinValueText,4);
        pinNumber = atoi(pinNumberText);
        pinValue = atoi(pinValueText);
        direction = getPinDirection(inputStrs[2]);
        switch (getMode(inputStrs[0]))
        {
        case setPinMode:
            if(!(0 <= direction && direction <= 2)) Serial.println("入出力方向が間違っています。");
            pinMode(pinNumber,direction);
            break;
        case setPinValue:
            if (0 <= pinValue && pinValue <= 1 && inputStrs[3] == "digital"){
                digitalWrite(pinNumber,pinValue);
            }else if (0 <= pinValue && inputStrs[3] == "analog"){
                analogWrite(pinNumber,pinValue);
            }else{
                digitalWrite(pinNumber,pinValue);
            }
            break;
        case readPinValue:
            if (inputStrs[2] == "digital"){
                Serial.println(digitalRead(pinNumber));
            }else if (inputStrs[2] == "analog"){
                Serial.println(analogRead(pinNumber));
            }else{
                Serial.println(digitalRead(pinNumber));
            }
            break;
        case UnknownCommand:
            Serial.println("不明なコマンドです。" + inputStrs[0]);
            break;
        }
    }
}
Mode getMode(String modeText){
    int setPinModeCnt = 0,setPinValueCnt = 0,readPinValueCnt = 0;
    if(modeText == "setPinMode") return setPinMode;
    else if(modeText == "setPinValue") return setPinValue;
    else if (modeText == "readPinValue") return readPinValue;
    else return UnknownCommand;
}
int getPinDirection(String directionText){
    if (directionText == "input") return INPUT;
    else if(directionText == "inputPullUp") return INPUT_PULLUP;
    else if(directionText == "output") return OUTPUT;
    else return -1;
}