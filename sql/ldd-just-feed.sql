--- LDD (langage de définition de données)

--- Supprime les tables

DROP TABLE IF EXISTS Intervention;
DROP TABLE IF EXISTS Approvisionnement;
DROP TABLE IF EXISTS StockDistributeur;
DROP TABLE IF EXISTS NiveauApprovisionnement;
DROP TABLE IF EXISTS Produit;
DROP TABLE IF EXISTS Distributeur;
DROP TABLE IF EXISTS Technicien;
DROP TABLE IF EXISTS ServeurTTN;

--- Création des tables

-- Structure de la table ServeurTTN

CREATE TABLE IF NOT EXISTS ServeurTTN(
    idServeurTTN INTEGER PRIMARY KEY AUTOINCREMENT,
    hostname VARCHAR NOT NULL,
    port INTEGER NOT NULL DEFAULT 1883,
    username VARCHAR DEFAULT NULL,
    `password` VARCHAR DEFAULT NULL,
    applicationID VARCHAR NOT NULL,
    estActif INTEGER DEFAULT 1
);

-- Structure de la table Technicien

CREATE TABLE IF NOT EXISTS Technicien (
  idTechnicien INTEGER PRIMARY KEY AUTOINCREMENT,
  nom varchar(64) NOT NULL,
  prenom varchar(64) NOT NULL,
  identifiant varchar(255) DEFAULT NULL,
  email varchar(64) NOT NULL
);

-- Structure de la table Distributeur

CREATE TABLE IF NOT EXISTS Distributeur(
    idDistributeur INTEGER PRIMARY KEY AUTOINCREMENT,
    idServeurTTN INTEGER NOT NULL,
    libelle VARCHAR,
    description VARCHAR,
    adresse VARCHAR,
    ville VARCHAR,
    codepostal VARCHAR(5),
    dateMiseEnService DATE NOT NULL,
    longitude VARCHAR,    
    latitude VARCHAR,
    deviceID VARCHAR NOT NULL,
    hygrometrie INTEGER DEFAULT 0,
    nbBacs INTEGER NOT NULL DEFAULT 2,
    UNIQUE (deviceID),
    CONSTRAINT Distributeur_fk_1 FOREIGN KEY (idServeurTTN) REFERENCES ServeurTTN(idServeurTTN) ON DELETE CASCADE
);

-- Structure de la table Produit

CREATE TABLE IF NOT EXISTS Produit (
  idProduit INTEGER PRIMARY KEY AUTOINCREMENT,
  marque VARCHAR NOT NULL,
  code VARCHAR,
  designation VARCHAR
);

-- Structure de la table NiveauApprovisionnement

CREATE TABLE IF NOT EXISTS NiveauApprovisionnement(idNiveauApprovisionnement INTEGER PRIMARY KEY, niveau VARCHAR);

-- Structure de la table StockDistributeur

CREATE TABLE IF NOT EXISTS StockDistributeur (
  idStockDistributeur INTEGER PRIMARY KEY AUTOINCREMENT,
  idDistributeur INTEGER NOT NULL,
  idProduit INTEGER NOT NULL,
  idNiveauApprovisionnement INTEGER NOT NULL,
  quantite INTEGER DEFAULT 0,
  quantiteMax INTEGER DEFAULT 0,
  numeroBac INTEGER NOT NULL,
  CONSTRAINT Unique_Bac UNIQUE (idDistributeur,numeroBac),
  CONSTRAINT Stock_fk_1 FOREIGN KEY (idDistributeur) REFERENCES Distributeur(idDistributeur) ON DELETE CASCADE,
  CONSTRAINT Stock_fk_2 FOREIGN KEY (idProduit) REFERENCES Produit(idProduit) ON DELETE CASCADE
);

--- Voir aussi :
--- ON DELETE CASCADE

-- Structure de la table Intervention

CREATE TABLE IF NOT EXISTS Intervention (
  idIntervention INTEGER PRIMARY KEY AUTOINCREMENT,
  idTechnicien INTEGER NOT NULL,
  dateIntervention DATE NOT NULL,
  effectuee INTEGER DEFAULT 0,
  CONSTRAINT Intervention_fk_1 FOREIGN KEY (idTechnicien) REFERENCES Technicien(idTechnicien) ON DELETE CASCADE
);

-- Structure de la table Approvisionnement

CREATE TABLE IF NOT EXISTS Approvisionnement(
    idApprovisionnement INTEGER PRIMARY KEY AUTOINCREMENT,
    idIntervention INTEGER NOT NULL,
    idStockDistributeur INTEGER NOT NULL,
    dateApprovisionnement DATE NOT NULL,
    effectue INTEGER DEFAULT 0,
    CONSTRAINT Approvisionnement_fk_1 FOREIGN KEY (idIntervention) REFERENCES Intervention(idIntervention) ON DELETE CASCADE,
    CONSTRAINT Approvisionnement_fk_2 FOREIGN KEY (idStockDistributeur) REFERENCES StockDistributeur(idStockDistributeur) ON DELETE CASCADE
);
