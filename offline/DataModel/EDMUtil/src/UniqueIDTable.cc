#include "EDMUtil/UniqueIDTable.h"

void TablePerTree::MergeTable(const TablePerTree& table)
{
    size_t len = table.m_GUIDs.size();
    for (size_t i = 0; i < len; ++i) {
        GUIDVector::iterator pos = std::find(m_GUIDs.begin(), m_GUIDs.end(), table.m_GUIDs[i]);
        if (pos == m_GUIDs.end()) {
            // A new guid added into table
            m_GUIDs.push_back(table.m_GUIDs[i]);
            if (table.m_UniqueIDs.size() > i) {
                m_UniqueIDs.push_back(table.m_UniqueIDs[i]);
            }
            if (table.m_BranchIDs.size() > i) {
                m_BranchIDs.push_back(table.m_BranchIDs[i]);
            }
        }
        else {
            // Same guid found
            size_t idx = pos - m_GUIDs.begin();
            if (table.m_UniqueIDs.size() > i && m_UniqueIDs.size() > idx) {
                m_UniqueIDs[idx].insert(m_UniqueIDs[idx].end(), table.m_UniqueIDs[i].begin(), table.m_UniqueIDs[i].end());
            }
            if (table.m_BranchIDs.size() > i && m_BranchIDs.size() > idx) {
                m_BranchIDs[idx].insert(m_BranchIDs[idx].end(), table.m_BranchIDs[i].begin(), table.m_BranchIDs[i].end());
            }
        }
    }
}

UniqueIDTable::~UniqueIDTable()
{
    for (TableMap::iterator it = m_tables.begin(); it != m_tables.end(); ++it) {
        delete it->second;
    }
}

void UniqueIDTable::AddTable(const std::string& treename,
                             const TablePerTree::GUIDVector& guid,
                             const TablePerTree::UIDVector& uid,
                             const TablePerTree::BIDVector& bid)
{
    TablePerTree* table = new TablePerTree();
    table->SetGUIDs(guid);
    table->SetIDs(uid, bid);
    this->AddTable(treename, table);
}

void UniqueIDTable::AddTable(const std::string& treename, TablePerTree* table)
{
    if (0 == table) return;
    m_tables.insert(std::make_pair(treename, table));
}

void UniqueIDTable::MergeTable(const UniqueIDTable& table)
{
    for (TableMap::const_iterator it = table.m_tables.begin(); it != table.m_tables.end(); ++it) {
        if (m_tables.find(it->first) == m_tables.end()) {
            TablePerTree* newTPR = new TablePerTree(*it->second);
            m_tables.insert(TableMap::value_type(it->first, newTPR));
        }
        else {
            m_tables[it->first]->MergeTable(*it->second);
        }
    }
}
