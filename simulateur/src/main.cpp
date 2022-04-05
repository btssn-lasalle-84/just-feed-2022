/**
*
* @file src/main.cpp
* @brief Simulateur the-things-uno just-feed
* @author Thierry Vaira
* @version 0.1
*/

#include <Arduino.h>
#include <TheThingsNetwork.h>
#include <DHT.h>

/**
 * @def DISTRIBUTEUR_1
 * @brief Définit le distributeur n°1
 */
#define DISTRIBUTEUR_1
//#define DISTRIBUTEUR_2

// 2022
// AppId : just-feed

#ifdef DISTRIBUTEUR_1
// DevicesId : distributeur-1-sim
const char *appEui = "70B3D57ED003FFB5";
const char *appKey = "130048A2229D9342D4FB5CF7F91322E9"; //
#endif

#ifdef DISTRIBUTEUR_2
// DevicesId : distributeur-2-sim
const char *appEui = "70B3D57ED003FFB5";
const char *appKey = "CB217855A6EB5DB53F022AC82B91A0AD";
#endif

/*
  enum ttn_response_t
  {
    TTN_ERROR_SEND_COMMAND_FAILED = (-1),
    TTN_ERROR_UNEXPECTED_RESPONSE = (-10),
    TTN_SUCCESSFUL_TRANSMISSION = 1,
    TTN_SUCCESSFUL_RECEIVE = 2
  };
*/

/**
 * @def loraSerial
 * @brief Définit l'interface LoRa
 */
#define loraSerial Serial1
/**
 * @def debugSerial
 * @brief Définit le débuggage sur la liaison série
 */
#define debugSerial Serial
/**
 * @def freqPlan
 * @brief Définit la fréquence LoRa
 */
#define freqPlan TTN_FP_EU868

// un cycle conprend l'envoi des plusieurs ports
/**
 * @def PERIODE
 * @brief Définit la période d'envoi en ms des données
 */
#define PERIODE         60000 // en ms attente avant prochain cycle
/**
 * @def TEMPO
 * @brief Définit la temporistation en ms entre deux cycles d'envoi
 */
#define TEMPO           10000 // en ms attente entre deux envois dans un cycle
#define NB_ERREURS_MAX  3

#ifdef DISTRIBUTEUR_1
/**
 * @def NB_BACS
 * @brief Définit le nombre de bacs du distributeur
 */
#define NB_BACS     4

#define LEDPIN      4
#define DHTPIN      7
#define DHTTYPE     DHT22
//#define VBATPIN     A0
#endif

#ifdef DISTRIBUTEUR_2
/**
 * @def NB_BACS
 * @brief Définit le nombre de bacs du distributeur
 */
#define NB_BACS     3

#define LEDPIN      13
#define LEDROUGE    12
#define DHTPIN      4
#define DHTTYPE     DHT11
//#define VBATPIN     A0
//#define LUMINOSITE  A1
//#define LM35        A2
#endif



/*
  Port 1 : Remplissage des bacs % Entier Entier 1 octet pour chaque bac
  Port 2 : Humidité intérieure % Entier Entier 1 octet
  Port 3 : Maintenance prédictive
 */

#define PORT_BACS             1
#define PORT_ENVIRONNEMENT    2
#define PORT_MAINTENANCE      3

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan); //!< 
DHT dht(DHTPIN, DHTTYPE); //!< le capteur d'humidité
bool etatLed = false; //!< l'état de la led
uint16_t humidity; //!< l'humidité
uint8_t remplissageBacs[NB_BACS]; //!< le niveau de remplissage en % de chaque bac de produit
uint8_t demandeMaintenance = 0; //!< demande de maintrnance si = 1

byte payloadBacs[NB_BACS]; //!< les données de remplissage des bacs
byte payloadEnvironnement[1]; //!< les données d'environnement (humidité) du distributeur
byte payloadMaintenance[5]; //!< les données de maintenance

uint32_t compteurErreurs = 0; //!< compteur d'erreurs
uint32_t totalErreurs = 0; //!< total des erreurs
uint32_t totalTrames = 0; //!< total des trames
uint32_t tempo = TEMPO; //!<  cf. duty cycle et airtime calculators

void message(const byte *payload, size_t length, port_t port);
bool controlerReponse(ttn_response_t ttn_response);

/**
 * @brief Initialise les ressources
 *
 * @fn setup()
 */
void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Attente max de 10s pour le port série
  while (!debugSerial && millis() < 10000);

  #ifdef DISTRIBUTEUR_1
  debugSerial.println("-- DISTRIBUTEUR_1");
  #endif

  #ifdef DISTRIBUTEUR_1
  //pinMode(VBATPIN, INPUT);
  digitalWrite(LEDPIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  #endif

  #ifdef DISTRIBUTEUR_2
  debugSerial.println("-- DISTRIBUTEUR_2");
  #endif

  #ifdef DISTRIBUTEUR_2
  //pinMode(VBATPIN, INPUT);
  //pinMode(LUMINOSITE, INPUT);
  //pinMode(LM35, INPUT);
  digitalWrite(LEDPIN, LOW);
  digitalWrite(LEDROUGE, LOW);
  #endif
  dht.begin();

  // Pour les messages reçus
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  randomSeed(millis());

  // initialise le remplissage des bacs
  for(int i = 0; i < NB_BACS; ++i)
  {
    remplissageBacs[i] = 100;
  }
}

/**
 * @brief Boucle principale
 *
 * @fn loop()
 */
