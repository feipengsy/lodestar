#include "RootIOStream/RootOutputStream.h"
#include "RootIOUtil/RootOutputFileManager.h"
#include "RootIOUtil/OutputTreeHandle.h"
#include "RootIOUtil/RootInterface.h"
#include "EDMUtil/UniqueIDTable.h"
#include "EDMUtil/EDMManager.h"
#include "Event/HeaderObject.h"
#include "Event/EventObject.h"
#include "SniperKernel/SniperLog.h"

RootOutputStream::RootOutputStream(const std::string& path, const std::string& filename)
    : m_path(path)
    , m_outputFile(filename)
    , m_headerTree(0)
    , m_fileMgr(RootOutputFileManager::get())
{
    this->init();
}

RootOutputStream::~RootOutputStream()
{
    UniqueIDTable* table = new UniqueIDTable;
    m_headerTree->finalize();
    table->AddTable(m_headerTree->getTreeName(), m_headerTree->getUniqueIDTable());
    delete m_headerTree;
    for (std::vector<OutputTreeHandle*>::iterator it = m_eventTrees.begin(); it != m_eventTrees.end(); ++it) {
        (*it)->finalize();
        table->AddTable((*it)->getTreeName(), (*it)->getUniqueIDTable());
        delete *it;
    }
    RootInterface::writeUniqueIDTable(table, m_fileMgr->getFile(m_outputFile));
    delete table;
    m_fileMgr->releaseFile(m_outputFile);
    m_fileMgr->decRef();
}

void RootOutputStream::init() 
{
    m_fileMgr->addRef();
    TFile* file = m_fileMgr->newFile(m_outputFile);
    m_fileMgr->occupyFile(m_outputFile);
    const std::string& headerName = EDMManager::get()->getHeaderNameWithPath(m_path);
    m_headerTree = new OutputTreeHandle(m_path, headerName, "header", false);
    m_headerTree->startFile(file);
    const std::vector<std::string>& eventNames = EDMManager::get()->getEventNameWithPath(m_path);
    for (std::vector<std::string>::const_iterator it = eventNames.begin(); it != eventNames.end(); ++it) {
        OutputTreeHandle* oth = new OutputTreeHandle(m_path, *it, *it, true);
        oth->startFile(file);
        m_eventTrees.push_back(oth);
    }
}

bool RootOutputStream::write(EventObject* header)
{
    HeaderObject* headerObject = dynamic_cast<HeaderObject*>(header);
    if (0 == headerObject) {
        LogError << "RootOutputStream can only handle HeaderObject!"
                 << std::endl;
        return false;
    }
    for (std::vector<OutputTreeHandle*>::iterator it = m_eventTrees.begin(); it != m_eventTrees.end(); ++it) {
        void* evtAddr = headerObject->event((*it)->getClassName());
        if (0 == evtAddr) {
            LogDebug << "Skipping event: " << (*it)->getClassName()
                     << " of path: " << m_path
                     << std::endl;
            continue;
        }
        (*it)->setAddress(evtAddr);
        Long64_t entry = (*it)->write();
        if (-1 == entry) {
            LogError << "Failed writing event: " << (*it)->getClassName()
                     << " of path: " << m_path
                     << std::endl;
            return false;
        }
        headerObject->setEventEntry((*it)->getClassName(), entry);
    }

    m_headerTree->setAddress(headerObject);
    if (-1 == m_headerTree->write()) {
        LogError << "Failed writing header: " << m_headerTree->getClassName()
                 << " of path: " << m_path
                 << std::endl;
        return false;
    }
    return true;
}
