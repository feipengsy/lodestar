#include "RootIOUtil/OutputTreeHandle.h"
#include "RootIOUtil/RootInterface.h"
#include "EDMUtil/UniqueIDTable.h"

#include "TTree.h"
#include "TFile.h"
#include "TProcessID.h"

OutputTreeHandle::OutputTreeHandle(const std::string& path, const std::string& className, const std::string& treeName, bool withTable)
    : m_path(path)
    , m_className(className)
    , m_table(0)
    , m_tree(0)
    , m_file(0)
    , m_entry(-1)
    , m_addr(0)
    , m_withTable(withTable)
{
    if (treeName.find("::") == treeName.npos) {
        m_treeName = treeName;
    } 
    else {
        m_treeName = treeName.substr(treeName.rfind("::")+2);
    }
    if (m_path[m_path.size() - 1] != '/') {
        m_fullTreeName = m_path + '/' + m_treeName;
    }
    else {
        m_fullTreeName = m_path + m_treeName;
    }
}

OutputTreeHandle::~OutputTreeHandle()
{
}

const std::string& OutputTreeHandle::getTreeName()
{
    return m_fullTreeName;
}

const std::string& OutputTreeHandle::getClassName()
{
    return m_className;
}

TablePerTree* OutputTreeHandle::getUniqueIDTable()
{
    return m_table;
}

void OutputTreeHandle::startFile(TFile* file)
{
    m_file = file;
    m_entry = -1;
    RootInterface::createDirectory(m_path, file);
    std::string treeTitle = std::string("Tree at ") + m_path + " for " + m_className.substr(m_className.rfind("::")+2);
    m_tree = new TTree(m_treeName.c_str(), treeTitle.c_str());
    m_tree->Branch(m_treeName.c_str(), m_className.c_str(), &m_addr, 32000, 0);
}

void OutputTreeHandle::setAddress(void* addr)
{
    m_addr = addr;
}

Long64_t OutputTreeHandle::write()
{
    if (!m_addr) {
        return -1;
    }
    if (m_withTable) {
        this->fillUID();
    }
    m_tree->Fill();
    return ++m_entry;
}

void OutputTreeHandle::finalize()
{
    if (m_withTable && m_uid.size()) {
        m_table = new TablePerTree;
        m_table->SetGUIDs(m_guid);
        m_table->SetIDs(m_uid, m_bid);
    }
    if (-1 != m_entry) {
        m_file->cd(m_path.c_str());
        m_tree->Write();
    }
}

void OutputTreeHandle::fillUID()
{
    TObject* obj = static_cast<TObject*>(m_addr);
    UInt_t uid = obj->GetUniqueID();
    TProcessID* pid = TProcessID::GetProcessWithUID(uid,obj);
    const char* guid = pid->GetTitle();
    int iid;
    StringVector::const_iterator posPID = find( m_guid.begin(), m_guid.end(), guid);
    if (posPID == m_guid.end()) {
        m_guid.push_back(guid);
        m_uid.push_back(std::vector<Int_t>());
        iid = m_guid.size() - 1;
    }
    else {
        iid = posPID - m_guid.begin();
    }
    uid = uid & 0xffffff;
    m_uid[iid].push_back(uid);
}
