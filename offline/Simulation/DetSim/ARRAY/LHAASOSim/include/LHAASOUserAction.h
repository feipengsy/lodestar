#ifndef LHAASO_SIM_ACTION_H
#define LHAASO_SIM_ACTION_H 1

#include <vector>

class IAnalysisElement;

class LHAASOUserAction {
 
    public:
        LHAASOUserAction() {}
        ~LHAASOUserAction() {}

        void addUserAction(IAnalysisElement* ac) { m_ac.push_back(ac); }

    protected:
        typedef std::vector<IAnalysisElement*> ACVECTOR;
        ACVECTOR m_ac;

};

#endif
