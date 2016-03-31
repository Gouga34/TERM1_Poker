SOURCES += \
    src/Interface/main.cpp \
    src/Interface/Fenetre.cpp \
    src/Interface/CarteGraphique.cpp \
    src/Interface/ListeCartes.cpp \
    src/Jeu/Jeu.cpp \
    src/Jeu/Joueur.cpp \
    src/Interface/CartesDialog.cpp \
    src/Interface/CarteCliquable.cpp \
    src/Interface/ChoixOptionsDialog.cpp \
    src/Interface/CompteurJetons.cpp \
    src/Interface/Logger.cpp \
    src/IA/EstimationProba.cpp \
    src/IA/IntelligenceArtificielle.cpp \
    src/IA/Resolveur.cpp \
    src/Profilage/ScenariosDeTests.cpp \
    src/IA/IntelligenceArtificielleProfilage.cpp \
    src/Jeu/Action.cpp \
    src/Jeu/JoueurHumain.cpp \
    src/IA/CalibrageIdeal.cpp \
    src/Interface/ContenuFenetre.cpp \
    src/Interface/ContenuFenetreHumain.cpp \
    src/Interface/ContenuFenetreIA.cpp \
    src/Interface/ChoixCalibrage.cpp \
    src/Interface/CustomPlot/qcustomplot.cpp \
    src/Assessor/Assessor.cpp \
    src/Assessor/PlayerCards.cpp \
    src/MathematicalFormula/MathematicalFormula.cpp \
    src/Profilage/CalculateProfilingData.cpp \
    src/Profilage/GamePhase.cpp \
    src/Jeu/Card.cpp \
    src/Profilage/Profile.cpp \
    src/Profilage/Profiling.cpp


QT+=widgets printsupport

HEADERS += \
    include/Interface/Fenetre.h \
    include/Interface/CarteGraphique.h \
    include/Interface/ListeCartes.h \
    include/Jeu/Jeu.h \
    include/Jeu/Joueur.h \
    include/Constantes.h \
    include/Interface/CartesDialog.h \
    include/Interface/CarteCliquable.h \
    include/Interface/ChoixOptionsDialog.h \
    include/Interface/CompteurJetons.h \
    include/Interface/Logger.h \
    include/IA/EstimationProba.h \
    include/IA/IntelligenceArtificielle.h \
    include/IA/Resolveur.h \
    include/Profilage/ScenariosDeTests.h \
    include/IA/IntelligenceArtificielleProfilage.h \
    include/Jeu/Action.h \
    include/Jeu/JoueurHumain.h \
    include/IA/CalibrageIdeal.h \
    include/Interface/ContenuFenetre.h \
    include/Interface/ContenuFenetreHumain.h \
    include/Interface/ContenuFenetreIA.h \
    include/Interface/ChoixCalibrage.h \
    include/Interface/CustomPlot/qcustomplot.h \
    include/Assessor/Assessor.h \
    include/Assessor/PlayerCards.h \
    include/MathematicalFormula/MathematicalFormula.h \
    include/Profilage/CalculateProfilingData.h \
    include/Profilage/GamePhase.h \
    include/Jeu/Card.h \
    include/Profilage/Profile.h \
    include/Profilage/Profiling.h

CONFIG += c++14
QMAKE_CXX = g++-4.9
QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
