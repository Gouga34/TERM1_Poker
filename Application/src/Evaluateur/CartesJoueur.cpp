/*========================================================================
Nom: Evaluateur.h       Auteur: Morgane Vidal
Maj: 09/03/2015          Creation: 04/03/2015  
Projet: Profilage par essais et erreurs au poker
--------------------------------------------------------------------------
Specification: Fichier contenant les définitions de la classe Evaluateur.
=========================================================================*/

#include "../../include/Evaluateur/CartesJoueur.h"
#include "../../include/Constantes.h"

CartesJoueur::CartesJoueur(std::vector<Carte> table, std::vector<Carte> mainJoueur) : m_occurrences(), m_poids(0) {
    initialiserOccurrences();
	remplirTableau(table,mainJoueur);
    calculCombinaison();
}

CartesJoueur::~CartesJoueur() {

}

FORCE_MAIN CartesJoueur::getCombinaison() const {
    return m_combinaison;
}

int CartesJoueur::getPoids() const {
    return m_poids;
}

void CartesJoueur::setCombinaison(FORCE_MAIN combi) {
    m_combinaison = combi;
}

void CartesJoueur::setPoids(int p) {
    m_poids = p;
}

void CartesJoueur::initialiserOccurrences() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 5; ++j) {
            m_occurrences[i][j] = 0;
        }
    }
}

void CartesJoueur::remplirTableau(std::vector<Carte> table, std::vector<Carte> mainJoueur) {
    enregistrerOccurrencesCartes(mainJoueur);
    enregistrerOccurrencesCartes(table);
}

void CartesJoueur::enregistrerOccurrencesCartes(std::vector<Carte> listeCartes) {
    for (auto carte : listeCartes) {
        int rangCarte = carte.getRang();
        int couleurCarte = carte.getCouleur();

        if (rangCarte == AS) {
            ++m_occurrences[0][couleurCarte];
            ++m_occurrences[13][couleurCarte];

            ++m_occurrences[0][4];
            ++m_occurrences[13][4];
        }
        else {
            ++m_occurrences[rangCarte-1][couleurCarte];
            ++m_occurrences[rangCarte-1][4];
        }
        ++m_occurrences[14][couleurCarte];
    }
}

bool CartesJoueur::estSuite(int depart, int ligne) const {
    int i = depart;

    while (i < 14 && m_occurrences[i][ligne] >= 1 && (depart + 5) > i) {
        ++i;
	}

    int fin = depart +5;

    return (fin == i);
}

bool CartesJoueur::contientSuite(int ligne) {
    int i = 0;
    int cpt = 0;

    while (i < 14 && cpt < 5){
        if (m_occurrences[i][ligne] >= 1) {
            ++cpt;
		}
        else {
            cpt = 0;
		}
        ++i;
	}

    if (cpt == 5) {
		setPoids(i);
	}

    return (cpt == 5);
}

int CartesJoueur::cartesIdentiques(int nb, int nbfois) const {
    int poids = 0;
    int cpt = 0;

    for (int i = 13; i > 0; --i){
        if (m_occurrences[i][4] == nb) {
            ++cpt;

            if (poids == 0) {
                poids = i;
            }

            if (cpt == nbfois) {
                return poids;
			}
		}
	}
	return -1;
}

bool CartesJoueur::checkIfFlushAndSetCombinaison() {
    for (int i = 0; i < 4; ++i) {
        if (m_occurrences[14][i] >= 5) {
            //Le poids est alors égal à la ligne de la couleur
			//afin de pouvoir trouver plus tard si égalité la carte la plus haute de la couleur 
			//sans avoir à chercher à nouveau quelle couleur c'est
            setPoids(i);
            setCombinaison(COULEUR);

            return true;
		}
	}

	return false;
}

bool CartesJoueur::checkIfQuinteFlushRoyaleAndSetCombinaison() {
    int i = 0;
    while (i < 4 && getCombinaison() == 0) {
        if (m_occurrences[14][i] >= 5) {
            if (estSuite(9,i)) {
                setCombinaison(QUINTE_FLUSH_ROYALE);
                setPoids(14);
                return true;
            }
        }
        ++i;
    }

    return false;
}

bool CartesJoueur::checkIfQuinteFlushAndSetCombinaison() {
    int i = 0;
    while (i < 4 && getCombinaison() == 0) {
        if (m_occurrences[14][i] >= 5 && contientSuite(i)) {
            setCombinaison(QUINTE_FLUSH);
            return true;
        }
        ++i;
    }

    return false;
}

