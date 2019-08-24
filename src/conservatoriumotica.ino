/*
  A MIT licensed progect,
  started by Alessandro Massarenti
*/

/*
  An Arduino based controller to keep a plant alive in the most efficent way possibile
  - Future implementations:
      * data gathering in a mysql db trough serial
      * data processing made by raspberry computer or a cluster computer of some form.
      * machine learning adaptive response to I/O
*/

/*
  Sensor implemented in this script:
    legend:
      + done
      * to be done
    list:
      + air-humidity;
      * soil-humidity;
      + air-temperature;
      * tank-level
      * water-temperature in the tank
      * air pressure
*/

// Import external libraries
#include "DHT.h"
#define DHTPIN 2      // analog reading pin
#define DHTTYPE DHT22 // sensor model

// Dht inizialization
DHT dht(DHTPIN, DHTTYPE);

// Valve control pins
const int EU_valve = 6; //Emergency unload valve control pin
const int L_valve = 5;  //load valve control pin
const int U_valve = 4;  //Watering unload valve

const int soil_sensor_sw = 3;

// Sensor pins
const int TL_sensor = A1; //Tank level
const int soil_sensor = A0;  // Analog input pin that senses Vout

int lst;
int A[10];

// Auxiliary functions
void IO_initialize();
int rollingAvg(int input);
void tankRoutine(bool &L_apri, bool &EU_apri, int &l1, int M = 87, int z = 70, int x = 40, int m = 20);
float check_air_humidity();
float check_air_temp();
float check_soil_humidity();

// setup
void setup()
{
    Serial.begin(9600); // Initialize serial communications at 9600 bps
    IO_initialize();
}

// Main function
void loop()
{
    int sensorValue = 0; // humidity default value
    float Vin = 5;       // Input voltage
    float Vout = 0;      // Vout default value
    float Rref = 47000;  // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
    float R = 0;

    // Greenhouse life parameters
    int h = 0; //Current humidity level in greenhouse
    int h1;
    unsigned long t1 = 0;
    bool low_moisture = false;

    // Auxiliary
    bool L_apri = false;
    bool EU_apri = false;
    int l1 = 0;

    // Main repeating cycle
    while (true)
    {
        unsigned long timestamp = millis();

        //tankRoutine(L_apri, EU_apri, l1);

        float air_humidity = check_air_humidity();
        float air_temp = check_air_temp();
        float soil_humidity = check_soil_humidity();

        Serial.print(soil_humidity);
        Serial.print(" ");
        Serial.print(air_humidity);
        Serial.print(" ");
        Serial.println(air_temp);

        // Clock
        delay(2000);
    }

    // Reboot error
    Serial.println("Qualcosa è andato storto, ora riavvio");
}

// Initialize the I/O of the control board
void IO_initialize()
{

    // Valves becomes initialized
    pinMode(EU_valve, OUTPUT);
    pinMode(L_valve, OUTPUT);
    pinMode(U_valve, OUTPUT);
    digitalWrite(EU_valve, HIGH);
    digitalWrite(L_valve, HIGH);
    digitalWrite(U_valve, HIGH);

    // Temp/humid sensor becomes initialized
    dht.begin();

    //soil humidity sensor switch becomes initialized
    pinMode(soil_sensor_sw, OUTPUT);
}

int rollingAvg(int input)
{
    long long sum = 0;

    A[lst] = input;
    for (int i = 0; i < 10; i++)
        sum = sum + A[i];
    sum = sum / 10;
    lst++;
    if (lst == 10)
        lst = 0;
    return sum;
}

/**
  a tankRoutine
  Tank life parameters
  int M = 87; //Maximum water level of the tank
  int z = 70; // Median maximum level
  int x = 40; // Median minimum level
  int m = 20; //Minimum water level of the tank
*/
void tankRoutine(bool &L_apri, bool &EU_apri, int &l1, int M = 87, int z = 70, int x = 40, int m = 20)
{
    int l = map(analogRead(TL_sensor), 0, 1023, 0, 100); //mappa i valori letti dal sensore legandoli a valori percentuali

    if (l != l1)
    {
        if (!L_apri && l <= m)
            L_apri = true;
        if (L_apri && l > x)
            L_apri = false;
        L_apri ? digitalWrite(L_valve, LOW) : digitalWrite(L_valve, HIGH);

        if (!EU_apri && l >= M)
            EU_apri = true;
        if (EU_apri && l < z)
            EU_apri = false;
        EU_apri ? digitalWrite(EU_valve, LOW) : digitalWrite(EU_valve, HIGH);
    }
    l1 = l;
}

float check_air_humidity()
{
    float h = dht.readHumidity();
    return h;
    // in the future a rolling average should be added for more stability
}

float check_air_temp()
{

    float t = dht.readTemperature();
    return t;
    // in the future a rolling average should be added for more stability
}

float check_soil_humidity(){
    digitalWrite(soil_sensor_sw,HIGH);
    delay(5);
    int x = analogRead(soil_sensor);
    digitalWrite(soil_sensor_sw,LOW);
    return x;
}

// Surplus code

/*
//Moisture reading.
    if ((timestamp - t1) >= 100)
    {
      sensorValue = rollingAvg(analogRead(humidity)); // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
      Vout = (Vin * sensorValue) / 1023;              // Convert Vout to volts
      R = Rref * (1 / ((Vin / Vout) - 1));            // Formula to calculate tested resistor's value
      if (!low_moisture && R > 10000)
        low_moisture = true;
      if (low_moisture && R < 5000)
        low_moisture = false;
      low_moisture ? digitalWrite(U_valve, LOW) : digitalWrite(U_valve, HIGH);
      t1 = millis();
    }
    Serial.print("Level: ");
    Serial.print(l1); // Give level in Serial Monitor
    Serial.print(" ");
    Serial.print("light: ");
    Serial.print(analogRead(2)); // Give level in Serial Monitor
    Serial.print(" ");
    Serial.print("R: ");
    Serial.println(R); // Give calculated resistance in Serial Monitor
*/