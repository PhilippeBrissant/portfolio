import serial
import pyrebase
import base64
import picamera
from time import sleep

def capturarImagem():
    camera = picamera.PiCamera()
    try:
        camera.start_preview()
        sleep(1)
        camera.capture('image_test.jpg')
        camera.stop_preview()
        pass
    finally:
        camera.close()

def convertToBase64():
    with open('image_test.jpg', 'rb') as image_file:
        encoded = base64.b64encode(image_file.read())
        return encoded

def inicializaFirebase():
    global db

    credenciais = {
		'apiKey': "AIzaSyCn4dkLFiPdy112uMwq4JctA1jZjfQgDCI",
		'authDomain': "greenhousev2-786f6.firebaseapp.com",
		'databaseURL': "https://greenhousev2-786f6-default-rtdb.firebaseio.com",
		'projectId': "greenhousev2-786f6",
		'storageBucket': "greenhousev2-786f6.appspot.com",
		'messagingSenderId': "797098832447",
		'appId': "1:797098832447:web:bcc15d66e850d0e535fe8a"
    }

    print('inicializando firebase')
    firebase = pyrebase.initialize_app(credenciais)

    db = firebase.database()

def inicializaSerialArduino():
    global comm
    print('abrindo conexao com o arduino')
    comm = serial.Serial('/dev/ttyUSB0', 9600)
    sleep(3)
    print('conexao aberta')

def getDados():
    # 1|11:52:39   |0|0|91.08|1|36|1
    comm.flushInput()
    print('enviando comando')
    comm.write(bytes('W'))
    print('comando enviado')
    return comm.readline()

def enviaDados():
    resp = getDados().split('|')
    print(resp)

    luz = {'luz': int(resp[0])}
    db.child('realtime').update(luz)

    cronometro_luz = {'cronometro_luz': resp[1]}
    db.child('realtime').update(cronometro_luz)

    umidade_solo = {'umidade_solo': int(resp[2])}
    db.child('realtime').update(umidade_solo)

    bomba = {'bomba': int(resp[3])}
    db.child('realtime').update(bomba)

    temperatura = {'temperatura': float(resp[4])}
    db.child('realtime').update(temperatura)

    ventilador = {'ventilador': int(resp[5])}
    db.child('realtime').update(ventilador)

    umidade_ar = {'umidade_ar': float(resp[6])}
    db.child('realtime').update(umidade_ar)
    
    if(resp[0] == '1'):
        capturarImagem()
        data = {'image': convertToBase64()}
        db.child('realtime').update(data)
    

inicializaFirebase()
inicializaSerialArduino()
while(True):
	try:
		enviaDados()
	except:
		inicializaFirebase()
		inicializaSerialArduino()		
	
