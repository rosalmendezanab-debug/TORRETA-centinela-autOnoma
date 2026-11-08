#include <Servo.h>
int servoPin = 9;
Servo myServo;

#include "Adafruit_VL53L0X.h"
#include  <wire.h>;
Adafruit_VL53L0X laser = Adafruit_VL53L0X();

 int ledPin = 8;          
 int distanciaUmbral = 300; // Distancia límite en milímetros (30 cm) para la alerta
int buzzer = 2;

void setup() {
  // put your setup code here, to run once:
myServo.attach(servoPin);
Serial.begin(115200);
pinMode(ledPin, OUTPUT);    
pinMode (buzzer, OUTPUT);
 //Esperar a que abra el monitor serial
  while (!Serial) {
    delay(1);}

if (!laser.begin()) {
    Serial.println("¡Error! No se encuentra el sensor láser.");
    while (1); // Se detiene si falla
  }
  Serial.println("¡Torreta lista para el barrido!");

}

void loop() {
// 1. Barrido de 0 a 180 grados (de izquierda a derecha)
//El ciclo for de ida (pos = 0; pos <= 180; pos += 5) Le dice al servo que empiece en 0 grados
// y vaya avanzando de 5 en 5 grados hasta llegar a 180°.
  for (int pos = 0; pos <= 180; pos += 5) {
    myServo.write(pos);          // Mueve el servo al ángulo actual
    delay(30);                   // Pequeña pausa para que el motor alcance a llegar

// Medir con el láser en este ángulo exacto
    VL53L0X_RangingMeasurementData_t measure; //aca creamos la variable para guardar los resultados
    laser.rangingTest(&measure, false); // aca damos la orden de disparo

    if (measure.RangeStatus != 4) //si no da el numero de codigo que arroja cuando da error, entonces...
     {int distancia = measure.RangeMilliMeter; // Guardamos la distancia en una variable limpia

      // aca aplica la regla de decission
      // Si la distancia es menor a 300 mm (30 cm), hay un "intruso"
      if (distancia < 300) {
        digitalWrite(ledPin, HIGH); // se enciende el led rojo
        
        Serial.print("¡OBJETIVO DETECTADO en ");
        Serial.print(pos);
        Serial.print("°! Distancia: ");
        Serial.print(distancia);
        Serial.println(" mm");

        for(int i = 1 ; i<4 ; i = i+1){
        tone(buzzer, 2500, 80); // Pin, Frecuencia en Hz (más alto = más agudo), Duración en ms
    delay(120);
        }

        while(distancia <300) {
          myServo.write(pos); 
           laser.rangingTest(&measure, false);
            distancia = measure.RangeMilliMeter;
        }         
    }
else {
        digitalWrite(ledPin, LOW); // Si está lejos, apagamos el LED
      }
  }


  }

  // 2. Barrido de vuelta de 180 a 0 grados (de derecha a izquierda)
  for (int pos = 180; pos >= 0; pos = pos - 5) {
    myServo.write(pos);
    delay(30);

    VL53L0X_RangingMeasurementData_t measure;
    laser.rangingTest(&measure, false);

    if (measure.RangeStatus != 4) {
      int distancia = measure.RangeMilliMeter;

      //misma regla de decision para el barrido devuelta
      if (distancia < 300) {
        digitalWrite(ledPin, HIGH); // 
        
        Serial.print("¡OBJETIVO DETECTADO en ");
        Serial.print(pos);
        Serial.print("°! Distancia: ");
        Serial.print(distancia);
        Serial.println(" mm");

        
        for(int i = 1 ; i<4 ; i = i+1){
       tone(buzzer, 2500, 80); 
    delay(120);
        }

         while(distancia <300) {
          myServo.write(pos); 
           laser.rangingTest(&measure, false);
            distancia = measure.RangeMilliMeter;
        }         
        }
       else {
        digitalWrite(ledPin, LOW); // Apagamos el LED
      }
    }
  }
  }
