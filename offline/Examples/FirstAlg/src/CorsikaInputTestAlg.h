#ifndef CORSIKA_INPUT_TEST_ALG_H
#define CORSIKA_INPUT_TEST_ALG_H

#include "SniperKernel/AlgBase.h"
#include <string>

class CorsikaInputTestAlg : public AlgBase {

    public:
        CorsikaInputTestAlg(const std::string& name);
        ~CorsikaInputTestAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int m_iEvt;


};


#endif