bool CartesJoueur::checkIfCarreAndSetCombinaison() {
    int carte = -1;

    if ((carte = cartesIdentiques(4)) > 0) {
        setCombinaison(CARRE);
        setPoids(carte);
        carte = 0;
    }
    return (carte != -1);
}

bool CartesJoueur::checkIfFullAndSetCombinaison() {
    int carte;

    if ((carte = cartesIdentiques(3)) > 0 && cartesIdentiques(2) > 0) {
        setCombinaison(FULL);
        setPoids(carte);
        return true;
    }

    return false;
}

bool CartesJoueur::checkIfBrelanAndSetCombinaison() {
    int carte;

    if ((carte = cartesIdentiques(3)) > 0) {
        setCombinaison(BRELAN);
        setPoids(carte);
        return true;
    }
    return false;
}

bool CartesJoueur::checkIfDoublePaireAndSetCombinaison() {
    int carte;

    if ((carte = cartesIdentiques(2,2)) > 0) {
        setCombinaison(DOUBLE_PAIRE);
        setPoids(carte);
        return true;
    }
    return false;
}

bool CartesJoueur::checkIfPaireAndSetCombinaison() {
    int carte;

    if ((carte = cartesIdentiques(2,1)) > 0) {
        setCombinaison(PAIRE);
        setPoids(carte);
        return true;
    }

    return false;
}

bool CartesJoueur::checkIfSuiteAndSetCombinaison() {
    if (contientSuite(4)) {
        setCombinaison(SUITE);
        return true;
    }

    return false;
}

void CartesJoueur::calculCombinaison() {

    if (checkIfQuinteFlushRoyaleAndSetCombinaison()) {
        return;
    }

    if (checkIfQuinteFlushAndSetCombinaison()) {
        return;
    }

    if (checkIfCarreAndSetCombinaison()) {
        return;
    }

    if (checkIfFullAndSetCombinaison()) {
        return;
    }

    if (checkIfFlushAndSetCombinaison()) {
        return;
    }

    if (checkIfSuiteAndSetCombinaison()) {
        return;
    }

    if (checkIfBrelanAndSetCombinaison()) {
        return;
    }

    if (checkIfDoublePaireAndSetCombinaison()) {
        return;
    }

    if (checkIfPaireAndSetCombinaison()) {
        return;
    }

    //Si on n'est rentré dans aucun cas,
    //la seule combinaison possible est la carte haute.
    setCombinaison(CARTE_HAUTE);
}


void CartesJoueur::setPoidsCarteLaPlusHaute() {
    int numeroLigne = -1;

    if (getCombinaison() == COULEUR) {
        numeroLigne = getPoids();
    }
    else if (getCombinaison() == CARTE_HAUTE) {
        numeroLigne=4;
    }

    int i = 13;
    int p = 0;

    while (p == 0 && i > 0) {
        if (m_occurrences[i][numeroLigne] > 0) {
            setPoids(i);
            p = i;
        }
        --i;
    }
}

void CartesJoueur::calculerPoidsBasique() {
   setPoidsCarteLaPlusHaute();
}

RESULTAT_PARTIE CartesJoueur::compareEqualFourOfAKind(CartesJoueur hand2) {
    int pMain1 = 0, pMain2 = 0;
    int i = 13;
    //On calcule la carte la plus haute autre que celles du carré pour this
    while (pMain1 == 0 && pMain2 == 0 && i > 0) {
        if (m_occurrences[i][4] < 4 && m_occurrences[i][4] > 0) {
            pMain1 = i;
        }

        if (hand2.m_occurrences[i][4] < 4 && hand2.m_occurrences[i][4] > 0) {
            pMain2 = i;
        }
        --i;
    }

    if (pMain1 > pMain2) {
        return GAGNE;
    }
    else if (pMain1 < pMain2) {
        return PERDU;
    }
    else {
        return EGALITE;
    }
}

RESULTAT_PARTIE CartesJoueur::compareWeights(CartesJoueur hand2) {
    if (getPoids() > hand2.getPoids()) {
        return GAGNE;
    }
    else if (getPoids() < hand2.getPoids()) {
        return PERDU;
    }
    else {
        return EGALITE;
    }
}

