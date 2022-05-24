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




--- Liste des distributeurs



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



-- Contenu détaillé d'un distributeur

SELECT Distributeur.libelle,Distributeur.ville,Distributeur.deviceID,ServeurTTN.applicationID,Produit.designation,NiveauApprovisionnement.libelle,StockDistributeur.numeroBac,StockDistributeur.quantiteMax FROM StockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur=StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur.idNiveauApprovisionnement
INNER JOIN ServeurTTN ON ServeurTTN.idServeurTTN=Distributeur.idServeurTTN
WHERE StockDistributeur.idDistributeur='1';

SELECT Distributeur.libelle,Distributeur.description,Distributeur.ville,Distributeur.deviceID,Produit.designation,NiveauApprovisionnement.libelle,StockDistributeur.numeroBac,StockDistributeur.quantiteMax FROM StockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur=StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur.idNiveauApprovisionnement
INNER JOIN ServeurTTN ON ServeurTTN.idServeurTTN=Distributeur.idServeurTTN
WHERE StockDistributeur.idDistributeur='3';

Résultats :

Place Centrale	Distributeur de riz	Orange	distributeur_3	Riz Long Parfumé Dragon	presque vide	1	2500
Place Centrale	Distributeur de riz	Orange	distributeur_3	Riz Long Basmati		presque vide	2	2500

--------------------------------

SELECT Distributeur.*,Produit.designation,NiveauApprovisionnement.libelle AS niveauApprovisionnement,StockDistributeur.numeroBac,StockDistributeur.quantite,StockDistributeur.quantiteMax FROM StockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur=StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit=StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement=StockDistributeur.idNiveauApprovisionnement
INNER JOIN ServeurTTN ON ServeurTTN.idServeurTTN=Distributeur.idServeurTTN;

1	1	Grand Frais Orange	Distributeur de céréales	Zone du Coudoulet Rond point du Péage Sud	Orange	84100	2022-01-08	4.86741	44.12331	distributeur_1	2	Lion	plein	1	2000	2000
1	1	Grand Frais Orange	Distributeur de céréales	Zone du Coudoulet Rond point du Péage Sud	Orange	84100	2022-01-08	4.86741	44.12331	distributeur_1	2	Cookie Crisp	presque vide	2	200	2000
2	1	Carrefour	Distributeur de pâtes	390 Rue Jean Marie Tjibaou	Avignon	84000	2022-01-09	4.79247	43.92844	distributeur_2	2	Panzani Farfalle	plein	1	2900	3000
2	1	Carrefour	Distributeur de pâtes	390 Rue Jean Marie Tjibaou	Avignon	84000	2022-01-09	4.79247	43.92844	distributeur_2	2	Panzani Marcaroni	plein	2	2500	3000
3	1	Cosy Primers	Distributeur de riz	292 Route de Boulbon	Barbentane	13570	2022-01-10	4.75279	43.90265	distributeur_3	2	Riz Long Parfumé Dragon	presque vide	1	100	2500
3	1	Cosy Primers	Distributeur de riz	292 Route de Boulbon	Barbentane	13570	2022-01-10	4.75279	43.90265	distributeur_3	2	Riz Long Basmati	presque vide	2	500	2500

--------------------------------

SELECT StockDistributeur.quantiteMax, StockDistributeur.numeroBac, Distributeur.libelle, Distributeur.nbBacs, Produit.designation, NiveauApprovisionnement.libelle FROM StockDistributeur
INNER JOIN Distributeur ON Distributeur.idDistributeur = StockDistributeur.idDistributeur
INNER JOIN Produit ON Produit.idProduit = StockDistributeur.idProduit
INNER JOIN NiveauApprovisionnement ON NiveauApprovisionnement.idNiveauApprovisionnement = StockDistributeur.idNiveauApprovisionnement
WHERE idStockDistributeur=3;

-- Une intervention



|dateIntervention|nom|prenom|libelle|ville|deviceID|designation|libelle|
|---|---|---|---|---|---|---|---|
|2022-03-01|VAIRA|Thierry|CAFE_LASALLE_1|Avignon|distributeur_1|Lucaffé Classic|presque vide|
|2022-03-01|VAIRA|Thierry|CAFE_SAINTLOUIS_1|Orange|distributeur_3|Perleo Espresso Cremoso|presque vide|
|2022-03-01|VAIRA|Thierry|CAFE_SAINTLOUIS_1|Orange|distributeur_3|Lucaffé Classic|presque vide|
