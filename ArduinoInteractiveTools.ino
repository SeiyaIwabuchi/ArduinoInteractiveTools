#define MAXIMUM_NUMBER_OF_PINS 19
#if defined(__AVR_ATmega328P__)
    #define MAXIMUM_NUMBER_OF_PINS 19
#elif defined(__AVR_ATmega2560__)
    #define MAXIMUM_NUMBER_OF_PINS 69
#endif
enum Mode{
    setPinMode,
    setPinValue,
    readPinValue,
    UnknownCommand
};
void setup(){
    Serial.begin(9600);
    Serial.print("対話型の操作プログラムです。このボードで使用できるピンは");
    Serial.print(2);
    Serial.print("~");
    Serial.print(MAXIMUM_NUMBER_OF_PINS);
    Serial.println("までです。");
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
            if(!(0 <= direction && direction <= 2)){
                Serial.println("入出力方向が間違っています。");
                Serial.println(0);
                break;
            }
            if (!(2 <= pinNumber && pinNumber <= MAXIMUM_NUMBER_OF_PINS)){
                Serial.println("ピン番号が間違っています。");
                Serial.println(0);
                break;
            }
            pinMode(pinNumber,direction);
            Serial.println(1);
            break;
        case setPinValue:
            if (!(2 <= pinNumber && pinNumber <= MAXIMUM_NUMBER_OF_PINS)){
                Serial.println("ピン番号が間違っています。");
                Serial.println(0);
                break;
            }
            if (0 <= pinValue && pinValue <= 1 && inputStrs[3] == "digital"){
                digitalWrite(pinNumber,pinValue);
            }else if (0 <= pinValue && inputStrs[3] == "analog"){
                analogWrite(pinNumber,pinValue);
            }else{
                digitalWrite(pinNumber,pinValue);
            }
            Serial.println(1);
            break;
        case readPinValue:
            if (!(2 <= pinNumber && pinNumber <= MAXIMUM_NUMBER_OF_PINS)){
                Serial.println("ピン番号が間違っています。");
                Serial.println(0);
                break;
            }
            if (inputStrs[2] == "digital"){
                Serial.println(digitalRead(pinNumber));
            }else if (inputStrs[2] == "analog"){
                Serial.println(analogRead(pinNumber));
            }else{
                Serial.println(digitalRead(pinNumber));
            }
            Serial.println(1);
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