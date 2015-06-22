#ifndef G4_SVC_H
#define G4_SVC_H

#include "G4Svc/IG4Svc.h"
#include "SniperKernel/SvcBase.h"
#include <string>
#include <vector>

class G4SvcRunManager;

class G4Svc : public SvcBase, public IG4Svc {

    public:
        G4Svc(const std::string& name);
        ~G4Svc();

        bool initialize();
        bool finalize();
        bool G4Init();
        bool SimulateEvent(int i);
        G4SvcRunManager* runMgr();

    private:
        void G4VisInit(); 
        G4SvcRunManager* m_runMgr;
        std::string m_run_mac;
        std::vector<std::string> m_run_cmds;
        std::string m_vis_mac;

};

#endif
