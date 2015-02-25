SOURCES += \
    main.cpp \
    Fenetre.cpp \
    CarteGraphique.cpp \
    ListeCartes.cpp \
    ../../src/Jeu.cpp \
    ../../src/Carte.cpp \
    ../../src/Joueur.cpp \
    ../../src/IntelligenceArtificielle.cpp

QT+=widgets

HEADERS += \
    Fenetre.h \
    CarteGraphique.h \
    ListeCartes.h \
    ../../include/Jeu.h \
    ../../include/Carte.h \
    ../../include/Joueur.h \
    ../../include/IntelligenceArtificielle.h

CONFIG += c++11

MOC_DIR = ../tmp
OBJECTS_DIR = ../tmp
