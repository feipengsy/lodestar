#ifndef RW_EXAMPLE_H
#define RW_EXAMPLE_H

#include "Sniperkernel/AlgBase.h"

class RWExample : public AlgBase {

    public:
        RWExample(const std::string& name);
        ~RWExample();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int m_iEvt;      

};


#endif
