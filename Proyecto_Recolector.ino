/// *****************************************************************************
/// ------------------- Proyecto Recolector - Gloty -----------------------------
/// -----------------------------------------------------------------------------
/// ---------- Desarrollado por: Julian Andres Trujillo Molina ------------------
/// *****************************************************************************


// ------------------------ Librerias necesarias ------------------------
#include "HX711.h"  // Libreria --> Celda de carga
#include <SPI.h>      // incluye libreria bus SPI --> RFID
#include <MFRC522.h>      // incluye libreria especifica para MFRC522 -->RFID
#include <SoftwareSerial.h> // incluye libreria para comunicacion con modulo --> A6 GSM/GPRS

 
long CALIBRACION = 193; // Parámetro para calibrar el peso y el sensor
#define DEBUG_HX711 // Parametro para definir objeto HX711 haciendo uso de la libreria
#define RST_PIN  49      // constante para referenciar pin de reset (RST)
#define SS_PIN  53      // constante para referenciar pin de slave select (SDA)

// -------------------- Pines para los modulos HX711---------------

byte pinData1 = 30; // Resicolector 1
byte pinClk1 = 31;

byte pinData2 = 32; // Resicolector 2
byte pinClk2 = 33;

byte pinData3 = 34; // Resicolector 3
byte pinClk3 = 35;

byte pinData4 = 36; // Resicolector 4
byte pinClk4 = 37;

byte pinData5 = 38; // Resicolector 5
byte pinClk5 = 39;

byte pinData6 = 40; // Resicolector 6
byte pinClk6 = 41;

/// ---------------------------------

// Variables correspondientes al peso de cada Resicolector
int weight1=0;
int weight2=0;
int weight3=0;
int weight4=0;
int weight5=0;
int weight6=0;


// Definicion de Modulos para cada Resicolector
HX711 Resicolector1;
HX711 Resicolector2;
//HX711 Resicolector3;
//HX711 Resicolector4;
//HX711 Resicolector5;
//HX711 Resicolector6;
///-------------------------------------


MFRC522 mfrc522(SS_PIN, RST_PIN); // Crea objeto para el RFID --> (SDA,RST)
SoftwareSerial A6MODULE(2,3); // Crea objeto para el modulo A6 GSM/GPRS // rx tx

//******************* NOTA*********************

// Tener en cuenta que la conexion entre dos componentes por medio de los pines Rx y Tx deben realizarse de manera cruzada, es decir :
// - El pin Rx del primer componente debe ser conectado al pin Tx del segundo componente
// - El pin Tx del primer componente debe ser conectado al pin Rx del segundo compnenete

// ********************************************

