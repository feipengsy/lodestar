#include "MemoryMgr/EvtDataBlock.h"
#include "Event/HeaderObject.h"
#include "EDMUtil/EDMManager.h"

EvtDataBlock::EvtDataBlock()
{
}

EvtDataBlock::~EvtDataBlock()
{
    for (std::map<std::string, HeaderObject*>::iterator it = m_headers.begin(); it != m_headers.end(); ++it) {
        delete it->second;
    }
}

bool EvtDataBlock::addHeader(HeaderObject* header)
{
    if (header != NULL) {
        std::string path = EDMManager::get()->getPathWithHeader(header->ClassName());
        if (m_headers.find(path) == m_headers.end()) {
            m_headers.insert(std::make_pair(path, header));
            return true;
        }
    }
    return true; 
}

HeaderObject* EvtDataBlock::getHeader(const std::string& path)
{
    return m_headers.find(path) == m_headers.end() ? 0 : m_headers[path];
}
