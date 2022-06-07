#include "communication.h"
#include "ihmjustfeed.h"
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QJsonDocument>
#include <QDateTime>
#include <QDebug>

/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Laura Morello <lauramorello1604@gmail.com>
 * @version 1.0
 *
 */

/**
 * @brief Constructeur de la classe Communication
 * @param parent
 * @fn Communication::Communication
 */
Communication::Communication(QObject* parent) :
    QObject(parent), clientMqtt(new QMqttClient(this)), abonnementMqtt(nullptr)
{
    qDebug() << Q_FUNC_INFO;

    configurer();
}

/**
 * @brief Destructeur de la classe Communication
 * @fn Communication::~Communication
 */
Communication::~Communication()
{
    this->deconnecter();
    this->sauvegarder();
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Méthode qui retourne une valeur booléene true si le client Mqtt est
 * connecté
 * @fn Communication::estConnecte
 * @return bool : une valeur booléene true ou false
 */
bool Communication::estConnecte() const
{
    if(clientMqtt->state() == QMqttClient::Connected)
        return true;
    return false;
}

/**
 * @brief Méthode qui connecte le client Mqtt (par défaut) et permet la
 * récupération du message (à l'aide de la connection)
 * @fn Communication::connecter
 * @return void : un message dans le journal
 */
void Communication::connecter()
{
    if(clientMqtt->state() != QMqttClient::Connected)
    {
        qDebug() << Q_FUNC_INFO << hostname << port << username << password;

        /*
            hostname -> eu1.cloud.thethings.network
            port -> 1883
            username -> just-feed-2022@ttn
            password -> secret (voir console TTN)
         */

        clientMqtt->setHostname(hostname);
        clientMqtt->setPort(port);
        clientMqtt->setUsername(username);
        clientMqtt->setPassword(password);

        clientMqtt->connectToHost();

        connect(clientMqtt,
                SIGNAL(messageReceived(QByteArray, QMqttTopicName)),
                this,
                SLOT(recevoirMessage(QByteArray, QMqttTopicName)));
    }
}

/**
 * @brief Méthode qui connecte le client Mqtt à partir des paramètres (surcharge
 * de connecter())
 * @fn Communication::connecter
 * @param hostname
 * @param port
 * @param username
 * @param password
 * @return void
 */
void Communication::connecter(QString hostname,
                              int     port,
                              QString username,
                              QString password)
{
    if(clientMqtt->state() != QMqttClient::Connected)
    {
        /*
            hostname -> eu1.cloud.thethings.network
            port -> 1883
            username -> just-feed-2022@ttn
            password -> secret (voir console TTN)
         */
        this->hostname = hostname;
        this->port     = port;
        this->username = username;
        this->password = password;

        this->connecter();
    }
}

/**
 * @brief Méthode qui déconnecte puis connecte le client Mqtt
 * @fn Communication::reconnecter
 * @return void
 */
void Communication::reconnecter()
{
    qDebug() << Q_FUNC_INFO;
    this->deconnecter();
    this->connecter();
}

/**
 * @brief Méthode qui déconnecte puis connecte le client Mqtt à partir des
 * paramètres (surcharge de reconnecter())
 * @fn Communication::reconnecter
 * @param hostname
 * @param port
 * @param username
 * @param password
 * @return void
 */
void Communication::reconnecter(QString hostname,
                                int     port,
                                QString username,
                                QString password)
{
    this->deconnecter();
    this->connecter(hostname, port, username, password);
}

/**
 * @brief Méthode qui déconnecte le client Mqtt
 * @fn Communication::deconnecter
 * @return void : un message dans le journal
 */
void Communication::deconnecter()
{
    if(clientMqtt->state() != QMqttClient::Connected)
        return;
    clientMqtt->disconnectFromHost();
    disconnect(clientMqtt,
               SIGNAL(messageReceived(QByteArray, QMqttTopicName)),
               this,
               SLOT(recevoirMessage(QByteArray, QMqttTopicName)));
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Méthode qui abonne le client Mqtt à un topic , à partir du deviceID
 * @fn Communication::abonner
 * @param deviceID
 * @return bool : une valeur booléene true ou false , et un message dans le
 * journal
 */
bool Communication::abonner(QString deviceID)
{
    if(clientMqtt->state() != QMqttClient::Connected)
        return false;

    // Exemple de deviceID : distributeur-1-sim, distributeur-2-sim
    // Exemple de topic : v3/just-feed-2022@ttn/devices/distributeur-1-sim/up

    QString topic =
      QString("v3/") + username + QString("/devices/") + deviceID + "/up";
    qDebug() << Q_FUNC_INFO << deviceID << topic;
    QMqttTopicFilter topicFilter(topic.toLatin1());
    abonnementMqtt = clientMqtt->subscribe(topicFilter);

    if(!abonnementMqtt)
    {
        QMessageBox::critical(0, "Erreur", "Impossible de s'abonner !");
        return false;
    }

    return true;
}

/**
 * @brief Méthode qui désabonne le client Mqtt , à partir du deviceID
 * @fn Communication::desabonner
 * @param deviceID
 * @return void : un message dans le journal
 */
void Communication::desabonner(QString deviceID)
{
    if(clientMqtt->state() != QMqttClient::Connected)
        return;
    /**
     * @warning Si un client se déconnecte sans se désabonner, le serveur MQTT
     * stockera tous les messages et les publiera lors de la prochaine
     * reconnexion.
     */
    QString topic =
      QString("v3/") + username + QString("/devices/") + deviceID + "/up";
    qDebug() << Q_FUNC_INFO << deviceID << topic;
    QMqttTopicFilter topicFilter(topic.toLatin1());
    clientMqtt->unsubscribe(topicFilter);
}

/**
 * @brief Methode qui retourne le nom d'hote
 * @fn Communication::getHostname
 * @return QString
 */
QString Communication::getHostname() const
{
    return this->hostname;
}

/**
 * @brief Methode qui retourne le nom d'utilisateur
 * @fn Communication::getUsername
 * @return QString
 */
QString Communication::getUsername() const
{
    return this->username;
}

/**
 * @brief Methode qui retourne le mot de passe
 * @fn Communication::getPassword
 * @return
 */
QString Communication::getPassword() const
{
    return this->password;
}

/**
 * @brief Méthode qui configure le client Mqtt , à partir du fichier
 * justfeed.ini
 * @fn Communication::configurer
 * @return void
 */
void Communication::configurer()
{
    QSettings settings(QDir::currentPath() + "/justfeed.ini",
                       QSettings::IniFormat);

    settings.beginGroup("TTN");
    hostname = settings.value("Hostname").toString();
    port     = settings.value("Port").toInt();
    username = settings.value("Username").toString();
    password = settings.value("Password").toString();
    settings.endGroup();
    settings.sync();
    qDebug() << Q_FUNC_INFO << hostname << port << username << password;

    connect(clientMqtt,
            SIGNAL(stateChanged(ClientState)),
            this,
            SLOT(changerEtat()));
}

/**
 * @brief Méthode qui sauvegarde la configuration du client Mqtt , dans le
 * fichier justfeed.ini
 * @fn Communication::sauvegarder
 * @return void : sauvegarde des parametres dans justfeed.ini
 */
void Communication::sauvegarder()
{
    QSettings settings(QDir::currentPath() + "/justfeed.ini",
                       QSettings::IniFormat);

    settings.beginGroup("TTN");
    settings.setValue("Hostname", hostname);
    settings.setValue("Port", port);
    settings.setValue("Username", username);
    settings.setValue("Password", password);
    settings.sync();

    // qDebug() << Q_FUNC_INFO << hostname << port <<  username << password;
}

/**
 * @brief Méthode qui signal l'état du client Mqtt à l'IHM
 * @fn Communication::changerEtat
 * @return void : un etat connecté ou deconnecté dans l'IHM, et un message dans
 * le journal
 */
void Communication::changerEtat()
{
    qDebug() << Q_FUNC_INFO << clientMqtt->state();
    QString message;
    switch(clientMqtt->state())
    {
        case QMqttClient::Disconnected:
            qDebug() << Q_FUNC_INFO << "Déconnecté";
            emit ttnDeconnecte();
            break;
        case QMqttClient::Connecting:
            qDebug() << Q_FUNC_INFO << "En cours de connexion";
            break;
        case QMqttClient::Connected:
            qDebug() << Q_FUNC_INFO << "Connecté";
            emit ttnConnecte();
            sauvegarder();
            break;
        default:
            break;
    }
}

/**
 * @brief Méthode qui reçoit et traite le message du topic, et signal les
 * nouvelles mesures à l'IHM
 * @fn Communication::recevoirMessage
 * @param messageRecu, topic
 * @return void : des nouvelles mesures dans l'IHM, et un message dans le
 * journal
 */
void Communication::recevoirMessage(const QByteArray&     messageRecu,
                                    const QMqttTopicName& topic)
{
    QJsonDocument documentJSON = QJsonDocument::fromJson(messageRecu);
    QJsonObject   objetJSON    = documentJSON.object();

    qDebug() << Q_FUNC_INFO << "topic" << topic;
    // qDebug() << Q_FUNC_INFO << "message" << messageRecu;
    // qDebug() << Q_FUNC_INFO << "objetJSON" << objetJSON;

    // Extraction du deviceID et du port
    QJsonObject end_device_ids =
      objetJSON.value(QString("end_device_ids")).toObject();
    // qDebug() << Q_FUNC_INFO << "end_device_ids" << end_device_ids;
    QString deviceID = end_device_ids.value(QString("device_id")).toString();
    qDebug() << Q_FUNC_INFO << "deviceID" << deviceID;

    QJsonObject uplink_message =
      objetJSON.value(QString("uplink_message")).toObject();
    // qDebug() << Q_FUNC_INFO << "uplink_message" << uplink_message;
    int port = uplink_message.value(QString("f_port")).toInt();
    qDebug() << Q_FUNC_INFO << "port" << port;
    QJsonObject messageDonnees =
      uplink_message.value(QString("decoded_payload")).toObject();
    int bac1, bac2;
    switch(port)
    {
        case PORT_BACS:
            bac1 = uplink_message.value(QString("bac1")).toInt();
            bac2 = uplink_message.value(QString("bac2")).toInt();
            qDebug() << Q_FUNC_INFO << "bacs" << bac1 << bac2;

            emit nouvellesDonneesPortBacs(deviceID, bac1, bac2);
            break;
        case PORT_ENVIRONNEMENT:
            /**
             * @todo extraire les données en fonction du numéro de port
             */
            break;
        case PORT_MAINTENANCE:
            /**
             * @todo extraire les données en fonction du numéro de port
             */
            break;
        default:
            break;
    }
}
