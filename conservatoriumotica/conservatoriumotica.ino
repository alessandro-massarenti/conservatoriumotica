
int sensorValue = 0; // humidity default value
float Vin = 5;       // Input voltage
float Vout = 0;      // Vout default value
float Rref = 47000;  // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;

// Tank life parameters
int M = 87; //Maximum water level of the tank
int m = 20; //Minimum water level of the tank
int l = 50;  //Current water level of the tank
int x = 40; // Median minimum level
int z = 70; // Median maximum level

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
bool L_apri = false;
bool EU_apri = false;
int h1;
unsigned long t1 = 0;

void setup()
{
    // setup
    Serial.begin(9600); // InitialiL_aprie serial communications at 9600 bps
    pinMode(EU_valve, OUTPUT);
    pinMode(L_valve, OUTPUT);
    pinMode(U_valve, OUTPUT);
    digitalWrite(EU_valve, HIGH);
    digitalWrite(L_valve, HIGH);
    digitalWrite(U_valve, HIGH);
}

void loop()
{
    unsigned long time_now = millis();

    //Tank
    l = map(analogRead(TL_sensor), 0, 1023, 0, 100); //mappa i valori letti dal sensore legandoli a valori percentuali
    Serial.print("Level: ");
    Serial.println(l); // Give level in Serial Monitor
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

    if ((time_now - t1) >= 20000)
    {
        sensorValue = analogRead(humidity);  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
        Vout = (Vin * sensorValue) / 1023;   // Convert Vout to volts
        R = Rref * (1 / ((Vin / Vout) - 1)); // Formula to calculate tested resistor's value
        Serial.print("R: ");
        Serial.println(R); // Give calculated resistance in Serial Monitor
        if (R > 50000)
        {
            digitalWrite(U_valve, LOW);
            delay(1500);
        }
        digitalWrite(U_valve, HIGH);
        t1 = millis();
    }
    
    delay(1);
}
