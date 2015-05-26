#include "EndEvtHdl.h"
#include "MemoryMgr/EvtBlockBuffer.h"
#include "MemoryMgr/EvtDataBlock.h"
#include "DataIOSvc/DataOutputSvc.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"

EndEvtHdl::EndEvtHdl(Task* par)
    : m_par(par)
    , m_buf(0)
    , m_oSvc(0)
{
    SniperPtr<DataOutputSvc> oSvc(m_par, "OutputSvc");
    if ( oSvc.valid() ) {
        m_oSvc = oSvc.data();
    }

    SniperDataPtr<EvtBlockBuffer> buffer(m_par, "/Event");
    if ( buffer.invalid() ) {
        LogError << "cann't get the Data Buffer" << std::endl;
        throw SniperException("Data Buffer is invalid");
    }
    m_buf = buffer.data();
}

bool EndEvtHdl::handle(Incident& /*incident*/)
{
    if (m_buf->size() == 0) {
        LogError << "There is nothing in Cur Buffer." << std::endl;
        return false;
    }
    if (not m_buf->curEvt()) {
        LogError << "There is no data in the buffer" << std::endl;
        return false;
    }
    if (m_oSvc) {
        std::map<std::string, HeaderObject*>& headers = m_buf->curEvt()->getHeaders();
        for (std::map<std::string, HeaderObject*>::iterator it = headers.begin(); it != headers.end(); ++it) {
            if (!m_oSvc->write(it->first, (EventObject*)it->second)) return false;
        }
    }
    m_buf->next();
    return true;
}
