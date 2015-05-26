#include "BeginEvtHdl.h"
#include "MemoryMgr/EvtBlockBuffer.h"
#include "DataIOSvc/DataInputSvc.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"

BeginEvtHdl::BeginEvtHdl(Task* par)
    : m_par(par)
{
    SniperPtr<DataInputSvc> iSvc(m_par, "InputSvc");
    if ( iSvc.invalid() ) {
        LogFatal << "cann't find InputSvc for "
                 << m_par->scope() << m_par->objName() << std::endl;
        throw SniperException("InputSvc is invalid");
    }
    m_iSvc = iSvc.data();

    SniperDataPtr<EvtBlockBuffer> buffer(m_par, "/Event");
    if ( buffer.invalid() ) {
        LogError << "cann't get the Data Buffer" << std::endl;
        throw SniperException("Data Buffer is invalid");
    }
    m_buf = buffer.data();
}

bool BeginEvtHdl::handle(Incident& /*incident*/)
{
    if ( m_buf->next(m_iSvc) ) {
        return true;
    }
    LogError << "failed to retrieve next event"
             << std::endl;
    return Incident::fire("StopRun");
}
