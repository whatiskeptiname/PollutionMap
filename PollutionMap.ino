const int dustPin = A5;  // dust sensor - Arduino A5 pin
const int ledPin = 2;    // IRED pin - 2
const int buzzerPin = 3 ; // buzzer pin - 3

float voltsMeasured = 0; // raw voltage data from sensor
float calcVoltage = 0;   // refined voltage data range (0-5)volts
float dustDensity = 0;   // dust density measured with refrence to refined voltage
char data;               // data received form bluetooth

void setup()
{
  Serial.begin(9600); // set baud rate to 9600
  pinMode(ledPin, OUTPUT);
}

void s_read() // function to read data from the dust sensor
{
  digitalWrite(ledPin, LOW); //   power on the LED
  delayMicroseconds(300);

  voltsMeasured = analogRead(dustPin); // read the dust value

  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH); // turn the LED off
  delayMicroseconds(9680);

  if (Serial.available()) // used for debugging, print any data received from bluetooth device via serial com
  {
    data = Serial.read();
    Serial.println(data);
  }
}

void s_calculate() // function to convert the sensor data to AQI unit ug/m3
{
  calcVoltage = voltsMeasured * (5.0 / 1024.0);
  dustDensity = 170 * ( calcVoltage - 2);
}

void s_print() // print the data to serial monitor (only for testing)
{
  Serial.println("GP2Y1010AU0F readings");
  Serial.print("Raw Signal Value = ");
  Serial.println(voltsMeasured);
  Serial.print("Voltage = ");
  Serial.println(calcVoltage);
  Serial.print("Dust Density = ");
  Serial.println(dustDensity); // ug/m3
  Serial.println("");
}

void s_send() // send data to Bluetooth device
{
  // same function is used to send data to Bluetooth and print to serial device
  // as they both lie in same serial com line
  Serial.println(dustDensity);
}

void dangerCheck() // trigger the buzzer for higher value of pollution
{
  if (dustDensity > 180)
  {
    tone(buzzerPin, 1000);
    delay(250);
    tone(buzzerPin, 800);
    delay(250);
    tone(buzzerPin, 1000);
    delay(250);
    tone(buzzerPin, 900);
    delay(250);
    tone(buzzerPin, 1000);
    delay(250);
    tone(buzzerPin, 700);
    delay(250);
    tone(buzzerPin, 1000);
    delay(250);
    tone(buzzerPin, 800);
    delay(250);
    noTone(buzzerPin);
  }
}

void loop()
{
  s_read();
  s_calculate();
  s_send();
  s_print(); // use only for debugging and during sensor calibration
  dangerCheck();
  delay(2000); // wait for 2 seconds before next cycle
}
