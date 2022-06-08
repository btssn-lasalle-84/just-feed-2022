--- LMD (langage de manipulation de données)

--- Contenu des tables (tests)

-- Table ServeurTTN

INSERT INTO ServeurTTN(hostname,port,username,`password`,applicationID,estActif) VALUES('eu.thethings.network',1883,'just_feed','ttn-account-v2.vC-aqMRnLLzGkNjODWgy81kLqzxBPAT8_mE-L7U2C_w','just_feed',0);

-- Table Technicien

INSERT INTO Technicien(nom,prenom,identifiant,email) VALUES('VAIRA','Thierry','tvaira','tvaira@free.fr');
INSERT INTO Technicien(nom,prenom,identifiant,email) VALUES('BEAUMONT','Jérôme','jbeaumont','beaumont@lasalle84.org');

-- Table Distributeur

INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,hygrometrie,nbBacs) VALUES(1,'Grand Frais','Distributeur de céréales','Zone du Coudoulet Rond point du Péage Sud','Orange', '84100','2022-01-08','4.84856','44.11161','distributeur-1-sim',10,2);
INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,hygrometrie,nbBacs) VALUES(1,'Carrefour','Distributeur de pâtes','390 Rue Jean Marie Tjibaou','Avignon', '84000','2022-01-09','4.79247','43.92844','distributeur-2-sim',15,2);
INSERT INTO Distributeur(idServeurTTN,libelle,description,adresse,ville,codepostal,dateMiseEnService,longitude,latitude,deviceID,hygrometrie,nbBacs) VALUES(1,'Cosy Primeurs','Distributeur de riz','292 Route de Boulbon','Barbentane', '13570','2022-01-10','4.75280','43.90252','distributeur_3',12,2);

-- Table Produit

INSERT INTO Produit (marque,code,designation) VALUES('Panzani','CG_10001','Panzani Farfalle');
INSERT INTO Produit (marque,code,designation) VALUES('Panzani','CG_10002','Panzani Macaroni');
INSERT INTO Produit (marque,code,designation) VALUES('Panzani','CG_10003','Panzani Fusilli');
INSERT INTO Produit (marque,code,designation) VALUES('Riz du monde','CG_25001','Riz Long Dragon');
INSERT INTO Produit (marque,code,designation) VALUES('Riz du monde','CG_26001','Riz Parfumé Thai');
INSERT INTO Produit (marque,code,designation) VALUES('Riz du monde','CG_50001','Riz Long Basmati');
INSERT INTO Produit (marque,code,designation) VALUES('Nestle','CG_50002','Cookie Crisp');
INSERT INTO Produit (marque,code,designation) VALUES('Nestle','CG_87001','Lion Choco');
INSERT INTO Produit (marque,code,designation) VALUES('Nestle','CG_87002','Chocapic');

-- Table NiveauApprovisionnement

INSERT INTO NiveauApprovisionnement(idNiveauApprovisionnement,libelle) VALUES
(-1,'inconnu'),
(0,'vide'),
(1,'presque vide'),
(2,'plein');

-- Table StockDistributeur

INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(1,8,2,2000,2550,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(1,7,1,200,2550,2);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(2,1,2,2150,2550,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(2,2,2,2500,2550,2);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(3,4,1,100,2550,1);
INSERT INTO StockDistributeur (idDistributeur,idProduit,idNiveauApprovisionnement,quantite,quantiteMax,numeroBac) VALUES(3,6,1,500,2550,2);

-- Table Intervention

INSERT INTO Intervention (idTechnicien,dateIntervention) VALUES(1,'2022-03-01');

-- Table Approvisionnement

INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,2,'2022-03-10');
INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,5,'2022-03-10');
INSERT INTO Approvisionnement(idIntervention,idStockDistributeur,dateApprovisionnement) VALUES(1,6,'2022-03-10');
