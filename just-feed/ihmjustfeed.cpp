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

    ouvrirBaseDeDonnees();
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
    pageLocalisation = ui->webEngineViewLocalisation->page();
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
    connect(ui->pushButtonAnnuler,
            SIGNAL(clicked(bool)),
            this,
            SLOT(effacerSelectionIntervention()));
    connect(ui->comboBoxDistributeurs,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(selectionnerDistributeur(int)));
    connect(ui->tableViewDistributeurs,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(selectionner(QModelIndex)));
}

void IHMJustFeed::ouvrirBaseDeDonnees()
{
    baseDeDonnees = BaseDeDonnees::getInstance();
    baseDeDonnees->ouvrir("just-feed.sqlite");
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

int IHMJustFeed::recupererIndexInterventionDistributeur(QString idDistributeur)
{
    return -1; // pas trouvé
}

int IHMJustFeed::recupererIndexGeolocalisationDistributeur(
  QString idDistributeur)
{
    return -1; // pas trouvé
}

void IHMJustFeed::afficherNiveauRemplissage(int pourcentage, int numeroBac)
{
    QPalette p = palette();
    switch(numeroBac)
    {
        case 1:
            ui->progressBarRemplissageBac1->setValue(pourcentage);
            p.setColor(QPalette::Base, QColor(233, 185, 110)); // Background
            if(pourcentage > SEUIL_REMPLISSAGE_DEFAUT)
            {
                p.setColor(QPalette::Highlight, Qt::green);
            }
            else
            {
                p.setColor(QPalette::Highlight, Qt::red);
            }
            ui->progressBarRemplissageBac1->setPalette(p);
            break;
        case 2:
            ui->progressBarRemplissageBac2->setValue(pourcentage);
            p.setColor(QPalette::Base, QColor(233, 185, 110)); // Background
            if(pourcentage > SEUIL_REMPLISSAGE_DEFAUT)
            {
                p.setColor(QPalette::Highlight, Qt::green);
            }
            else
            {
                p.setColor(QPalette::Highlight, Qt::red);
            }
            ui->progressBarRemplissageBac2->setPalette(p);
            break;
        default:
            break;
    }
}

bool IHMJustFeed::recupererDonneesDistributeurs()
{
    QString requete = "SELECT * FROM Distributeur";

    distributeurs.clear();
    bool retour = baseDeDonnees->recuperer(requete, distributeurs);
    qDebug() << Q_FUNC_INFO << distributeurs;

    return retour;
}

bool IHMJustFeed::recupererEtatsDistributeurs()
{
    QString requete =
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
    etatsDistributeurs.clear();
    bool retour = baseDeDonnees->recuperer(requete, etatsDistributeurs);
    qDebug() << Q_FUNC_INFO << etatsDistributeurs;

    return retour;
}

/**
 * @brief Charge des données dans le QTableView
 *
 * @fn IHMJustFeed::chargerDistributeurs
 */
void IHMJustFeed::chargerDistributeurs()
{
    effacerTableDistributeurs();

    recupererDonneesDistributeurs();

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
 * @param distributeur Les informations sur un distributeur
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
            afficherNiveauRemplissage(
              (etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantite)
                 .toDouble() /
               etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantiteMax)
                 .toDouble()) *
                100,
              1);
            ui->labelNomProduitRemplissageBac1->setText(
              etatsDistributeurs.at(index + i).at(
                Distributeur::ChampDistributeur::CHAMP_designationProduit));
        }
        else if(etatsDistributeurs.at(index + i).at(
                  Distributeur::ChampDistributeur::CHAMP_numeroBac) == "2")
        {
            afficherNiveauRemplissage(
              (etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantite)
                 .toDouble() /
               etatsDistributeurs.at(index + i)
                 .at(Distributeur::ChampDistributeur::CHAMP_quantiteMax)
                 .toDouble()) *
                100,
              2);
            ui->labelNomProduitRemplissageBac2->setText(
              etatsDistributeurs.at(index + i).at(
                Distributeur::ChampDistributeur::CHAMP_designationProduit));
        }
    }

    afficherHygrometrie(indexDistributeur);
}

void IHMJustFeed::afficherHygrometrie(int indexDistributeur)
{
    qDebug() << Q_FUNC_INFO << "hygrometrie"
             << distributeurs.at(indexDistributeur)
                  .at(Distributeur::ChampDistributeur::CHAMP_hygrometrie);
    ui->progressBarHygrometrieBacs->setValue(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_hygrometrie)
        .toInt());
}

