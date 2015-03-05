SOURCES += \
    src/Interface/main.cpp \
    src/Interface/Fenetre.cpp \
    src/Interface/CarteGraphique.cpp \
    src/Interface/ListeCartes.cpp \
    src/Jeu/Jeu.cpp \
    src/Jeu/Carte.cpp \
    src/Jeu/Joueur.cpp \
    src/Jeu/IntelligenceArtificielle.cpp \
    src/Jeu/Profilage.cpp \
    src/Jeu/EstimationProba.cpp \
    src/Evaluateur/Evaluateur.cpp \
    src/Evaluateur/CartesJoueur.cpp

QT+=widgets

HEADERS += \
    include/Interface/Fenetre.h \
    include/Interface/CarteGraphique.h \
    include/Interface/ListeCartes.h \
    include/Jeu/Jeu.h \
    include/Jeu/Carte.h \
    include/Jeu/Joueur.h \
    include/Jeu/IntelligenceArtificielle.h \
    include/Jeu/Profilage.h \
    include/Jeu/EstimationProba.h \
    include/Constantes.h \
    include/Evaluateur/CartesJoueur.h \
    include/Evaluateur/Evaluateur.h

CONFIG += c++11

MOC_DIR = tmp
OBJECTS_DIR = tmp
