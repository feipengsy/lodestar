#include "G4Svc/G4Svc.h"
#include "G4SvcRunManager.h"

G4Svc::G4Svc(const std::string& name)
   : SvcBase(name)
   , IG4Svc()
   , m_runMgr(0)
{
}

G4Svc::~G4Svc()
{
}

bool G4Svc::initialize()
{
    m_runMgr = new G4SvcRunManager;

    LogDebug << "Sucessfully initialized." << std::endl;
    return true;
}

bool G4Svc::finalize()
{
    delete m_runMgr;
    return true;
}

G4SvcRunManager* G4Svc::runMgr()
{
    return m_runMgr;
}
