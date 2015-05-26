#ifndef ED_BLOCK_BUFFER
#define ED_BLOCK_BUFFER

#include "MemoryMgr/EvtDataBlock.h"
#include "DataBuffer/DataBuffer.h"

class HeaderObject;
class DataInputSvc;

class EvtBlockBuffer : public DataBuffer<EvtDataBlock> {

    public:
        EvtBlockBuffer(int sizeLimit);
        ~EvtBlockBuffer();

        bool next(DataInputSvc* iSvc);
        void next();
        bool adopt(HeaderObject* header);

    private:
        void readNext(DataInputSvc* iSvc);

    private:
        int m_sizeLimit;
        EvtDataBlock* m_beyond;
        bool m_init;
};

#endif
