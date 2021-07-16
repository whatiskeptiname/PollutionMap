const int dustPin = A5;  // dust sensor - Arduino A5 pin
const int ledPin = 2;    // IRED pin - 2
const int buzzerPin = 3 ; //buzzer pin - 3

float voltsMeasured = 0; //raw voltage data from sensor
float calcVoltage = 0;   //refined voltage data range (0-5)volts
float dustDensity = 0;   //dust density measured with refrence to refined voltage
char data;               //data received form bluetooth

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void s_read()
{
  digitalWrite(ledPin, LOW); //   power on the LED
  delayMicroseconds(300);

  voltsMeasured = analogRead(dustPin); // read the dust value

  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH); // turn the LED off
  delayMicroseconds(9680);

  if (Serial.available())
  {
    data = Serial.read();
    Serial.println(data);
  }
}

void s_calculate()
{
  //measure your 5v and change below
  calcVoltage = voltsMeasured * (5.0 / 1024.0);
  dustDensity = 170 * ( calcVoltage - 2);
}

void s_print()
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

void s_send()
{
  Serial.println(dustDensity);
}

void dangerCheck()
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
  s_print();
  dangerCheck();
  delay(2000);
}
