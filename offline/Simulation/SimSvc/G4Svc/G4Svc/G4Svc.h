#ifndef G4_SVC_H
#define G4_SVC_H

class G4Svc : public SvcBase, public IG4Svc {

    public:
        G4Svc(const std::string& name);
        ~G4Svc();

        bool initialize();
        bool finalize();

        G4SvcRunManager* runMgr();

    private:
        G4SvcRunManager* m_runMgr;

};

#endif
