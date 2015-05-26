#ifndef OUTPUT_TREE_HANDLE_H
#define OUTPUT_TREE_HANDLE_H

#include "TObject.h"
#include <vector>
#include <string>

class TablePerTree;
class TFile;
class TTree;

class OutputTreeHandle {

    public:
        typedef std::vector<std::string>                  StringVector;
        typedef std::vector<std::vector<Short_t> >        BIDVector;
        typedef std::vector<std::vector<Int_t> >          UIDVector;

        OutputTreeHandle(const std::string& path,
                         const std::string& className,
                         const std::string& treeName,
                         bool withTable);
        ~OutputTreeHandle();

        const std::string& getTreeName();
        const std::string& getClassName();
        TablePerTree* getUniqueIDTable();
        void startFile(TFile* file);
        void setAddress(void* addr);
        Long64_t write();
        void finalize();

    private:
        void init();
        void fillUID();

    private:
        std::string   m_path;
        std::string   m_className;
        std::string   m_treeName;
        std::string   m_fullTreeName;
        TablePerTree* m_table; 
        TTree*        m_tree;
        TFile*        m_file;
        Long64_t      m_entry;
        void*         m_addr;
        bool          m_withTable;
        StringVector  m_guid;
        BIDVector     m_bid;
        UIDVector     m_uid;
};

#endif
