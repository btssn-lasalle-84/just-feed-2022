#include "ihmjustfeed.h"
#include "ui_ihmjustfeed.h"
#include <QDebug>

/**
 * @file ihmjustfeed.cpp
 *
 * @brief Définition de la classe IHMJustFeed
 * @author
 * @version 1.0
 *
 */

/**
 * @brief Constructeur de la classe IHMJustFeed
 *
 * @fn IHMJustFeed::IHMJustFeed
 * @param parent L'adresse de l'objet parent, si nullptr IHMJustFeed sera la
 * fenêtre principale de l'application
 */
IHMJustFeed::IHMJustFeed(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::IHMJustFeed)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Destructeur de la classe IHMJustFeed
 *
 * @fn IHMJustFeed::~IHMJustFeed
 * @details Libère les ressources de l'application
 */
IHMJustFeed::~IHMJustFeed()
{
    delete ui;
    qDebug() << Q_FUNC_INFO;
}