void loop()
{
  ttn_response_t ttn_response;
  bool etat = true;

  debugSerial.println("-- SEND");

  #ifdef DISTRIBUTEUR_1
  digitalWrite(LED_BUILTIN, HIGH);
  #endif

  #ifdef DISTRIBUTEUR_2
  digitalWrite(LEDROUGE, HIGH);
  #endif

  // PORT_BACS :
  if(etat)
  {
    delay(tempo);

    debugSerial.print("-> Port : ");
    debugSerial.println(PORT_BACS);
    uint8_t consommation = 0;
    for(int i = 0; i < NB_BACS; ++i)
    {
      // remplir ?
      if(remplissageBacs[i] == 0)
      {
          consommation = random(0, 5); // une chance sur 5 pour remplir
          if(consommation == 1)
            remplissageBacs[i] -= 100;
      }
      else // consommer ?
      {
        consommation = random(0, 10); // simule une petite consommation
        remplissageBacs[i] -= consommation;
        if(remplissageBacs[i] < 0)
          remplissageBacs[i] = 0;
      }
      payloadBacs[i] = lowByte(remplissageBacs[i]);
      debugSerial.print("   Bac ");
      debugSerial.print(i+1);
      debugSerial.print(" : ");
      debugSerial.println(remplissageBacs[i]);
    }

    ttn_response = ttn.sendBytes(payloadBacs, sizeof(payloadBacs), PORT_BACS);
    totalTrames++;
    etat = controlerReponse(ttn_response);
  }

  // PORT_ENVIRONNEMENT : humidité
  if(etat)
  {
    delay(tempo);

    debugSerial.print("-> Port : ");
    debugSerial.println(PORT_ENVIRONNEMENT);

    humidity = dht.readHumidity(false);
    payloadEnvironnement[0] = lowByte(humidity);
    debugSerial.print("   Humidite (int) : ");
    debugSerial.println(humidity);

    ttn_response = ttn.sendBytes(payloadEnvironnement, sizeof(payloadEnvironnement), PORT_ENVIRONNEMENT);
    totalTrames++;
    etat = controlerReponse(ttn_response);
  }

  // PORT_MAINTENANCE : maintenance
  if(etat)
  {
    delay(tempo);

    uint8_t taus = 0;
    taus = random(0, 5); // une chance sur 5 pour simuler une demande de maintenance
    if(taus == 1)
      demandeMaintenance = 1;
    else
      demandeMaintenance = 0;
    debugSerial.print("-> Port : ");
    debugSerial.println(PORT_MAINTENANCE);

    payloadMaintenance[0] = lowByte(demandeMaintenance);
    payloadMaintenance[1] = highByte(uint16_t(totalErreurs));
    payloadMaintenance[2] = lowByte(uint16_t(totalErreurs));
    payloadMaintenance[3] = highByte(uint16_t(totalTrames));
    payloadMaintenance[4] = lowByte(uint16_t(totalTrames));
    debugSerial.print("   Maintenance : ");
    debugSerial.println(demandeMaintenance);

    ttn_response = ttn.sendBytes(payloadMaintenance, sizeof(payloadMaintenance), PORT_MAINTENANCE);
    totalTrames++;
    etat = controlerReponse(ttn_response);
  }

  debugSerial.print("-- STATUS : ");
  debugSerial.print(totalErreurs);
  debugSerial.print(" / ");
  debugSerial.println(totalTrames);

  #ifdef DISTRIBUTEUR_1
  digitalWrite(LED_BUILTIN, LOW);
  #endif

  #ifdef DISTRIBUTEUR_2
  digitalWrite(LEDROUGE, LOW);
  #endif

  delay(PERIODE);
}

void message(const byte *payload, size_t length, port_t port)
{
  debugSerial.println("-- RECEIVE");
  debugSerial.print("   Port : ");
  debugSerial.println(port);
  debugSerial.print("   Longueur : ");
  debugSerial.println(length);

  /*
  // 1 seul octet accepté
  if (length != 1)
  {
    return;
  }

  if (payload[0] == 0)
  {
    debugSerial.println("LED : off");
    digitalWrite(LEDPIN, LOW);
    etatLed = false;
  }
  else if (payload[0] == 1)
  {
    debugSerial.println("LED : on");
    digitalWrite(LEDPIN, HIGH);
    etatLed = true;
  }
  */
}

bool controlerReponse(ttn_response_t ttn_response)
{
  bool etat = false;
  switch(ttn_response)
  {
    case TTN_ERROR_SEND_COMMAND_FAILED :
      debugSerial.println("-- RESPONSE : TTN_ERROR_SEND_COMMAND_FAILED");
      compteurErreurs++;
      totalErreurs++;
      tempo = compteurErreurs * TEMPO;
      etat = false;
      break;
    case TTN_ERROR_UNEXPECTED_RESPONSE :
      debugSerial.println("-- RESPONSE : TTN_ERROR_UNEXPECTED_RESPONSE");
      compteurErreurs++;
      totalErreurs++;
      etat = false;
      break;
    case TTN_SUCCESSFUL_TRANSMISSION :
      // reset le compteur d'erreurs
      compteurErreurs = 0;
      tempo = TEMPO;
      etat = true;
      break;
    case TTN_SUCCESSFUL_RECEIVE :
      etat = true;
      break;
    default:
      debugSerial.println("-- RESPONSE : UNKNOWN_ERROR !");
      break;
  }
  if(compteurErreurs > NB_ERREURS_MAX)
  {
    debugSerial.print("-- NB ERROR : ");
    debugSerial.println(compteurErreurs);
    debugSerial.println("-- RESET");
    ttn.reset();
    delay(250);
    debugSerial.println("-- JOIN");
    etat = ttn.join(appEui, appKey);
    debugSerial.print("-- JOINED : ");
    debugSerial.println(etat);
  }

  return etat;
}
