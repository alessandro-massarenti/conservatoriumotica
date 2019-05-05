
int sensorValue = 0; // humidity default value
float Vin = 5;       // Input voltage
float Vout = 0;      // Vout default value
float Rref = 47000;  // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;

// Tank life parameters
int M = 80; //Maximum water level of the tank
int m = 20; //Minimum water level of the tank
int l = 0;  //Current water level of the tank

// Greenhouse life parameters
int h = 0; //Current humidity level in greenhouse

// Valve control pins
int EU_valve = 4; //Emergency unload valve control pin
int L_valve = 3;  //load valve control pin
int U_valve = 2;  //Watering unload valve

// Sensor pins
int TL_sensor = A1;      //Tank level
const int humidity = A0; // Analog input pin that senses Vout

// Auxiliary
int l1;
int h1;
unsigned long t1 = 0;

void setup()
{
    // setup
    Serial.begin(9600); // Initialize serial communications at 9600 bps
    pinMode(EU_valve, OUTPUT);
    pinMode(L_valve, OUTPUT);
    pinMode(U_valve, OUTPUT);
}

void loop()
{
    unsigned long time_now = millis();
    

    //Tank
    l = map(analogRead(TL_sensor), 0, 1023, 0, 100);
    Serial.print("Level: ");
    Serial.println(l); // Give calculated resistance in Serial Monitor
    if (l != l1)
    {
        (l > M) ? digitalWrite(EU_valve, LOW) : digitalWrite(EU_valve, HIGH);
        (l < m) ? digitalWrite(L_valve, LOW) : digitalWrite(L_valve, HIGH);
    }
    l1 = l;

    if ((time_now - t1) >= 10000)
    {
        unsigned long time = millis();
        sensorValue = analogRead(humidity);  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
        Vout = (Vin * sensorValue) / 1023;   // Convert Vout to volts
        R = Rref * (1 / ((Vin / Vout) - 1)); // Formula to calculate tested resistor's value
        Serial.print("R: ");
        Serial.println(R); // Give calculated resistance in Serial Monitor
        if (R > 50000)
        {
            digitalWrite(2, LOW);
            delay(1500);
        }
        digitalWrite(2, HIGH);
        t1 = millis();
    }
    delay(1);
}