RESULTAT_PARTIE CartesJoueur::compareEqualFullHouse(CartesJoueur hand2) {
    setPoids(cartesIdentiques(2));
    hand2.setPoids(hand2.cartesIdentiques(2));

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::compareEqualFlush(CartesJoueur hand2) {
    int i = 13;
    int color = getPoids();

    while (i > 0) {
        hand2.setPoids(0);
        setPoids(0);

        if (m_occurrences[i][color] > 0) {
            setPoids(i);
        }
        if (hand2.m_occurrences[i][color] > 0) {
            hand2.setPoids(i);
        }

        if (getPoids() != hand2.getPoids()) {
            break;
        }

        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::compareThreeOfAKind(CartesJoueur hand2) {
    int i = 13;

    while (i > 0) {
        hand2.setPoids(0);
        setPoids(0);

        if (m_occurrences[i][4] > 0 && m_occurrences[i][4] != 3) {
            setPoids(i);
        }
        if (hand2.m_occurrences[i][4] > 0 && hand2.m_occurrences[i][4] != 3) {
            hand2.setPoids(i);
        }
        if (getPoids() != hand2.getPoids()) {
            break;
        }
        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::compareTwoPair(CartesJoueur hand2) {
    int i = 13;
    int poidsCarte = getPoids();

    //On commence par comparer la seconde paire
    while (i > 0) {
        hand2.setPoids(0);
        setPoids(0);

        if (m_occurrences[i][4] == 2 && i != poidsCarte) {
            setPoids(i);
        }
        if (hand2.m_occurrences[i][4] == 2 && i != poidsCarte) {
            hand2.setPoids(i);
        }
        if(getPoids() != hand2.getPoids()) {
            break;
        }

        --i;
    }

    //Si la seconde paire est la même, on compare la carte la plus haute
    if (getPoids() == hand2.getPoids()) {
        i = 13;
        int poidsCartePaireMain1=getPoids(), poidsCartePaireMain2=hand2.getPoids();

        //On remet les poids à 0 pour compter juste une seule carte.
        setPoids(0);
        hand2.setPoids(0);
        while (i > 0){
            if ((m_occurrences[i][4] == 1 || (i < poidsCartePaireMain1 && m_occurrences[i][4] >= 1))
                    && getPoids() == 0) {
                setPoids(i);
            }
            if ((hand2.m_occurrences[i][4] == 1 || (i < poidsCartePaireMain2 && m_occurrences[i][4] >= 1))
                    && hand2.getPoids() == 0) {
                hand2.setPoids(i);
            }
            if (getPoids() > 0 || hand2.getPoids() > 0){ //si au moins un nouveau poids, on arrête
                break;
            }
            --i;
        }
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::comparePair(CartesJoueur hand2) {
    int i = 13;
    while (i > 0) {
        hand2.setPoids(0);
        setPoids(0);

        if (m_occurrences[i][4] == 1) {
            setPoids(i);
        }
        if (hand2.m_occurrences[i][4] == 1) {
            hand2.setPoids(i);
        }

        if (getPoids() != hand2.getPoids()) {
            break;
        }
        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::compareHightCard(CartesJoueur hand2) {
    int i = getPoids();

    while (i > 0) {
        hand2.setPoids(0);
        setPoids(0);

        if (m_occurrences[i][4] == 1) {
            setPoids(i);
        }
        if (hand2.m_occurrences[i][4] == 1) {
            hand2.setPoids(i);
        }

        if(getPoids() != hand2.getPoids()) {
            break;
        }

        --i;
    }

    return compareWeights(hand2);
}

RESULTAT_PARTIE CartesJoueur::comparerCombinaisonsEgales(CartesJoueur main2) {

    if (getPoids() == 0) {
 		calculerPoidsBasique();
 	}
    if (main2.getPoids() == 0) {
 		main2.calculerPoidsBasique();
 	}

    if (getPoids() == main2.getPoids()) {
        if (getCombinaison() == CARRE) {
           return compareEqualFourOfAKind(main2);
 		}

        else if (getCombinaison() == FULL) {
           return compareEqualFullHouse(main2);
 		}
        else if (getCombinaison() == COULEUR) {
           return compareEqualFlush(main2);
 		}
        else if (getCombinaison() == BRELAN) {
            return compareThreeOfAKind(main2);
 		}
        else if (getCombinaison() == DOUBLE_PAIRE) {
           return compareTwoPair(main2);
 		}
        else if (getCombinaison() == PAIRE) {
           return comparePair(main2);
 		}
        else {
           return compareHightCard(main2);
		}
 	}
    else {
        return compareWeights(main2);
    }
 }
