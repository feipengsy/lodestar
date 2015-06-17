#ifndef I_G4_SVC_H
#define I_G4_SVC_H

class G4RunManager;

class IG4Svc {

    public:
        virtual ~IG4Svc() {}
        virtual G4RunManager* runMgr() = 0;

};

#endif
