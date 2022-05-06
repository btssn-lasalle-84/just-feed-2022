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
    QMainWindow(parent), ui(new Ui::IHMJustFeed), nbLignesDistributeurs(0),
    numeroDistributeurSelectionne(-1)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;

    baseDeDonnees = BaseDeDonnees::getInstance();
    baseDeDonnees->ouvrir("just-feed.sqlite");

    initialiser();
    gererEvenements();
    chargerDistributeurs();
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
    ui->labelTitreProjet->setText(QString::fromUtf8(NOM_APPLICATION) + " v" +
                                  QString::fromUtf8(VERSION_APPLICATION));

    // Initialise le QTableView
    nomColonnes << "Nom"
                << "Ville"
                << "Code Postal";
    modeleDistributeur = new QStandardItemModel(0, nomColonnes.size());
    modeleDistributeur->setHorizontalHeaderLabels(nomColonnes);
    ui->tableViewDistributeurs->setEditTriggers(
      QAbstractItemView::NoEditTriggers);

    // Redimensionner automatiquement la colonne pour occuper l'espace
    // disponible
    ui->tableViewDistributeurs->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
    nbLignesDistributeurs = modeleDistributeur->rowCount();

    afficherPageAccueil();
}

/**
 * @brief Assure la connexion des signaux et slots
 *
 * @fn IHMJustFeed::gererEvenements
 */
void IHMJustFeed::gererEvenements()
{
    connect(ui->pushButtonRetour,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherPageAccueil()));
    connect(ui->pushButtonEtat,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherPageEtatDistributeur()));
    connect(ui->pushButtonIntervention,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherPageInterventionDistributeur()));
    connect(ui->pushButtonGeolocalisation,
            SIGNAL(clicked(bool)),
            this,
            SLOT(afficherPageGeolocalisationDistributeur()));
    connect(ui->comboBoxDistributeurs,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(selectionnerDistributeur(int)));
    connect(ui->tableViewDistributeurs,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(selectionner(QModelIndex)));
}

int IHMJustFeed::recupererIndexEtatsDistributeur(QString idDistibuteur)
{
    for(int index = 0; index < etatsDistributeurs.size(); ++index)
    {
        if(etatsDistributeurs.at(index).at(
             Distributeur::ChampDistributeur::CHAMP_idDistributeur) ==
           idDistibuteur)
            return index;
    }
    return -1; // pas trouvé
}

/**
 * @brief Charge des données dans le QTableView
 *
 * @fn IHMJustFeed::chargerDistributeurs
 */
