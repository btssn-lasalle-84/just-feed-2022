#ifndef DISTRIBUTEUR_H
#define DISTRIBUTEUR_H

#include <QString>

#define NB_DISTRIBUTEURS_DEFAUT 3

#define SEUIL_REMPLISSAGE_DEFAUT 30

class Distributeur
{
  private:
  public:
    Distributeur();

    /**
     * @enum ChampDistributeur
     * brief Définit les différentes champs de la table Distributeur
     */
    enum ChampDistributeur
    {
        CHAMP_idDistributeur,
        CHAMP_idServeurTTN,
        CHAMP_libelle,
        CHAMP_description,
        CHAMP_adresse,
        CHAMP_ville,
        CHAMP_codepostal,
        CHAMP_dateMiseEnService,
        CHAMP_latitude,
        CHAMP_longitude,
        CHAMP_deviceID,
        CHAMP_hygrometrie,
        CHAMP_nbBacs,
        CHAMP_designationProduit,
        CHAMP_niveauApprovisionnement,
        CHAMP_numeroBac,
        CHAMP_quantite,
        CHAMP_quantiteMax,
        NB_CHAMPS_DISTRIBUTEUR
    };
};

#endif // DISTRIBUTEUR_H
