#ifndef ROOT_INTERFACE_H
#define ROOT_INTERFACE_H

#include <string>

class TFile;
class TTree;
class UniqueIDTable;

class RootInterface {

    public:

        static TFile*          openFile(const std::string& file);
        static TTree*          getHeaderTree(const std::string& path, TFile* file);
        static TTree*          getTree(const std::string& name, TFile* file);
        static UniqueIDTable*  getUniqueIDTable(TFile* file);
        static void            writeUniqueIDTable(UniqueIDTable* table, TFile* file);
        static void            createDirectory(const std::string& path, TFile* file);

};

#endif
