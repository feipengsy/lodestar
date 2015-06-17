#ifndef G4_SVC_RUN_MANAGER_H
#define G4_SVC_RUN_MANAGER_H

#include "G4RunManager.hh"

class G4SvcRunManager : public G4RunManager {

    public:
        G4SvcRunManager();
        ~G4SvcRunManager();
 
        bool initializeRun();
        bool SimulateEvent(int i);
        bool finalizeRun();
};

#endif
