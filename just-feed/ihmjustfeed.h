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
    BaseDeDonnees*   baseDeDonnees; //!< Instance d'un objet BaseDeDonnees
    QStringList      nomColonnes;   //!< Liste de nom des colonnes
    int              nbLignesDistributeurs; //!< nombre de lignes
    QVector<QStringList> distributeurs;     //!< Les distributeurs
    QVector<QStringList>
                        etatsDistributeurs; //!< Les états des stocks des distributeurs
    QStandardItemModel* modeleDistributeur; //!< Modèle pourle QTableView
    int numeroDistributeurSelectionne; //!< l'index du distributeur sélectionné

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
        COLONNE_DISTRIBUTEUR_NOM,        //!< Emplacement du nom
        COLONNE_DISTRIBUTEUR_VILLE,      //!< Emplacement de la ville
        COLONNE_DISTRIBUTEUR_CODEPOSTAL, //!< Emplacement du code postal
        NB_COLONNES
    };

    void initialiser();
    void gererEvenements();
    int  recupererIndexEtatsDistributeur(QString idDistibuteur);
    int  recupererIndexInterventionDistributeur(QString idDistributeur);
    int  recupererIndexGeolocalisationDistributeur(QString idDistributeur);
    void afficherNiveauRemplissage(int pourcentage, int numeroBac);

  public slots:
    void chargerDistributeurs();
    void effacerTableDistributeurs();
    void afficherDistributeurTable(QStringList distributeur);
    void afficherEtatDistributeur(int indexDistributeur);
    void afficherInterventionDistributeur(int indexDistributeur);
    void afficherGeolocalisationDistributeur(int indexDistributeur);
    void selectionner(QModelIndex index);
    void selectionnerDistributeur(int index);
    void afficherPage(Page page);
    void afficherPageAccueil();
    void afficherPageEtatDistributeur();
    void afficherPageInterventionDistributeur();
    void afficherPageGeolocalisationDistributeur();
};

#endif // IHMJUSTFEED_H
