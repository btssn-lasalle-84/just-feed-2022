#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include "distributeur.h"

#define PORT_TTN_POIDS         1
#define PORT_TTN_ENVIRONNEMENT 2

class Communication : public QObject
{
    Q_OBJECT
  public:
    Communication(QObject* parent = nullptr);
    ~Communication();

    bool    estConnecte() const;
    void    connecter();
    void    connecter(QString hostname,
                      int     port,
                      QString username,
                      QString password);
    void    reconnecter();
    void    reconnecter(QString hostname,
                        int     port,
                        QString username,
                        QString password);
    void    deconnecter();
    bool    abonner(QString deviceID);
    void    desabonner(QString deviceID);
    QString getHostname() const;
    QString getUsername() const;
    QString getPassword() const;

  private:
    QMqttClient*       clientMqtt;
    QMqttSubscription* abonnementMqtt;
    QString            hostname;
    int                port;
    QString            username;
    QString            password;

    void configurer();
    void sauvegarder();

  public slots:
    void changerEtat();
    void recevoirMessage(const QByteArray&     message,
                         const QMqttTopicName& topic);

  signals:
    void ttnConnecte();
    void ttnDeconnecte();
    void messageJournal(QString message);
};

#endif // COMMUNICATION_H
