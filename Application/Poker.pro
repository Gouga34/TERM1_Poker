SOURCES += \
    src/Interface/main.cpp \
    src/Interface/Fenetre.cpp \
    src/Interface/CarteGraphique.cpp \
    src/Interface/ListeCartes.cpp \
    src/Jeu/Jeu.cpp \
    src/Jeu/Carte.cpp \
    src/Jeu/Joueur.cpp \
    src/Jeu/IntelligenceArtificielle.cpp \
    src/Profilage/Profilage.cpp \
    src/Profilage.CalculDonneesProfilage.cpp \
    src/Jeu/EstimationProba.cpp \
    src/Evaluateur/Evaluateur.cpp \
    src/Evaluateur/CartesJoueur.cpp \
    src/Interface/CartesDialog.cpp \
    src/Interface/CarteCliquable.cpp \
    src/Interface/ChoixPseudoDialog.cpp \
    src/Interface/CompteurJetons.cpp \
    src/Interface/CalibrageIA.cpp 


QT+=widgets

HEADERS += \
    include/Interface/Fenetre.h \
    include/Interface/CarteGraphique.h \
    include/Interface/ListeCartes.h \
    include/Jeu/Jeu.h \
    include/Jeu/Carte.h \
    include/Jeu/Joueur.h \
    include/Jeu/IntelligenceArtificielle.h \
    include/Profilage/Profilage.h \
    include/Profilage/CalculDonneesProfilage.h \
    include/Jeu/EstimationProba.h \
    include/Constantes.h \
    include/Evaluateur/CartesJoueur.h \
    include/Evaluateur/Evaluateur.h \
    include/Interface/CartesDialog.h \
    include/Interface/CarteCliquable.h \
    include/Interface/ChoixPseudoDialog.h \
    include/Interface/CompteurJetons.h \
    include/Interface/CalibrageIA.h

CONFIG += c++11

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
