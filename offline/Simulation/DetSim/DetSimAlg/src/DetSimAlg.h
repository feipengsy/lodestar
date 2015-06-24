#ifndef DET_SIM_ALG_H
#define DET_SIM_ALG_H

#include "SniperKernel/AlgBase.h"
#include <string>

class IG4Svc;
class IDetSimFactory;

class DetSimAlg : public AlgBase {

    public:
        DetSimAlg(const std::string& name);
        ~DetSimAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        void initG4Settings();

        int               m_iEvt;
        IG4Svc*           m_g4Svc;
        IDetSimFactory*   m_detSimFactory;
};

#endif
