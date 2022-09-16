/// *****************************************************************************
/// ------------------- Recepcion de SMS y envio a API --------------------------
/// ------------- Desarrollado por: Julian Andres Trujillo Molina ---------------
---------------------------------------------------------------------------------
/// *****************************************************************************


// ------------------------ Librerias necesarias ------------------------
#include <WiFi.h> // Libreria para usar el WIFI de la esp32
#include "Fetch.h" // Libreria para realizar peticiones http

const char* ssid = "Jatsukinger"; // Nombre de red WIFI
const char* password = "00000000"; // Contraseña de red WIFI

#define rxPin 16 // Pin Rx de la esp32
#define txPin 17 // Pin Tx de la esp32
#define BAUD_RATE 9600 // Velocidad de transmision de datos del modulo A6
HardwareSerial A6MODULE(2); // Crear objeto para comunicacion de la esp32 y el modulo A6 GSM/GPRS

//******************* NOTA*********************

// Tener en cuenta que la conexion entre dos componentes por medio de los pines Rx y Tx deben realizarse de manera cruzada, es decir :
// - El pin Rx del primer componente debe ser conectado al pin Tx del segundo componente
// - El pin Tx del primer componente debe ser conectado al pin Rx del segundo compnenete

// ********************************************

/// ----------------- Funcion para iniciar y conectar la esp32  a una red WIFI

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
/// --------------------------------------------------------------------------

/// Funcion que se ejecuta una sola vez al inicio

void setup() {

  Serial.begin(115200); // Velocidad de transmision de datos del serial del Arduino
  Serial.println("Iniciando puerto serial...");
  initWiFi(); // Se llama la funcion para iniciar el WIFI de la esp32
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  delay(5000); // Espera 5 segundos para seguir ejecutando el codigo

  if ((WiFi.status() == WL_CONNECTED)) { // Si se encunetra conectado a la red WIFI

 // Se inicia comunicacion con el modulo A6, BAUD_RATE = Velocidad de transmision, SERIAL_8N1 = Tipo de comunicación, pinRx, pinTx
    A6MODULE.begin(BAUD_RATE, SERIAL_8N1, rxPin, txPin);
    delay(5000);
    Serial.println("Iniciando modulo A6...");


    /// Se activa el modulo A6 enviando una señal al pin PWR del modulo
    pinMode(5, OUTPUT);
    digitalWrite(5,LOW);
    delay(5000);
    Serial.println("Activando modulo A6...");
    digitalWrite(5,HIGH);
    delay(1000);
    /// ----------------------------------------------------------------

    /// El modulo se configura por defecto en 115200
    Serial.println("configurando a 115200Bps.."); 
    A6MODULE.begin(115200);
    
    delay(2000);

    Serial.println("Enviando AT...");
    A6MODULE.println("AT"); // Comando para verificar que hay comunicacion entre el modulo A6 y el Arduino
    delay(3000); 
    
    // Configurando modo texto
    A6MODULE.println("AT+CMGF=1"); 
    delay(3000);
  
    Serial.println("Módulo listo.");
  
  } /// ************************************************************************************************************************************
}
 
void loop() { /// *********************************** Funcion que se ejecuta constantemente **********************************************

  String sms = ""; /// Se crea la variable para almacenar el mensaje recibido
  
  if(A6MODULE.available()){ /// Si se detecta un mensaje
    delay(1000); /// Espera 1 segundo
    sms = A6MODULE.readString();  /// Se almacena el mensaje recibido
    Serial.println(sms);
  
    int indexStart = sms.indexOf("RESI-"); /// Se busca la palabra clave 'RESI-' en el mensaje recibido y se guarda su indice
    int indexEnd = sms.indexOf("-END") + 4; /// Se busca la palabra clave '-END' en el mensaje recibido y se guarda su indice aumnetado en 4
    
    if(indexStart >= 0){ /// Si se encuentra la palabra clave es porque el mensaje provine de un resicolector
      
        String smsApi = sms.substring(indexStart, indexEnd); /// Se extrae la informacion necesria del mensaje
        enviarApi(smsApi);    /// Se llama la funcion para enviar el mensaje hacia la API
        
      }
  }
  delay(4000); // Se espera 4 segundos

} /// **********************************************************************************************************************************

/// Funcion para enviar el mensaje recibido hacia la API, por medio de peticiones http---------------------------------------------------

void enviarApi(String smsApi){

  String body = smsApi;
  RequestOptions options;
  options.method = "POST";
  options.headers["Content-Type"] = "application/json";
  options.headers["Content-Length"] = body.length();
  options.body = body;
  Response response = fetch("https://eogkku9415y3s9v.m.pipedream.net", options);
  Serial.println(response);
  
}

/// --------------------------------------------------------------------------------------------------------------------------------------
