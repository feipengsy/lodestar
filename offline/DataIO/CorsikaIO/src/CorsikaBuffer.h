#ifndef CORSIKA_BUFFER_H
#define CORSIKA_BUFFER_H

#include <vector>

struct CskRunBuffer {

    int run_number;
    float date;
    float version;
    int nr_obs_level;  
    float obs_level_cm[10];
    float slope;
    float e_min;
    float e_max;
    int egs_flag;
    int nkg_flag;
    float cut_hadr;
    float cut_muon;
    float cut_elec;
    float cut_phot;
    float c[50];
    float cc[20];
    float cka[40];
    float ceta[5];
    float cstrba[11];
    float can[50];
    float cann[50];
    float aatm[5];
    float batm[5];
    float catm[5];
    int nflain;
    int nfldif;
    int nflpi0;
    int nflpi100;
    int muon_mult_flag;
};

struct CskParticleBuffer {

    int particle;
    int generation;
    int obs_level;
    float px;
    float py;
    float pz;
    float x;
    float y;
    float time;
    float height;
    float theta;
    float phi; 
    float theta_deg;
    float phi_deg;
    float p; 
    int inter;
    int number;
    int multiplicity;
    int charge;
    int baryon_number;
};

struct CskEventBuffer {  

    ~CskEventBuffer() {
        for (std::vector<CskParticleBuffer*>::iterator it = Particle.begin(); it != Particle.end(); ++it) {
            delete *it;
        }
    }

    int event_number;
    int particle;
    float energy;
    float start_altitude;
    int target;
    float interaction_height;
    float px;
    float py;
    float pz;
    float theta;
    float phi;
    int sequence_number;
    int seed[10][3];
    int call_offset_high[10];
    int call_offset_low[10];
    int gheisha_flag;
    int venus_flag;
    int cerenkov_flag;
    int neutrino_flag;
    int horizont_flag;
    int computer;
    float theta_min;
    float theta_max;
    float phi_min;
    float phi_max;
    int muon_flag;
    int sybill_flag;
    int sybill_cross_flag;
    int qgsjet_flag;
    int qgsjet_cross_flag;
    int dpmjet_flag;
    int dpmjet_cross_flag;
    int venus_cross_flag;
    int muon_mult_flag;
    int reduced;
    float latitude, longitude; 
    std::vector<CskParticleBuffer*> Particle;
};

#endif