void IHMJustFeed::chargerDistributeurs()
{
    effacerTableDistributeurs();

    // Exemple avec une base de donnée SQLite
    QString requete = "SELECT * FROM Distributeur";

    baseDeDonnees->recuperer(requete, distributeurs);
    qDebug() << Q_FUNC_INFO << distributeurs;
    ui->comboBoxDistributeurs->clear();
    ui->comboBoxDistributeurs->addItem("");
    ui->pushButtonEtat->setEnabled(false);
    ui->pushButtonIntervention->setEnabled(false);
    ui->pushButtonGeolocalisation->setEnabled(false);
    for(int i = 0; i < distributeurs.size(); ++i)
    {
        ui->comboBoxDistributeurs->addItem(distributeurs.at(i).at(
          Distributeur::ChampDistributeur::CHAMP_libelle));
        afficherDistributeurTable(distributeurs.at(i));
    }

    requete =
      "SELECT "
      "Distributeur.*,Produit.designation,NiveauApprovisionnement.libelle AS "
      "niveauApprovisionnement,StockDistributeur.numeroBac,StockDistributeur."
      "quantite,StockDistributeur.quantiteMax FROM StockDistributeur "
      "INNER JOIN Distributeur ON "
      "Distributeur.idDistributeur=StockDistributeur.idDistributeur "
      "INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit "
      "INNER JOIN NiveauApprovisionnement ON "
      "NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur."
      "idNiveauApprovisionnement "
      "INNER JOIN ServeurTTN ON "
      "ServeurTTN.idServeurTTN=Distributeur.idServeurTTN;";
    baseDeDonnees->recuperer(requete, etatsDistributeurs);
    qDebug() << Q_FUNC_INFO << etatsDistributeurs;
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
 * @brief Affiche un distributeur dans le QTableView
 *
 * @fn IHMJustFeed::afficherDistributeurTable
 * @param distributeur Lesinformations sur un utilisateur
 */
void IHMJustFeed::afficherDistributeurTable(QStringList distributeur)
{
    qDebug() << Q_FUNC_INFO << distributeur;

    // Crée les items pour les cellules d'une ligne
    QStandardItem* nom = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_libelle));
    QStandardItem* ville = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_ville));
    QStandardItem* codePostal = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_codepostal));

    // Ajoute les items dans le modèle de données
    modeleDistributeur->setItem(nbLignesDistributeurs,
                                IHMJustFeed::COLONNE_DISTRIBUTEUR_NOM,
                                nom);
    modeleDistributeur->setItem(nbLignesDistributeurs,
                                IHMJustFeed::COLONNE_DISTRIBUTEUR_VILLE,
                                ville);
    modeleDistributeur->setItem(nbLignesDistributeurs,
                                IHMJustFeed::COLONNE_DISTRIBUTEUR_CODEPOSTAL,
                                codePostal);
    // Exemple de personnalisation de l'affichage d'une ligne
    QFont texte;
    // texte.setPointSize(12);
    texte.setBold(true);
    for(int i = 0; i < nomColonnes.size(); ++i)
    {
        QStandardItem* item =
          modeleDistributeur->item(nbLignesDistributeurs, i);
        item->setBackground(QColor(225, 223, 0));
        item->setFont(texte);
    }

    qDebug() << Q_FUNC_INFO << "nbLignesDistributeurs" << nbLignesDistributeurs;

    nbLignesDistributeurs += 1;

    // Configure l'affichage du QTableView
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
    ui->tableViewDistributeurs->setFixedHeight(
      ui->tableViewDistributeurs->verticalHeader()->length() +
      ui->tableViewDistributeurs->horizontalHeader()->height());
}

void IHMJustFeed::afficherEtatDistributeur(int indexDistributeur)
{
    QString idDistributeur =
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_idDistributeur);
    qDebug() << Q_FUNC_INFO << indexDistributeur << idDistributeur;
    int index = recupererIndexEtatsDistributeur(idDistributeur);
    qDebug() << Q_FUNC_INFO << index;
    if(index == -1)
        return;

    ui->labelNom->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_libelle));
    ui->labelAdresse->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_adresse));
    ui->labelVille->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_codepostal) +
      QString(" ") +
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_ville));

    for(int i = 0; i < distributeurs.at(indexDistributeur)
                         .at(Distributeur::ChampDistributeur::CHAMP_nbBacs)
                         .toInt();
        ++i)
    {
        if(etatsDistributeurs.at(index + i).at(
             Distributeur::ChampDistributeur::CHAMP_numeroBac) == "1")
        {
            ui->progressBarRemplissageBac1->setValue(
              (etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantite)
                 .toDouble() /
               etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantiteMax)
                 .toDouble()) *
              100);
            ui->labelNomProduitRemplissageBac1->setText(
              QString("Bac 1 : ") +
              etatsDistributeurs.at(index + i).at(
                Distributeur::ChampDistributeur::CHAMP_designationProduit));
        }
        else if(etatsDistributeurs.at(index + i).at(
                  Distributeur::ChampDistributeur::CHAMP_numeroBac) == "2")
        {
            ui->progressBarRemplissageBac2->setValue(
              (etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantite)
                 .toDouble() /
               etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantiteMax)
                 .toDouble()) *
              100);
            ui->labelNomProduitRemplissageBac2->setText(
              QString("Bac 2 : ") +
              etatsDistributeurs.at(index + i).at(
                Distributeur::ChampDistributeur::CHAMP_designationProduit));
        }

        ui->progressBarHygrometrieBacs->setValue(
          etatsDistributeurs.at(indexDistributeur)
            .at(Distributeur::ChampDistributeur::CHAMP_hygrometrie)
            .toInt());
    }
}

