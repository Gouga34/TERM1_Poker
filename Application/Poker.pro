SOURCES += \
    src/Interface/main.cpp \
    src/Interface/Fenetre.cpp \
    src/Interface/CarteGraphique.cpp \
    src/Interface/ListeCartes.cpp \
    src/Interface/CartesDialog.cpp \
    src/Interface/CarteCliquable.cpp \
    src/Interface/ChoixOptionsDialog.cpp \
    src/Interface/CompteurJetons.cpp \
    src/Interface/Logger.cpp \
    src/Jeu/Action.cpp \
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
    src/AI/IdealCalibration.cpp \
    src/AI/WinningChancesEstimator.cpp \
    src/AI/ArtificialIntelligence.cpp \
    src/AI/ArtificialIntelligenceProfiling.cpp \
    src/AI/Resolver.cpp \
    src/Jeu/HumanPlayer.cpp \
    src/Jeu/Player.cpp \
    src/Jeu/Game.cpp


QT+=widgets printsupport

HEADERS += \
    include/Interface/Fenetre.h \
    include/Interface/CarteGraphique.h \
    include/Interface/ListeCartes.h \
    include/Constantes.h \
    include/Interface/CartesDialog.h \
    include/Interface/CarteCliquable.h \
    include/Interface/ChoixOptionsDialog.h \
    include/Interface/CompteurJetons.h \
    include/Interface/Logger.h \
    include/Jeu/Action.h \
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
    include/AI/IdealCalibration.h \
    include/AI/WinningChancesEstimator.h \
    include/AI/ArtificialIntelligence.h \
    include/AI/ArtificialIntelligenceProfiling.h \
    include/AI/Resolver.h \
    include/Jeu/HumainPlayer.h \
    include/Jeu/Player.h \
    include/Jeu/Game.h

CONFIG += c++14
QMAKE_CXX = g++-4.9
QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