void IHMJustFeed::afficherInterventions(QStringList distributeur)
{
    qDebug() << Q_FUNC_INFO << distributeur;

    // Crée les items pour les cellules d'une ligne
    QStandardItem* nom = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_libelle));
    QStandardItem* ville = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_ville));
    QStandardItem* codePostal = new QStandardItem(
      distributeur.at(Distributeur::ChampDistributeur::CHAMP_codepostal));
    QStandardItem* designationProduit      = new QStandardItem(distributeur.at(
      Distributeur::ChampDistributeur::CHAMP_designationProduit));
    QStandardItem* niveauApprovisionnement = new QStandardItem(distributeur.at(
      Distributeur::ChampDistributeur::CHAMP_niveauApprovisionnement));

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
    modeleDistributeur->setItem(
      nbLignesDistributeurs,
      IHMJustFeed::COLONNE_DISTRIBUTEUR_DESIGNATIONPRODUIT,
      designationProduit);
    modeleDistributeur->setItem(
      nbLignesDistributeurs,
      IHMJustFeed::COLONNE_DISTRIBUTEUR_NIVEAUAPPROVISIONNEMENT,
      niveauApprovisionnement);

    /**
     * @todo Afficher les données des interventions
     */

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

void IHMJustFeed::afficherGeolocalisationDistributeur(int indexDistributeur)
{
    QString idDistributeur =
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_idDistributeur);
    qDebug() << Q_FUNC_INFO << indexDistributeur << idDistributeur;

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
    ui->labelLatitude->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_latitude));
    ui->labelLongitude->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_longitude));

    /**
     * @todo Afficher la longitude et la latitude dans le QLabel
     */
    qDebug() << Q_FUNC_INFO << "latitude"
             << distributeurs.at(indexDistributeur)
                  .at(Distributeur::ChampDistributeur::CHAMP_latitude);
    qDebug() << Q_FUNC_INFO << "longitude"
             << distributeurs.at(indexDistributeur)
                  .at(Distributeur::ChampDistributeur::CHAMP_longitude);
    ui->labelLatitude->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_latitude));
    ui->labelLongitude->setText(
      distributeurs.at(indexDistributeur)
        .at(Distributeur::ChampDistributeur::CHAMP_longitude));
    /*  Exemple :
        "https://www.openstreetmap.org/export/embed.html?bbox="
        "4.81696%2C43.9483,4.81696%2C43.9483&marker=43.9483,4.81696"));
     */
    QUrl url("https://www.openstreetmap.org/export/embed.html?bbox=" +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_longitude) +
             QString("%2C") +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_latitude) +
             QString(",") +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_longitude) +
             QString("%2C") +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_latitude) +
             QString("&marker=") +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_latitude) +
             QString(",") +
             distributeurs.at(indexDistributeur)
               .at(Distributeur::ChampDistributeur::CHAMP_longitude));
    qDebug() << Q_FUNC_INFO << url.toString();
    if(pageLocalisation)
        pageLocalisation->load(url);
}

/**
 * @brief Sélectionne un distributeur dans la table
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

/**
 * @brief Sélectionne un distributeur dans la liste
 *
 * @fn IHMJustFeed::selectionnerDistributeur
 * @param index index dans la liste déroulante
 */
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
 * @brief Slot déclenché par le bouton Annuler pour effacer la sélection
 *
 * @fn IHMJustFeed::effacerSelectionIntervention
 */
void IHMJustFeed::effacerSelectionIntervention()
{
    ui->checkBoxBac1->setChecked(false);
    ui->checkBoxBac2->setChecked(false);
    ui->checkBoxEntretien->setChecked(false);
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
    recupererDonneesDistributeurs(); // pour l'hygrométrie
    recupererEtatsDistributeurs();   // pour le stock
    afficherEtatDistributeur(numeroDistributeurSelectionne);
    ui->pushButtonRetour->show();
    afficherPage(Page::Distributeur);
}

void IHMJustFeed::afficherPageInterventionDistributeur()
{
    /*qDebug() << Q_FUNC_INFO << "numeroDistributeurSelectionne"
             << numeroDistributeurSelectionne;
    afficherInterventions(numeroDistributeurSelectionne);
    ui->pushButtonRetour->show();
    afficherPage(Page::Intervention);*/

    ui->pushButtonRetour->show();
    afficherPage(Page::Intervention);
}

void IHMJustFeed::afficherPageGeolocalisationDistributeur()
{
    qDebug() << Q_FUNC_INFO << "numeroDistributeurSelectionne"
             << numeroDistributeurSelectionne;
    recupererDonneesDistributeurs();
    afficherGeolocalisationDistributeur(numeroDistributeurSelectionne);
    ui->pushButtonRetour->show();
    afficherPage(Page::Geolocalisation);
}
