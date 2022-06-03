#ifndef IHMJUSTFEED_H
#define IHMJUSTFEED_H

/**
 * @file ihmjustfeed.h
 *
 * @brief Déclaration de la classe IHMJustFeed
 * @author Laura MORELLO <lauramorello1604@gmail.com>
 * @version 1.0
 *
 */

#include <QtWidgets>
#include <QWebEnginePage>

/**
 * @def NOM_APPLICATION
 * @brief Le nom de l'application
 */

#define NOM_APPLICATION "JustFeed"

/**
 * @def VERSION_APPLICATION
 * @brief La version de l'application
 */

#define VERSION_APPLICATION "1.0"

// QT_BEGIN_NAMESPACE
namespace Ui
{
class IHMJustFeed;
}
// QT_END_NAMESPACE

class BaseDeDonnees;
class Communication;

/**
 * @class IHMJustFeed
 * @brief Déclaration de la classe IHMJustFeed
 * @details Cette classe s'occupe de l'affichage
 */
class IHMJustFeed : public QMainWindow
{
    Q_OBJECT

  public:
    IHMJustFeed(QWidget* parent = nullptr);
    ~IHMJustFeed();

  private:
    Ui::IHMJustFeed* ui; //!< la fenêtre graphique associée à cette classe
    BaseDeDonnees*
      baseDeDonnees; //!< adresse d'une instance d'un objet BaseDeDonnees
    Communication*
                         communicationMQTT; //!< adresse d'une instance d'un objet Communication
    QStringList          nomColonnes;           //!< Liste de nom des colonnes
    int                  nbLignesDistributeurs; //!< nombre de lignes
    QVector<QStringList> distributeurs;         //!< Les distributeurs
    QVector<QStringList>
                        etatsDistributeurs; //!< Les états des stocks des distributeurs
    QStandardItemModel* modeleDistributeur; //!< Modèle pourle QTableView
    int numeroDistributeurSelectionne; //!< l'index du distributeur sélectionné
    QWebEnginePage* pageLocalisation;
    int             nbLignesInterventions; //!< nombre de lignes

    /**
     * @enum Page
     * @brief Définit les différentes pages de l'IHM
     *
     */
    enum Page
    {
        Accueil = 0,
        Distributeur,
        Intervention,
        Geolocalisation,
        NbPages
    };

    /**
     * @enum ColonneDistributeur
     * brief Définit les différentes colonne du QTableView
     */
    enum ColonneDistributeur
    {
        COLONNE_DISTRIBUTEUR_NOM,                //!< Emplacement du nom
        COLONNE_DISTRIBUTEUR_VILLE,              //!< Emplacement de la ville
        COLONNE_DISTRIBUTEUR_CODEPOSTAL,         //!< Emplacement du code postal
        COLONNE_DISTRIBUTEUR_DESIGNATIONPRODUIT, //!< Emplacement de la
                                                 //!< designation
        COLONNE_DISTRIBUTEUR_NIVEAUAPPROVISIONNEMENT, //!< Emplacement du
                                                      //!< libelle
        NB_COLONNES
    };

    void initialiser();
    void gererEvenements();
    void ouvrirBaseDeDonnees();
    int  recupererIndexEtatsDistributeur(QString idDistibuteur);
    int  recupererIndexInterventionDistributeur(QString idDistributeur);
    int  recupererIndexGeolocalisationDistributeur(QString idDistributeur);
    void afficherNiveauRemplissage(int pourcentage, int numeroBac);
    bool recupererDonneesDistributeurs();
    bool recupererEtatsDistributeurs();

  public slots:
    void chargerDistributeurs();
    void effacerTableDistributeurs();
    void afficherDistributeurTable(QStringList distributeur);
    void afficherEtatDistributeur(int indexDistributeur);
    void afficherHygrometrie(int indexDistributeur);
    void afficherInterventions(QStringList distributeur);
    void afficherGeolocalisationDistributeur(int indexDistributeur);
    void selectionner(QModelIndex index);
    void selectionnerDistributeur(int index);
    void effacerSelectionIntervention();
    void afficherPage(Page page);
    void afficherPageAccueil();
    void afficherPageEtatDistributeur();
    void afficherPageInterventionDistributeur();
    void afficherPageGeolocalisationDistributeur();
    void selectionnerBac1(bool etat);
    void selectionnerBac2(bool etat);
    void selectionnerEntretien(bool etat);
    void connecterDistributeurs();
};

#endif // IHMJUSTFEED_H