void setup() {  /// ********************** Funcion que se ejecuta al iniciar el circuito ************************
 
  #ifdef DEBUG_HX711
    // Iniciar comunicación serie
    Serial.begin(9600);
    Serial.println("............Iniciando comunicación con modulos HX711..........\n");
    Serial.println("...Iniciando las celdas de carga, No debe haber ningun peso...\n");
  #endif

  SPI.begin();        // inicializa bus SPI --> RFID
  mfrc522.PCD_Init();     // inicializa modulo lector --> RFID

  pinMode(8, OUTPUT); // Establece el pin 8 como salida para el pin PWR del modulo A6 GSM/GPRS el cual permite activarlo.
 
  // -----------------------------Iniciar sensor-----------------------------
  Resicolector1.begin(pinData1, pinClk1);
  Resicolector2.begin(pinData2, pinClk2);
//  Resicolector3.begin(pinData3, pinClk3);
//  Resicolector4.begin(pinData4, pinClk4);
//  Resicolector5.begin(pinData5, pinClk5);
//  Resicolector6.begin(pinData6, pinClk6);
  
  // ---------------- Aplicar la calibración al sensor de peso ----------------------
  Resicolector1.set_scale(CALIBRACION);
  Resicolector2.set_scale(CALIBRACION);
//  Resicolector3.set_scale(CALIBRACION);
//  Resicolector4.set_scale(CALIBRACION);
//  Resicolector5.set_scale(CALIBRACION);
//  Resicolector6.set_scale(CALIBRACION);

  // Iniciar la tara que se encarga de inicializar el sensor con un peso de 0 Kg...(No tiene que haber nada sobre el peso)
  Resicolector1.tare(20);
  Resicolector2.tare(20);
//  Resicolector3.tare();
//  Resicolector4.tare();
//  Resicolector5.tare();
//  Resicolector6.tare();
  
  Serial.println("*** Se ha inicializado la lectura del sensor en 0 Gramos ***\n");
  
// Activar modulo A6 GSM/GPRS
  pinMode(8, OUTPUT);
  digitalWrite(8,LOW);
  delay(5000);
  Serial.println("Activando modulo A6...");
  digitalWrite(8,HIGH);
  delay(1000);

// El modulo por defecto se establece con una velocidad de 115200 Bds
  Serial.println("configurando a 115200Bps.."); 
  A6MODULE.begin(115200);
 
  // cambiamos la velocidad de transmisión a la mismna velocidad de transmisión del Arduino 
  delay(5000);
  Serial.println("Cambiando a 9600Bds");
  A6MODULE.println("AT+IPR=9600");
  A6MODULE.begin(9600);
  delay(5000);

  Serial.println("Módulo listo.");
  
} /// ************************************************************************************************************************************

 
void loop() { /// *********************************** Funcion que se ejecuta constantemente **********************************************

  String UID=""; // Variable para guardar el valor del UID del RFID
  String txt=""; // Variable para guardar la informacion que se enviara posteriormente a la API
  
  if (mfrc522.PICC_IsNewCardPresent()) // si hay una tarjeta presente
  {
    if (mfrc522.PICC_ReadCardSerial())   // si puede obtener datos de la tarjeta
    {
      
      for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
          UID+=mfrc522.uid.uidByte[i]; // Se concatena el UID para convertirlo en String
      }

      txt="RESI-" + UID; // Se concatena la informacion de UID
      Serial.println(txt);

      // ************************** Detectar el cambio en peso **************************
      
      delay(5000); // Esperamos 4 segundos

      #ifdef DEBUG_HX711
        int weight1=(Resicolector1.get_units(10)); // Peso nuevo de la Resicolector 1
        if(weight1<0){
          weight1=0;
        }
//      
        int weight2=Resicolector2.get_units(10); // Peso nuevo de la Resicolector 2
        if(weight2<0){
          weight2=0;
        }
//      
//        int weight3=Resicolector3.get_units(10); // Peso nuevo de la Resicolector 3
//        if(weight3<0){
//          weight3=0;
//        }
      #endif

      txt = txt + "," + weight1 + "," + weight2 + "-END"; // Se concatena la informacion del peso de todos los Resicolectores
      enviarMensaje(txt); // Se llama la función para enviar el mensaje de texto
      
      Serial.println();
      Serial.println("***********************************");
      Serial.println();
      
      // ********************************************************************************
   
      mfrc522.PICC_HaltA();     // Detiene comunicacion con tarjeta --> RFID     
    }
  }
} /// **********************************************************************************************************************************


/// ---------------------- Funcion para enviar mensaje --------------------------

void enviarMensaje(String txt)
{
   Serial.println("Enviando AT...");
   A6MODULE.println("AT"); // Comando para verificar que hay comunicacion entre el modulo A6 y el Arduino
   delay(1000); 
   
   // Configurando modo texto
   A6MODULE.println("AT+CMGF=1"); 
   delay(1000);
  
  // Indicando de destino --> Numero de Sim Card de destino
   A6MODULE.println("AT+CMGS=\"+573174759950\"");
   delay(3000);
   A6MODULE.println(txt);
   delay(3000);
  
   A6MODULE.println(char(26)); // Comando para realizar el envio del mensaje
   delay(2000);

   Serial.println();
   Serial.println(txt);
   Serial.println();
   Serial.println("Se ha enviado mensaje de notificacion ... ");
}
