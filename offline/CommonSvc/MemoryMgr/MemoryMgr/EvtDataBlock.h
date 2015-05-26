#ifndef EVT_DATA_BLOCK_H
#define EVT_DATA_BLOCK_H

#include <map>
#include <string>

class HeaderObject;

class EvtDataBlock {

    public:
        EvtDataBlock();
        ~EvtDataBlock();

        bool addHeader(HeaderObject* header);
        HeaderObject* getHeader(const std::string& path);
        std::map<std::string, HeaderObject*>& getHeaders() { return m_headers; }

    private:
        std::map<std::string, HeaderObject*> m_headers;

};

#endif
