import time
from serial import SerialBase,SerialException,Serial
from threading import Thread
import serial.tools.list_ports

def search_com_port():
    coms = serial.tools.list_ports.comports()
    comlist = []
    for com in coms:
        comlist.append(com.device)
    #print('Connected COM ports: ' + str(comlist))
    use_port = comlist[0]
    #print('Use COM port: ' + use_port)
    return comlist
def reciever(ser:SerialBase):
    while True:
        revStr = ser.readline().decode("utf-8")
        if revStr != "":
            print(revStr,end="")
        time.sleep(0.00001)

enableComs = search_com_port()
print("どのポートを使用しますか？")
for select,com in enumerate(enableComs):
    print(f"{select}:{com}",end="\t")
print()

with Serial(enableComs[int(input())],9600,timeout=1) as ser:
    receiverThread = Thread(target=reciever,args=(ser,))
    receiverThread.daemon = True
    receiverThread.start()
    while True:
        time.sleep(0.001)
        inputStr = input("> ")
        flag=bytes(inputStr,'utf-8') #バイト型に変換
        try:
            if inputStr != "exit":
                ser.write(flag) #送信
            else:
                print("Bye")
                break
        
        #よくわからんが何かシリアル通信で例外が発生したら、メッセージ表示して終了
        except SerialException:
            print("Arduinoとの接続を確認してください。")
            input()