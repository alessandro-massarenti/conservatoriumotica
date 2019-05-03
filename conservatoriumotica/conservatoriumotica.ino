int l = 0;  //Current level of the tank
int M = 80; //Maximum level of the tank
int m = 20; //Minimum level of the tank

int discharge = 2; //Discharge valve control pin
int charge = 3;    //Charge valve control pin
int watering = 4;

void setup()
{
    // setup
    pinMode(discharge, OUTPUT);
    pinMode(charge, OUTPUT);
}

void loop()
{
    // Main
    if (l > M)
    {
        digitalWrite(discharge, HIGH);
    }
    else if (l < m)
    {
        digitalWrite(charge, HIGH);
    }
    else
    {
        digitalWrite(discharge, HIGH);
        digitalWrite(charge, HIGH);
    }

    digitalRead(HIGH) ? digitalWrite(watering, HIGH) : digitalWrite(watering, LOW);
}
