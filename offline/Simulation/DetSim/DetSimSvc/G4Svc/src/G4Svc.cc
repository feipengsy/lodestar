#include "G4Svc/G4Svc.h"
#include "G4SvcRunManager.h"
#include <boost/foreach.hpp>

G4Svc::G4Svc(const std::string& name)
   : SvcBase(name)
   , IG4Svc()
   , m_runMgr(0)
{
    declProp("RunMac", m_run_mac);
    declProp("RunCmds", m_run_cmds);
    declProp("VisMac", m_vis_mac);
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
    if (!m_runMgr->RunFinalize()) {
        return false;
    }

    delete m_runMgr;
    return true;
}

bool G4Svc::G4Init()
{
    // Run the GUI if vis mac file is set
    if (m_vis_mac.length()) {
        this->G4VisInit();
        return false;
    }

    // Set run mac
    if (!m_run_mac.length()) {
        return false;
    }
    G4UImanager * UImanager = G4UImanager::GetUIpointer();
    std::string command = "/control/execute ";
    UImanager->ApplyCommand( command + m_run_mac);

    // Set run cmds
    G4UImanager * UImanager = G4UImanager::GetUIpointer();
    BOOST_FOREACH(const std::string& cmd, m_run_cmds) {
         UImanager->ApplyCommand(cmd);
    }
    
    m_runMgr->Initialize();
    return m_runMgr->RunInitialize();
}

bool G4Svc::SimulateEvent(int i)
{
    return m_runMgr->SimulateEvent(i);
}

G4SvcRunManager* G4Svc::runMgr()
{
    return m_runMgr;
}

void G4Svc::G4VisInit()
{
    LogInfo << "Start Geant4 Visual" << std::endl;
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    LogInfo << "Initialize Geant4 Vis Manager" << std::endl;

    G4UImanager * UImanager = G4UImanager::GetUIpointer();
    LogInfo << "Get Geant4 UI Manager" << std::endl;

    LogInfo << "Create Geant4 UI Executive" << std::endl;
    char* argv[1] = {"dummy_argv"};
    G4UIExecutive * ui = new G4UIExecutive(1,argv);
    std::string viscmd("/control/execute ");
    viscmd += m_vis_mac;
    LogInfo << "Execute Geant4 command" << viscmd << std::endl;
    UImanager->ApplyCommand(viscmd);

    ui->SessionStart();
    delete ui;
    delete visManager;
}
