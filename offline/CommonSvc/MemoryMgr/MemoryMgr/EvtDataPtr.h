#ifndef EVT_DATA_PTR_H
#define EVT_DATA_PTR_H

#include "SniperKernel/RefBase.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include "EvtDataBlock.h"
#include "EvtBlockBuffer.h"
#include <string>

template<typename EventType>
class EvtDataPtr : public RefBase<EventType>
{
    public:
        EvtDataPtr(const std::string& path);
        EvtDataPtr(Task* par, const std::string& path);

    private:
        void init(Task* par, const std::string& path);

        // Not supported
        EvtDataPtr();
        EvtDataPtr(const EvtDataPtr&);
        EvtDataPtr& operator=(const EvtDataPtr&);
};

template<typename EventType>
EvtDataPtr<EventType>::EvtDataPtr(const std::string& path)
{
    Task* par;
    try {
        par = Task::top();
    }
    catch(SniperException& e) {
        LogError << "TopTask=0: Please set TopTask or assign a TaskScope"
                 << std::endl;
        throw e;
    }

    init(par, path);
}

template<typename EventType>
EvtDataPtr<EventType>::EvtDataPtr(Task* par, const std::string& path)
{
    init(par, path);
}

template<typename EventType>
void EvtDataPtr<EventType>::init(Task* par, const std::string& path)
{
    Task* _par = par;
    std::string name = path;

    std::string::size_type pseg = path.rfind(":");
    if ( pseg != std::string::npos ) {
        _par = dynamic_cast<Task*>( par->find(path.substr(0, pseg)) );
        name = path.substr(pseg+1, std::string::npos);
    }

    DataMemSvc* svc = dynamic_cast<DataMemSvc*>(_par->find("DataMemSvc"));
    if (!svc) {
        this->m_obj = 0;
    }
    EvtBlockBuffer* buffer = dynamic_cast<EvtBlockBuffer*>(svc->find("/Event"));
    if (!buffer) {
        this->m_obj = 0;
    }
    EvtDataBlock* dataBlock = dynamic_cast<EvtDataBlock*>(buffer->curEvt());
    if (!dataBlock) {
        this->m_obj = 0;
    }
    this->m_obj = dynamic_cast<EventType*>(dataBlock->getHeader(name));
}

#endif
