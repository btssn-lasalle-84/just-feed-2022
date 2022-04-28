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

QT_BEGIN_NAMESPACE
namespace Ui
{
class IHMJustFeed;
}
QT_END_NAMESPACE

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
    BaseDeDonnees* BaseDeDonnees; //!< Instance d'un objet BaseDeDonnees
    QStringList nomColonnes; //!< Liste de nom des colonnes
    int nbLignesDistributeurs; //!< nombre de lignes
    QVector<QStringList> distributeurs; //!< Les distributeurs
    QStandardItemModel* modeleDistributeur; //!< Modèle pourle QTableView

    /**
     * @enum Page
     * @brief Définit les différentes fenêtres de l'IHM
     *
     */
    enum Page
    {
        Page1 = 0,
        Page2,
        NbPages
    };

    /**
     * @enum ColonneDistributeur
     * brief Définit les différentes colonne du QTableView
     */
    enum ColonneDistributeur
    {
        COLONNE_DISTRIBUTEUR_NOM //!< Emplacement du nom
        NB_COLONNES
    };

    void initialiser();
    void gererEvenements();
    void ajouterMenuAide();

  public slots:
    void chargerDistributeurs();
    void effacerTableDistributeurs();
    void afficherDistributeurTable(QStringList distributeur);
    void selectionner(QModelIndex index);
    void afficherPage(IHMJUSTFEED_H::Page page);
    void afficherPagePrincipale();
    void afficherAPropos();
};

#endif // IHMJUSTFEED_H
