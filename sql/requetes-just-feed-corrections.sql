--- Exemples et exercices d'accompagnement : déterminer les requêtes SQL

--- Liste des niveaux d'approvisionnement

SELECT idNiveauApprovisionnement,libelle FROM NiveauApprovisionnement
WHERE idNiveauApprovisionnement >= '0';

|idNiveauApprovisionnement|libelle|
|---|---|
|0|vide|
|1|presque vide|
|2|plein|

--- Liste des serveurs TTN

SELECT * FROM ServeurTTN;


--- Liste des distributeurs

SELECT * FROM Distributeur;

|idDistributeur|idServeurTTN|libelle|description|adresse|ville|codepostal|dateMiseEnService|longitude|latitude|deviceID|nbBacs|
|---|---|---|---|---|---|---|---|---|---|---|---|
|1|1|CAFE_LASALLE_1|Distributeur de cafés en grains|9 Rue Notre Dame des 7 douleurs|Avignon|84000|2022-01-08|4.8139952|43.9484858|distributeur_1|2|
|2|1|CAFE_STJO_1|Distributeur de cafés en grains|105 Rue Duplessis|Carpentras|84200|2022-01-09|5.0452813|44.0521754|distributeur_2|2|
|3|1|CAFE_SAINTLOUIS_1|Distributeur de cafés en grains|Rue du lycée Saint-Louis|Orange|84100|2022-01-10|4.810775|44.128586|distributeur_3|2|

SELECT Distributeur.libelle,Distributeur.ville,Distributeur.deviceID,ServeurTTN.hostname,ServeurTTN.applicationID FROM Distributeur
INNER JOIN ServeurTTN ON ServeurTTN.idServeurTTN=Distributeur.idServeurTTN;

|libelle|ville|deviceID|hostname|applicationID|
|---|---|---|---|---|
|CAFE_LASALLE_1|Avignon|distributeur_1|eu.thethings.network|just_feed|
|CAFE_STJO_1|Carpentras|distributeur_2|eu.thethings.network|just_feed|
|CAFE_SAINTLOUIS_1|Orange|distributeur_3|eu.thethings.network|just_feed|

--- Nombre de distributeurs

SELECT COUNT(*) AS NbDistributeurs FROM Distributeur;

|NbDistributeurs|
|---|
|3|


-- Liste des produits

SELECT * FROM Produit;

|idProduit|marque|code|designation|
|---|---|---|---|
|1|Lavazza|CG_10001|Lavazza Espresso Italiano|
|2|Lavazza|CG_10002|Lavazza Espresso Barista Perfetto|
|3|Lavazza|CG_10003|Lavazza Qualita Oro|
|4|Perleo|CG_25001|Perleo Espresso Cremoso|
|5|Pellini|CG_26001|Espresso Bar Vivace Pellini|
|6|Lucaffé|CG_50001|Lucaffé Classic|
|7|Lucaffé|CG_50002|Lucaffé Arabica|
|8|Zicaffe|CG_87001|Professional Bar Zicaffe|
|9|Zicaffe|CG_87002|Linea Espresso Zicaffe|

-- Stocks

SELECT * FROM StockDistributeur;

-- Contenu détaillé d'un distributeur

SELECT Distributeur.libelle,Distributeur.ville,Distributeur.deviceID,ServeurTTN.applicationID,Produit.designation,NiveauApprovisionnement.libelle FROM StockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur=StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur.idNiveauApprovisionnement
INNER JOIN ServeurTTN ON ServeurTTN.idServeurTTN=Distributeur.idServeurTTN
WHERE StockDistributeur.idDistributeur='1';

|libelle|ville|deviceID|applicationID|designation|libelle|
|---|---|---|---|---|---|
|CAFE_LASALLE_1|Avignon|distributeur_1|just_feed|Lavazza Qualita Oro|plein|
|CAFE_LASALLE_1|Avignon|distributeur_1|just_feed|Lucaffé Classic|presque vide|

-- Une intervention

SELECT Intervention.dateIntervention,Technicien.nom,Technicien.prenom,Distributeur.libelle,Distributeur.ville,Distributeur.deviceID,Produit.designation,NiveauApprovisionnement.libelle FROM Intervention
INNER JOIN Technicien ON Technicien.idTechnicien=Intervention.idTechnicien
INNER JOIN Approvisionnement ON Approvisionnement.idIntervention=Intervention.idIntervention
INNER JOIN StockDistributeur ON StockDistributeur.idStockDistributeur=Approvisionnement.idStockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur=StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur.idNiveauApprovisionnement;

|dateIntervention|nom|prenom|libelle|ville|deviceID|designation|libelle|
|---|---|---|---|---|---|---|---|
|2022-03-01|VAIRA|Thierry|CAFE_LASALLE_1|Avignon|distributeur_1|Lucaffé Classic|presque vide|
|2022-03-01|VAIRA|Thierry|CAFE_SAINTLOUIS_1|Orange|distributeur_3|Perleo Espresso Cremoso|presque vide|
|2022-03-01|VAIRA|Thierry|CAFE_SAINTLOUIS_1|Orange|distributeur_3|Lucaffé Classic|presque vide|
