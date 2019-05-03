// Tank life parameters
int M = 80; //Maximum water level of the tank
int m = 20; //Minimum water level of the tank
int l = 0;  //Current water level of the tank

// Greenhouse life parameters
int h = 0; //Current humidity level in greenhouse

// Valve control pins
int EU_valve = 2; //Emergency unload valve control pin
int L_valve = 3;  //load valve control pin
int U_valve = 4;  //Watering unload valve

// Sensor pins
int TL_sensor = A0;
int H_sensor = A1;

// Auxiliary
int l1;
int h1;
bool watering = false;

void setup()
{
    // setup
    pinMode(EU_valve, OUTPUT);
    pinMode(L_valve, OUTPUT);
}

void loop()
{
    //Tank
    l = analogRead(TL_sensor);
    if (l != l1)
    {
        (l > M) ? digitalWrite(EU_valve, HIGH) : digitalWrite(EU_valve, LOW);
        (l < m) ? digitalWrite(L_valve, HIGH) : digitalWrite(L_valve, LOW);
    }
    l1 = l;

    h = analogRead(H_sensor);
    if (h != h1)
    {
        
    }

    watering ? digitalWrite(U_valve, HIGH) : digitalWrite(U_valve, LOW);
}
