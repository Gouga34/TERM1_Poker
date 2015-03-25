#include "CalculProfilage.h"
#include "../../include/Constantes.h"
CalculProfilage::CalculProfilage(){





}

std::vector<int> CalculProfilage::getBornesRationalite(double estimation){

    std::vector<int> bornes;

    for(int i=0; i< RATIONALITE::NB_PALIERS; i++){

        if( estimation >= RATIONALITE::PALIER1::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER1::FIN_GAIN ){
            bornes.push_back(RATIONALITE::PALIER1::DEBUT_GAIN);
            bornes.push_back(RATIONALITE::PALIER1::FIN_GAIN);
            bornes.push_back(RATIONALITE::PALIER1::DEBUT_RA_THEORIQUE);
            bornes.push_back(RATIONALITE::PALIER1::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALIER2::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER2::FIN_GAIN ){
            bornes.push_back(RATIONALITE::PALIER2::DEBUT_GAIN);
            bornes.push_back(RATIONALITE::PALIER2::FIN_GAIN);
            bornes.push_back(RATIONALITE::PALIER2::DEBUT_RA_THEORIQUE);
            bornes.push_back(RATIONALITE::PALIER2::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALIER3::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER3::FIN_GAIN ){
            bornes.push_back(RATIONALITE::PALIER3::DEBUT_GAIN);
            bornes.push_back(RATIONALITE::PALIER3::FIN_GAIN);
            bornes.push_back(RATIONALITE::PALIER3::DEBUT_RA_THEORIQUE);
            bornes.push_back(RATIONALITE::PALIER3::FIN_RA_THEORIQUE);

            return bornes;
        }else if ( estimation >= RATIONALITE::PALIER4::DEBUT_GAIN &&  estimation <= RATIONALITE::PALIER4::FIN_GAIN ){
            bornes.push_back(RATIONALITE::PALIER4::DEBUT_GAIN); //g1
            bornes.push_back(RATIONALITE::PALIER4::FIN_GAIN);  //g2
            bornes.push_back(RATIONALITE::PALIER4::DEBUT_RA_THEORIQUE); //m1
            bornes.push_back(RATIONALITE::PALIER4::FIN_RA_THEORIQUE); //m2

            return bornes;
        }
    }
    return bornes;
}

double CalculProfilage::calculMiseTheorique(double estimation){

    std::vector<int> bornes = this->getBornesRationalite(estimation);

    return ( ( ((double) estimation-bornes.at(0) ) * ( ((double) bornes.at(3)-bornes.at(2)) /((double)bornes.at(1)-bornes.at(0)) )) + (double)bornes.at(2) ) ;

}


