#include "ihmjustfeed.h"
#include "ui_ihmjustfeed.h"
#include "distributeur.h"
#include "basededonnees.h"
#include <QDebug>

/**
 * @file ihmjustfeed.cpp
 *
 * @brief Définition de la classe IHMJustFeed
 * @author Laura Morello <lauramorello1604@gmail.com>
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
    QMainWindow(parent), ui(new Ui::IHMJustFeed), nbLignesDistributeurs(0)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;

    baseDeDonnees = BaseDeDonnees::getInstance();
    baseDeDonnees->ouvrir("exemple.sqlite");

    initialiser();

    gererEvenements();
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
    BaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief Initialise le contenu de la page
 *
 * @fn IHMJustFeed::initialiser()
 */
void IHMJustFeed::initialiser()
{
    ajouterMenuAide();

    ui->statusbar->showMessage(QString::fromUtf8(NOM_APPLICATION) + " " +
                               QString::fromUtf8(VERSION_APPLICATION));

    //Initialise le QTableView
    nomColonnes << "Nom";
    modeleDistributeur = new QStandardItemModel(0, nomColonnes.size());
    modeleDistributeur->setHorizontalHeaderLabels(nomColonnes);
    ui->tableViewDistributeurs->setEditTriggers(
                QAbstractItemView::NoEditTriggers);

    // Redimensionner automatiquement la colonne pour occuper l'espace
    // disponible
    ui->tableViewDistributeurs->horizontalHeader()->setSectionResizeMode(
                QHeaderView::Stretch);
    nbLignesDistributeurs = modeleDistributeur->rowCount();

    afficherPage(IHMJustFeed::Page1);
}

/**
 * @brief Assure la connexion des signaux et slots
 *
 * @fn IHMJustFeed::gererEvenements
 */
void IHMJustFeed::gererEvenements()
{
    connect(ui->boutonRetour,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherPagePrincipale()));
}

/**
 * @brief Charge des données dans le QTableView
 *
 * @fn IHMJustFeed::chargerDistributeurs
 */
void IHMJustFeed::chargerDistributeurs()
{
    effacerTableDistributeurs();

    //Exemple avec une base de donnée SQLite
    QString requete = "SELECT * FROM Distributeur";
    bool retour;

    retour = BaseDeDonnees->recuperer(requete, distributeurs);
    qDebug() << Q_FUNC_INFO << retour;
    qDebug() << Q_FUNC_INFO << distributeurs;
    for(int i = 0; i < distributeurs.size(); ++i)
        afficherDistributeurTable(distributeurs.at(i));
}

/**
  * @brief Efface le QTableView
  *
  * @fn IHMJustFeed::effacerTableDistributeurs
  */
void IHMJustFeed::effacerTableDistributeurs()
{
    qDebug() << Q_FUNC_INFO;
    distributeurs.clear();
    modeleDistributeur->setHorizontalHeaderLabels(nomColonnes);
    ui->tableViewDistributeurs->setModel(modeleDistributeur);
    nbLignesDistributeurs = 0;
}

/**
 * @brief Affiche un utilisateur dans le QTableView
 *
 * @fn IHMJustFeed::afficherDistributeurTable
 * @param distributeur Lesinformations sur un utilisateur
 */
void IHMJustFeed::afficherDistributeurTable(QStringList distributeur)
{
    qDebug() << Q_FUNC_INFO << distributeur;

    // Crée les items pour les cellules d'une ligne
    QStandardItem* nom =
       new QStandardItem(distributeur.at(Distributeur::TABLE_DISTRIBUTEUR_NOM));

    //Ajoute les items dans le modèle de données
    modeleDistributeur->setItem(nbLignesDistributeurs,
                                IHMJustFeed::COLONNE_DISTRIBUTEUR_NOM,
                                nom);
    // Exemple de personnalisation de l'affichage d'une ligne
    QFont texte;
    // texte.setPointSize(12);
    texte.setBold(true);
    for(int i = 0; i < nomColonnes.size(); ++i)
    {
        QStandardItem* item = modeleDistributeur->item(nbLignesDistributeurs, i);
        item->setBackground(QColor(225, 223, 0));
        item->setFont(texte);
    }

    qDebug() << Q_FUNC_INFO << "nbLignesDistributeurs" << nbLignesDistributeurs;

    nbLignesDistributeurs += 1;

    //Configure l'affichage du QTableView
    ui->tableViewDistributeurs->setSizePolicy(QSizePolicy::Minimum,
                                              QSizePolicy::Minimum);
    ui->tableViewDistributeurs->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    ui->tableViewDistributeurs->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    // ui->tableViewDistributeurs->resizeColumnsToContents();

    ui->tableViewDistributeurs->setMinimumWidth(ui->centralwidget->width());
    // ui->tableViewUtilisateurs->setMinimumHeight(ui->centralwidget->height());
    /*ui->tableViewUtilisateurs->setFixedSize(
      ui->tableViewUtilisateurs->horizontalHeader()->length() +
        ui->tableViewUtilisateurs->verticalHeader()->width(),
      ui->tableViewUtilisateurs->verticalHeader()->length() +
        ui->tableViewUtilisateurs->horizontalHeader()->height());*/
    ui->tableViewUtilisateurs->setFixedHeight(
      ui->tableViewUtilisateurs->verticalHeader()->length() +
      ui->tableViewUtilisateurs->horizontalHeader()->height());
}

/**
 * @brief Affiche les données du distributeur sélectionné
 *
 * @fn IHMJustFeed::selectionner
 * @param index index dans le modèle de données
 */
void IHMJustFeed::selectionner(QModelIndex index)
{
    qDebug() << Q_FUNC_INFO << index.row(); // le numéro de ligne
       qDebug() << Q_FUNC_INFO
                << index.data().toString(); // le contenu de la cellule
       qDebug() << Q_FUNC_INFO
                << modeleDistributeur->item(index.row(), 0)->text(); //
       qDebug() << Q_FUNC_INFO << distributeurs.at(index.row());

       //Insère les donnéeq d'un distributeur à afficher
       ui->nomDistributeur->setText(
             distributeurs.at(index.row()).at(Distributeur::TABLE_DISTRIBUTEUR_NOM));

       //Affiche la page associé
       afficherPage(IHMJustFeed::Page1)
}

/**
 * @brief Méthode qui permet d'afficher une page de la pile
 * QStackedWidget
 *
 * @fn IHMJustFeed::afficherPage
 * @param page le numéro de page à afficher
 */
void IHMJustFeed::afficherPage(IHMJustFeed::Page page)
{
    qDebug() << Q_FUNC_INFO << "page" << page;
       ui->pages->setCurrentIndex(page);
}

/**
 * @brief Affiche la première page
 *
 * @fn IHM::afficherPagePrincipale
 */
void IHMJustFeed::afficherPagePrincipale()
{
    afficherPage(IHMJustFeed::Page1);
}

