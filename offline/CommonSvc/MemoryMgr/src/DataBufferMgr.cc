#include "DataBufferMgr.h"
#include "BeginEvtHdl.h"
#include "EndEvtHdl.h"
#include "MemoryMgr/EvtBlockBuffer.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SvcFactory.h"

DECLARE_SERVICE(DataBufferMgr);

DataBufferMgr::DataBufferMgr(const std::string& name)
    : SvcBase(name)
{
    declProp("BufferSize", m_sizeLimit = 1);
}

DataBufferMgr::~DataBufferMgr()
{

}

bool DataBufferMgr::initialize()
{
    Task* task = getScope();

    SniperPtr<DataMemSvc> dms(task, "DataMemSvc");
    dms->regist("/Event", new EvtBlockBuffer(m_sizeLimit));

    if ( task->find("InputSvc") != 0 ) {
        IIncidentHandler* m_beginIcdt = new BeginEvtHdl(task);
        if ( task->isTop() ) {
            m_beginIcdt->regist("BeginEvent");
        }
        else {
            m_beginIcdt->regist(task->scope() + task->objName() + ":BeginEvent");
        }
    }

    IIncidentHandler* m_endIcdt = new EndEvtHdl(task);
    if ( task->isTop() ) {
        m_endIcdt->regist("EndEvent");
    }
    else {
        m_endIcdt->regist(task->scope() + task->objName() + ":EndEvent");
    }

    LogDebug << "Initialized Sucessfully." << std::endl;

    return true;
}

bool DataBufferMgr::finalize()
{
    delete m_beginIcdt;
    delete m_endIcdt;

    LogDebug << "Finalized Sucessfully." << std::endl;

    return true;
}

bool DataBufferMgr::adopt(HeaderObject* header, const std::string& path)
{
    SniperDataPtr<EvtBlockBuffer> buffer(getScope(), path);
    if ( buffer.valid() ) {
        return buffer->adopt(header);
    }

    LogError << "Cann't find DataBuffer @" << path << std::endl;
    return false;
}
