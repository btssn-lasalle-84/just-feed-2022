--- LMD (langage de manipulation de données)

--- Contenu des tables (tests)

-- Table ServeurTTN

INSERT INTO ServeurTTN(hostname,port,username,`password`,applicationID,estActif) VALUES('eu.thethings.network',1883,'just_feed','ttn-account-v2.vC-aqMRnLLzGkNjODWgy81kLqzxBPAT8_mE-L7U2C_w','just_feed',0);

-- Table Technicien

INSERT INTO Technicien(nom,prenom,identifiant,email) VALUES('VAIRA','Thierry','tvaira','tvaira@free.fr');
INSERT INTO Technicien(nom,prenom,identifiant,email) VALUES('BEAUMONT','Jérôme','jbeaumont','beaumont@lasalle84.org');

-- Table Distributeur

INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,nbRangees) VALUES(1,'CAFE_LASALLE_1','Distributeur de cafés en grains','9 Rue Notre Dame des 7 douleurs','Avignon', '84000','2022-01-08','4.8139952','43.9484858','distributeur_1',2);
INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,nbRangees) VALUES(1,'CAFE_STJO_1','Distributeur de cafés en grains','105 Rue Duplessis','Carpentras', '84200','2022-01-09','5.0452813','44.0521754','distributeur_1',2);
INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,nbRangees) VALUES(1,'CAFE_SAINTLOUIS_1','Distributeur de cafés en grains','Rue du lycée Saint-Louis','Orange', '84100','2022-01-10','4.810775','44.128586','distributeur_1',2);

-- Table Produit

INSERT INTO Produit (marque,code,designation) VALUES('Lavazza','CG_10001','Lavazza Espresso Italiano');
INSERT INTO Produit (marque,code,designation) VALUES('Lavazza','CG_10002','Lavazza Espresso Barista Perfetto');
INSERT INTO Produit (marque,code,designation) VALUES('Lavazza','CG_10003','Lavazza Qualita Oro');
INSERT INTO Produit (marque,code,designation) VALUES('Perleo','CG_25001','Perleo Espresso Cremoso');
INSERT INTO Produit (marque,code,designation) VALUES('Pellini','CG_26001','Espresso Bar Vivace Pellini');
INSERT INTO Produit (marque,code,designation) VALUES('Lucaffé','CG_50001','Lucaffé Classic');
INSERT INTO Produit (marque,code,designation) VALUES('Lucaffé','CG_50002','Lucaffé Arabica');
INSERT INTO Produit (marque,code,designation) VALUES('Zicaffe','CG_87001','Professional Bar Zicaffe');
INSERT INTO Produit (marque,code,designation) VALUES('Zicaffe','CG_87002','Linea Espresso Zicaffe');

-- Table NiveauApprovisionnement

INSERT INTO NiveauApprovisionnement(idNiveauApprovisionnement,libelle) VALUES
(-1,'inconnu'),
(0,'vide'),
(1,'presque vide'),
(2,'plein');

-- Table StockDistributeur

INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(1,3,2,0,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(1,6,1,0,2);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(2,1,2,0,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(2,2,2,0,2);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(3,4,1,0,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantiteMax,rangee) VALUES(3,6,1,0,2);

-- Table Intervention

INSERT INTO Intervention (idTechnicien,dateIntervention) VALUES(1,'2022-03-01');

-- Table Approvisionnement

INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,2,'2022-03-10');
INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,5,'2022-03-10');
INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,6,'2022-03-10');
