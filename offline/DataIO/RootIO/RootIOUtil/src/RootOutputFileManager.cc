#include "RootIOUtil/RootOutputFileManager.h"
#include "TFile.h"

RootOutputFileManager* RootOutputFileManager::m_instance = 0;

RootOutputFileManager::RootOutputFileManager()
{
}

RootOutputFileManager::~RootOutputFileManager()
{
    for (FileMap::iterator it = m_file.begin(); it != m_file.end(); ++it) {
        if (it->second.second->IsOpen()) {
            it->second.second->Close();
        }
        delete it->second.second;
    }
}

RootOutputFileManager* RootOutputFileManager::get()
{
    if (!m_instance) {
        m_instance = new RootOutputFileManager;
    }
    return m_instance;
}

TFile* RootOutputFileManager::newFile(const std::string& name)
{
    FileMap::iterator pos = m_file.find(name);
    if (pos == m_file.end()) {
        TFile* file = new TFile(name.c_str(), "recreate");
        m_file.insert(std::make_pair(name, std::make_pair(0, file)));
        return file;
    }
    return pos->second.second;
}

TFile* RootOutputFileManager::getFile(const std::string& name)
{
    FileMap::iterator pos = m_file.find(name);
    if (pos == m_file.end()) {
        return 0;
    }
    return pos->second.second;
}

void RootOutputFileManager::occupyFile(const std::string& name)
{
    FileMap::iterator pos = m_file.find(name);
    if (pos != m_file.end()) {
        ++pos->second.first;
    }
}

void RootOutputFileManager::releaseFile(const std::string& name)
{
    FileMap::iterator pos = m_file.find(name);
    if (pos != m_file.end()) {
        if (--pos->second.first == 0) {
            pos->second.second->Close();
        }
    }
}

void RootOutputFileManager::addRef()
{
    ++m_refCount;
}

void RootOutputFileManager::decRef()
{
    if (0 == --m_refCount) {
        delete this;
    }
}
