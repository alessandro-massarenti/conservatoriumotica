int l = 0;  //Current level of the tank
int M = 80; //Maximum level of the tank
int m = 20; //Minimum level of the tank

int euv = 2; //Emergency unload valve control pin
int lv = 3;  //load valve control pin
int uv = 4;  //Watering unload valve

bool watering = false;

void setup()
{
    // setup
    pinMode(euv, OUTPUT);
    pinMode(lv, OUTPUT);
}

void loop()
{
    // Main
    (l > M) ? digitalWrite(euv, HIGH) : digitalWrite(euv, LOW);
    (l < m) ? digitalWrite(lv, HIGH) : digitalWrite(lv, LOW);
    watering ? digitalWrite(uv, HIGH) : digitalWrite(uv, LOW);
}
