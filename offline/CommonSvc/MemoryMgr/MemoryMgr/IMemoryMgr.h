#ifndef I_MEMORY_MGR
#define I_MEMORY_MGR

class HeaderObject;

class IMemoryMgr {

    public:
        IMemoryMgr() {}
        virtual ~IMemoryMgr() {}

        virtual bool adopt(HeaderObject* header, const std::string& path) = 0;


};


#endif
