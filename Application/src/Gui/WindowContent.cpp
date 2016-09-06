/*========================================================================
Nom: ContenuFenetre.cpp       Auteur: Manuel Chataigner
Maj: 21/05/2015            Creation: 21/05/2015
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe ContenuFenetre.
=========================================================================*/

#include "../../include/Gui/WindowContent.h"
#include "../../include/Gui/Logger.h"

#include <QScrollBar>


namespace gui
{
    WindowContent::WindowContent(game::Game *game) : QWidget(), m_game(game)
    {
        Logger::createInstance(this);

        m_logs = new QTextEdit;
    }

    WindowContent::~WindowContent()
    {
        Logger::deleteInstance();
    }

    void WindowContent::addLogs(QString content)
    {
        m_logs->setText(m_logs->toPlainText() + content + "\n");

        // Scroll automatique vers la fin
        QScrollBar *sb = m_logs->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}
