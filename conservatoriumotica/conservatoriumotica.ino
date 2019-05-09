// Valve control pins
const int EU_valve = 4; //Emergency unload valve control pin
const int L_valve = 3;  //load valve control pin
const int U_valve = 2;  //Watering unload valve

// Sensor pins
const int TL_sensor = A1; //Tank level
const int humidity = A0;  // Analog input pin that senses Vout

struct nodo
{
    int data;
    nodo *next;
    nodo(int a = 0, nodo *b = 0)
    {
        data = a;
        next = b;
    }
};

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

// Funzioni Ausiliarie
nodo *avg_list; // Inizio lista della media se non fosse globale verrebbe resettata ogni volta
void cancella_nodo(nodo *&a, int max, int i = 0)
{
    if (a)
        cancella_nodo(a->next, i + 1);
    if (i > max)
        delete a;
    if (i == max - 1)
        a->next = NULL;
}

int rollingAvg(int input)
{
    if (avg_list)
        avg_list = new nodo(input, 0);
    else
    {
        avg_list = new nodo(input, avg_list);
    }
    nodo *avg_scorri = avg_list;
    cancella_nodo(avg_list, 10);
    int i = 0, somma = 0;
    while (!avg_scorri)
    {
        somma = somma + avg_scorri->data;
        avg_scorri = avg_scorri->next;
        i++;
    }
    return somma / i;
}

void Tankroutine(bool &L_apri, bool &EU_apri)
{

    // Tank life parameters
    int M = 87; //Maximum water level of the tank
    int m = 20; //Minimum water level of the tank
    int l;      //Current water level of the tank
    int x = 40; // Median minimum level
    int z = 70; // Median maximum level

    //Auxiliary variables
    int l1;

    //Tank
    l = map(analogRead(TL_sensor), 0, 1023, 0, 100); //mappa i valori letti dal sensore legandoli a valori percentuali
    //l = rollingAvg(l);
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
}

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

    while (true)
    {
        unsigned long timestamp = millis();

        Tankroutine(L_apri, EU_apri);

        //Moisture reading.
        if ((timestamp - t1) >= 2000)
        {
            sensorValue = analogRead(humidity); // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
            //sensorValue = rollingAvg(sensorValue);
            Vout = (Vin * sensorValue) / 1023;   // Convert Vout to volts
            R = Rref * (1 / ((Vin / Vout) - 1)); // Formula to calculate tested resistor's value
            Serial.print("R: ");
            Serial.println(R); // Give calculated resistance in Serial Monitor
            if (R > 50000)
                low_moisture = true;
            t1 = millis();
        }
        if (low_moisture)
        {
            digitalWrite(U_valve, LOW);
            delay(1500);
            digitalWrite(U_valve, HIGH);
            low_moisture = false;
        }
        delay(1); // Clock
    }
    Serial.println("Done");
}
