SOURCES += \
    src/Game/Action.cpp \
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
    src/Gui/CustomPlot/qcustomplot.cpp \
    src/Gui/AiWindowContent.cpp \
    src/Gui/CalibrationChoice.cpp \
    src/Gui/CardsDialog.cpp \
    src/Gui/CardsList.cpp \
    src/Gui/ClickableCard.cpp \
    src/Gui/GraphicCard.cpp \
    src/Gui/HumanWindowContent.cpp \
    src/Gui/Logger.cpp \
    src/Gui/main.cpp \
    src/Gui/OptionsDialog.cpp \
    src/Gui/TokensCounter.cpp \
    src/Gui/Window.cpp \
    src/Gui/WindowContent.cpp


QT+=widgets printsupport

HEADERS += \
    include/Constantes.h \
    include/Game/Action.h \
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
    include/Game/Player.h \
    include/Game/Game.h \
    include/Game/HumanPlayer.h \
    include/Gui/CustomPlot/qcustomplot.h \
    include/Gui/AiWindowContent.h \
    include/Gui/CalibrationChoice.h \
    include/Gui/CardsDialog.h \
    include/Gui/CardsList.h \
    include/Gui/ClickableCard.h \
    include/Gui/GraphicCard.h \
    include/Gui/HumanWindowContent.h \
    include/Gui/Logger.h \
    include/Gui/OptionsDialog.h \
    include/Gui/TokensCounter.h \
    include/Gui/Window.h \
    include/Gui/WindowContent.h

CONFIG += c++14
!win32 {
    QMAKE_CXX = g++-4.9
}
QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = moc_dir
OBJECTS_DIR = obj_dir
