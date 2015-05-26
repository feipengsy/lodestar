#ifndef SMART_REF_TABLE
#define SMART_REF_TABLE

#include "TObject.h"
#include <string>
#include <vector>
#include <map>

class TablePerTree;
class TFile;
class TTree;
class TBranch;

class SmartRefTableImpl {

    public:

        SmartRefTableImpl();
        ~SmartRefTableImpl();
        // Clear the table
        void clear();
        // Clear the table and reclaim memory
        void reset();
        // Given a SmartRef, get the id of the tree holding the object if refers to
        Int_t getTreeID(Int_t uid, const std::string& guid);
        // Read UniqueIDTable, register all referenced object it holds
        void readTable(TablePerTree* table, Int_t treeid);
        Int_t addRef();
        Int_t decRef();

    private:

        // Register a referenced object into this table
        void Add(const std::string& guid, Int_t uid, Int_t tid);
        // Add one guid of TProcessID
        Int_t AddInternalIdxForPID(const std::string& guid, bool create);
        // Expand table for a new TProcessID
        Int_t ExpandForIID(Int_t iid, Int_t newsize);
        void  ExpandPIDs(Int_t numpids);
        Int_t FindPIDGUID(const std::string& guid) const;
        Int_t GetInternalIdxForPID(const std::string& guid, bool create) const;

    private:
        Int_t**                        m_TreeIDs;       // Array of tree ids 
        Int_t                          m_PreIid;        // Cached iid
        std::string                    m_PrePID;        // Cached TProcessID title
        std::vector<std::string>       m_ProcessGUIDs;  // Array of TProcessID titles
        Int_t*                         m_N;             // Max number of fTreeIDs[iid]
        Int_t                          m_NumPIDs;       // Number of known TProcessIDs
        Int_t*                         m_AllocSize;     // Length of fTreeIDs[iid]
        Int_t                          m_MinSize;       // Minimum allocating size of fTreeIDs
        Int_t                          m_MaxSize;       // Maximum allocating size of fTreeIDs
        Int_t                          m_RefCount;      // Reference count
};


class SmartRefTable {

    typedef std::map<std::string, SmartRefTableImpl*>::iterator SRTIterator;

    public:
        SmartRefTable();
        ~SmartRefTable();
        // Given a SmartRef, get the branch holding the referenced object
        TBranch* getBranch(Int_t uid, const std::string& guid);
        // Add a new file, start a new table
        bool newFile(const std::string& filename, TFile* file);
        // Try to remove a table, when the reference count hits 0
        void decFileRef(const std::string& filename);

    private:
        std::map<std::string, SmartRefTableImpl*>  m_tableList;  // List of the SmartRefTableImpl*(one for each file) 
        std::vector<TTree*>                        m_treeList; // List of known trees
};


#endif
