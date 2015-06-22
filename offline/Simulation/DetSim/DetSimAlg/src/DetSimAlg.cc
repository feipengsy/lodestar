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

    this->initG4Setting();
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

void DetSimAlg::initG4Setting()
{

}
