SOURCES += \
    src/Interface/main.cpp \
    src/Interface/Fenetre.cpp \
    src/Interface/ListeCartes.cpp \
    src/Interface/CartesDialog.cpp \
    src/Interface/ChoixOptionsDialog.cpp \
    src/Interface/CompteurJetons.cpp \
    src/Interface/Logger.cpp \
    src/Game/Action.cpp \
    src/Interface/ContenuFenetre.cpp \
    src/Interface/ContenuFenetreHumain.cpp \
    src/Interface/ContenuFenetreIA.cpp \
    src/Interface/ChoixCalibrage.cpp \
    src/Interface/CustomPlot/qcustomplot.cpp \
    src/Assessor/Assessor.cpp \
    src/Assessor/PlayerCards.cpp \
    src/MathematicalFormula/MathematicalFormula.cpp \
    src/Game/Card.cpp \
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
    src/Game/HumanPlayer.cpp \
    src/Game/Player.cpp \
    src/Game/Game.cpp \
    src/Interface/GraphicCard.cpp \
    src/Interface/ClickableCard.cpp


QT+=widgets printsupport

HEADERS += \
    include/Interface/Fenetre.h \
    include/Interface/ListeCartes.h \
    include/Constantes.h \
    include/Interface/CartesDialog.h \
    include/Interface/ChoixOptionsDialog.h \
    include/Interface/CompteurJetons.h \
    include/Interface/Logger.h \
    include/Game/Action.h \
    include/Interface/ContenuFenetre.h \
    include/Interface/ContenuFenetreHumain.h \
    include/Interface/ContenuFenetreIA.h \
    include/Interface/ChoixCalibrage.h \
    include/Interface/CustomPlot/qcustomplot.h \
    include/Assessor/Assessor.h \
    include/Assessor/PlayerCards.h \
    include/MathematicalFormula/MathematicalFormula.h \
    include/Game/Card.h \
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
    include/Game/HumainPlayer.h \
    include/Game/Player.h \
    include/Game/Game.h \
    include/Interface/GraphicCard.h \
    include/Interface/ClickableCard.h

CONFIG += c++14
!win32 {
    QMAKE_CXX = g++-4.9
}
QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
