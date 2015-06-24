#include "DetSimAlg.h"

DetSimAlg::DetSimAlg(const std::string& name)
    : AlgBase(name)
    , m_iEvt(0)
    , m_g4Svc(0)
    , m_detSimFactory(0)
{
}

DetSimAlg::~DetSimAlg()
{
}

bool DetSimAlg::initialize()
{
    // Try to load the G4RunManager
    SniperPtr<IG4Svc> g4Svc(getScope(), m_g4Svc_name);
    if (g4Svc.invalid()) {
        LogError << "Failed to get the G4Svc, please check the configuration"
                 << std::endl;
        return false;
    }
    m_g4Svc = g4Svc.data();

    // Try to load the DetSimFactory
    SniperPtr<IDetSimFactory> detSimFactory(getScope(), m_det_factory_name);
    if (detSimFactory.invalid()) {
        LogError << "Failed to get the DetSimFactory, please check the configuration"
                 << std::endl;
        return false;
    }
    m_detSimFactory = detSimFactory.data();

    this->initG4Settings();
    if (!m_g4Svc->G4Init()) {
        return false;
    }

    LogDebug << "Successfully initialized" 
             << std::endl; 
    return true;
}

bool DetSimAlg::execute()
{
    LogInfo << "Simulating event: " << m_iEvt
            << std::endl;
    if (!m_g4Svc->SimulateEvent(m_iEvt)) {
        return false;
    }
    ++ m_iEvt;
    return true;
}

bool DetSimAlg::finalize()
{
    LogDebug << "Successfully finalized"
             << std::endl;

    return true;
}

void DetSimAlg::initG4Settings()
{
    G4VUserDetectorConstruction* detector = det_factory->createDetectorConstruction();
    run_manager->SetUserInitialization(detector);

    G4VUserPhysicsList* physics = det_factory->createPhysicsList();
    run_manager->SetUserInitialization(physics);

    G4VUserPrimaryGeneratorAction* gen_action = det_factory->createPrimaryGenerator();
    run_manager->SetUserAction(gen_action);

    G4UserRunAction* run_action = det_factory->createRunAction();
    run_manager->SetUserAction(run_action);

    G4UserEventAction* event_action = det_factory->createEventAction();
    run_manager->SetUserAction(event_action);

    G4UserStackingAction* stacking_action = det_factory->createStackingAction();
    run_manager->SetUserAction(stacking_action);

    G4UserTrackingAction* tracking_action = det_factory->createTrackingAction();
    run_manager->SetUserAction(tracking_action);

    G4UserSteppingAction* stepping_action = det_factory->createSteppingAction();
    run_manager->SetUserAction(stepping_action);
}
