#ifndef IHMJUSTFEED_H
#define IHMJUSTFEED_H

/**
 * @file ihmjustfeed.h
 *
 * @brief Déclaration de la classe IHMJustFeed
 * @author
 * @version 1.0
 *
 */

#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui
{
class IHMJustFeed;
}
QT_END_NAMESPACE

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
};

#endif // IHMJUSTFEED_H
