/**
  Algorithme de sauvegarde des données de profilage utilisé par l'IA.
  On peut aussi ajouter à cette classe la méthode de lecture d'un fichier.

  http://doc.qt.io/qt-5/qtcore-json-savegame-example.html
*/


class Profilage {

  String joueur;
  
  
  /**
   * Constructeur du profilage
   * @param Nom du joueur profilé
   */
  Profilage(String joueur);
  
  
  /**
   * @action Ajoute les données de profilage de la partie dans le fichier correspondant au joueur
   */
  void save() {
  
    QFile fichier(joueur + ".json");
    fichier.open(ReadWrite);


    QJsonObject partie;
    
    /* Ecriture de valeurs simples */
    
    partie["agressivité"] = tauxAgressivite;
    partie["rationnalite"] = tauxRationnalite;
    partie["passivite"] = tauxPassivite;
    
    
    /* Ecriture de sous-objets (ex : mise {nbMise, mise + haute}) */
    
    QJsonObject mise;
    mise["nbMises"] = mise;
    mise["misePlusHaute"] = maxMise;
    
    partie["mise"] = mise;
    
    
    /* Ecriture tableaux (ex : ajout partie courante à la liste de parties enregistrées */
    
    // Récupération du contenu existant
    QByteArray donneesJson = fichier.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(donneesJson)):
    QJsonObject json(doc.object());
    
    QJsonArray parties = json["parties"].toArray();
    
    parties.append(partie);
    
    // On modifie le QJsonObject
    json["parties"] = parties;
    
    // On modifie le QJsonDocument
    doc.setObject(json);
    
    // On écrit dans le fichier
    fichier.write(doc.toJson());
  }

}
