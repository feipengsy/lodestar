#include "MemoryMgr/EvtBlockBuffer.h"
#include "DataIOSvc/DataInputSvc.h"
#include "DataIOSvc/IInputStream.h"
#include "Event/HeaderObject.h"

EvtBlockBuffer::EvtBlockBuffer(int sizeLimit) 
    : m_sizeLimit(sizeLimit)
    , m_beyond(0)
    , m_init(false)
{

}

EvtBlockBuffer::~EvtBlockBuffer()
{
}

bool EvtBlockBuffer::next(DataInputSvc* iSvc)
{
    if (!m_init) {
        m_init = true;
        readNext(iSvc);
        m_iCur = 0;
    }
    if (m_iCur >= m_dBuf.size()) {
        if ( m_beyond == 0 ) {
            return false;
        }
        m_dBuf.push_back(ElementPtr(m_beyond));
        readNext(iSvc);
        if (m_dBuf.size() > m_sizeLimit) {
            m_dBuf.pop_front();
            --m_iCur;
        }
    }
    
    return true;
}

void EvtBlockBuffer::next()
{
    ++m_iCur;
}

bool EvtBlockBuffer::adopt(HeaderObject* header)
{
    if (!m_init) {
        m_init = true;
        m_iCur = 0;
    }
    if (m_iCur < m_dBuf.size()) {
        return curEvt()->addHeader(header);
    }
    EvtDataBlock* edb = new EvtDataBlock();
    edb->addHeader(header);
    m_dBuf.push_back(ElementPtr(edb));

    if (m_dBuf.size() > m_sizeLimit) {
        m_dBuf.pop_front();
        --m_iCur;
    }

    return true;
}

void EvtBlockBuffer::readNext(DataInputSvc* iSvc)
{
    EvtDataBlock* edb = 0;
    std::map<std::string, IInputStream*> iStreams = iSvc->inputStream();
    for (std::map<std::string, IInputStream*>::iterator it = iStreams.begin(); it != iStreams.end(); ++it) {
        if (it->second->next()) {
            if (!edb) edb = new EvtDataBlock;
            HeaderObject* header = static_cast<HeaderObject*>(it->second->get());
            edb->addHeader(header);
        }
    }
    m_beyond = edb;
}
