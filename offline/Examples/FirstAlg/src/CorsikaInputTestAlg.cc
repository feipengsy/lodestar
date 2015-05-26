#include "CorsikaInputTestAlg.h"
#include "MemoryMgr/EvtDataPtr.h"
#include "MemoryMgr/IMemoryMgr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Event/CorsikaHeader.h"
#include "Event/KM2ASimHeader.h"
#include "Event/KM2ASimEvent.h"

using namespace LHAASO;

DECLARE_ALGORITHM(CorsikaInputTestAlg);

CorsikaInputTestAlg::CorsikaInputTestAlg(const std::string& name)
    : AlgBase(name)
{

}

CorsikaInputTestAlg::~CorsikaInputTestAlg()
{

}

bool CorsikaInputTestAlg::initialize()
{
    return true;
}

bool CorsikaInputTestAlg::execute()
{
    ++m_iEvt;
    EvtDataPtr<CorsikaHeader> edp(getScope(), "/Event/CorsikaEvent");
    CorsikaHeader* header = edp.data();
    CorsikaEvent* event = header->event();
    LogInfo << "Header: " << header << std::endl;
    LogInfo << "Event: " << event << std::endl;
    LogInfo << "RunID: " << header->RunID() << " EventID: " << header->EventID() << std::endl;
    std::vector<CorsikaParticle*> particles = event->particle();
    //for (size_t i = 0; i < particles.size(); ++i) {
    //    LogInfo << particles[i]->px() << std::endl;
    //}
    KM2ASimHeader* sheader = new KM2ASimHeader;
    KM2ASimEvent* sevent = new KM2ASimEvent;
    sheader->setEvent(sevent);
    SniperPtr<IMemoryMgr> mm(getScope(), "DataBufferMgr");
    mm->adopt(sheader, "/Event");
    return true;
}

bool CorsikaInputTestAlg::finalize()
{
    return true;
}
