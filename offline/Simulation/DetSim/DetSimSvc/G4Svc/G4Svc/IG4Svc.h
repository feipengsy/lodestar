#ifndef I_G4_SVC_H
#define I_G4_SVC_H

class G4RunManager;

class IG4Svc {

    public:
        virtual ~IG4Svc() {}
        virtual bool G4Init() = 0;
        virtual bool SimulateEvent(int i) = 0;
        virtual G4RunManager* runMgr() = 0;

};

#endif
