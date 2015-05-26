#ifndef DATA_BUFFER_MGR
#define DATA_BUFFER_MGR

#include "SniperKernel/SvcBase.h"
#include "MemoryMgr/IMemoryMgr.h"
#include <string>

class IIncidentHandler;
class HeaderObject;

class DataBufferMgr : public SvcBase, public IMemoryMgr {

    public:
        DataBufferMgr(const std::string& name);
        ~DataBufferMgr();

        bool initialize();
        bool finalize();

        bool adopt(HeaderObject* header, const std::string& path);

    private:
        int                m_sizeLimit;
        IIncidentHandler*  m_beginIcdt;
        IIncidentHandler*  m_endIcdt;

};


#endif