void IHMJustFeed::afficherInterventionDistributeur(int indexDistributeur)
{
    QString idDistributeur =
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_idDistributeur);
    qDebug() << Q_FUNC_INFO << indexDistributeur << idDistributeur;
    int index = recupererIndexInterventionDistributeur(idDistributeur);
    qDebug() << Q_FUNC_INFO << index;
    if(index == -1)
        return;

    ui->labelNomIntervention->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_libelle));
    ui->labelAdresseIntervention->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_adresse));
    ui->labelVilleIntervention->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_codepostal) +
      QString(" ") +
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_ville));
}

void IHMJustFeed::afficherGeolocalisationDistributeur(int indexDistributeur)
{
    QString idDistributeur =
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_idDistributeur);
    qDebug() << Q_FUNC_INFO << indexDistributeur << idDistributeur;
    int index = recupererIndexGeolocalisationDistributeur(idDistributeur);
    qDebug() << Q_FUNC_INFO << index;
    if(index == -1)
        return;

    ui->labelNomGeolocalisation->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_libelle));
    ui->labelAdresseGeolocalisation->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_adresse));
    ui->labelVilleGeolocalisation->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_codepostal) +
      QString(" ") +
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_ville));
}

/**
 * @brief Affiche les données du distributeur sélectionné
 *
 * @fn IHMJustFeed::selectionner
 * @param index index dans le modèle de données
 */
void IHMJustFeed::selectionner(QModelIndex index)
{
    qDebug() << Q_FUNC_INFO << "numéro de distributeur"
             << index.row(); // le numéro de ligne dans la table
    qDebug() << Q_FUNC_INFO << "distributeur" << distributeurs.at(index.row());

    ui->comboBoxDistributeurs->setCurrentIndex(index.row() + 1);
}

void IHMJustFeed::selectionnerDistributeur(int index)
{
    qDebug() << Q_FUNC_INFO << "numéro de distributeur"
             << index; // le numéro dans la liste déroulante
    if(index == 0)
    {
        numeroDistributeurSelectionne = -1;
        ui->pushButtonEtat->setEnabled(false);
        ui->pushButtonIntervention->setEnabled(false);
        ui->pushButtonGeolocalisation->setEnabled(false);
    }
    else
    {
        numeroDistributeurSelectionne = index - 1;
        ui->pushButtonEtat->setEnabled(true);
        ui->pushButtonIntervention->setEnabled(true);
        ui->pushButtonGeolocalisation->setEnabled(true);
    }
}

/**
 * @brief Méthode qui permet d'afficher une page de la pile
 * QStackedWidget
 *
 * @fn IHMJustFeed::afficherPage
 * @param page le numéro de page à afficher
 */
void IHMJustFeed::afficherPage(Page page)
{
    qDebug() << Q_FUNC_INFO << "page" << page;
    ui->pages->setCurrentIndex(page);
}

/**
 * @brief Affiche la première page
 *
 * @fn IHM::afficherPagePrincipale
 */
void IHMJustFeed::afficherPageAccueil()
{
    ui->pushButtonRetour->hide();
    afficherPage(Page::Accueil);
}

void IHMJustFeed::afficherPageEtatDistributeur()
{
    qDebug() << Q_FUNC_INFO << "numeroDistributeurSelectionne"
             << numeroDistributeurSelectionne;
    afficherEtatDistributeur(numeroDistributeurSelectionne);
    ui->pushButtonRetour->show();
    afficherPage(Page::Distributeur);
}

void IHMJustFeed::afficherPageInterventionDistributeur()
{
    ui->pushButtonRetour->show();
    afficherPage(Page::Intervention);
}

void IHMJustFeed::afficherPageGeolocalisationDistributeur()
{
    ui->pushButtonRetour->show();
    afficherPage(Page::Geolocalisation);
}
