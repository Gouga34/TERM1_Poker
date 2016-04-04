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
    src/IA/Resolveur.cpp \
    src/Jeu/Action.cpp \
    src/Jeu/JoueurHumain.cpp \
    src/Interface/ContenuFenetre.cpp \
    src/Interface/ContenuFenetreHumain.cpp \
    src/Interface/ContenuFenetreIA.cpp \
    src/Interface/ChoixCalibrage.cpp \
    src/Interface/CustomPlot/qcustomplot.cpp \
    src/Assessor/Assessor.cpp \
    src/Assessor/PlayerCards.cpp \
    src/MathematicalFormula/MathematicalFormula.cpp \
    src/Jeu/Card.cpp \
    src/Profiling/CalculateProfilingData.cpp \
    src/Profiling/GamePhase.cpp \
    src/Profiling/Profile.cpp \
    src/Profiling/Profiling.cpp \
    src/Profiling/TestScenario.cpp \
    src/IA/IdealCalibration.cpp \
    src/IA/WinningChancesEstimator.cpp \
    src/IA/ArtificialIntelligence.cpp \
    src/IA/ArtificialIntelligenceProfiling.cpp


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
    include/IA/Resolveur.h \
    include/Jeu/Action.h \
    include/Jeu/JoueurHumain.h \
    include/Interface/ContenuFenetre.h \
    include/Interface/ContenuFenetreHumain.h \
    include/Interface/ContenuFenetreIA.h \
    include/Interface/ChoixCalibrage.h \
    include/Interface/CustomPlot/qcustomplot.h \
    include/Assessor/Assessor.h \
    include/Assessor/PlayerCards.h \
    include/MathematicalFormula/MathematicalFormula.h \
    include/Jeu/Card.h \
    include/Profiling/CalculateProfilingData.h \
    include/Profiling/GamePhase.h \
    include/Profiling/Profile.h \
    include/Profiling/Profiling.h \
    include/Profiling/TestScenario.h \
    include/IA/IdealCalibration.h \
    include/IA/WinningChancesEstimator.h \
    include/IA/ArtificialIntelligence.h \
    include/IA/ArtificialIntelligenceProfiling.h

CONFIG += c++14
QMAKE_CXX = g++-4.9
QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
