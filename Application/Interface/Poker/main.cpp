#include <QApplication>
#include "Fenetre.h"
#include "../../include/Jeu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Jeu *jeu = new Jeu(2, 20, 1000, 0);

    Fenetre fenetre(jeu);
    fenetre.show();

    return app.exec();
}
