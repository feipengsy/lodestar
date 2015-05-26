#include "RootIOUtil/RootInterface.h"
#include "EDMUtil/UniqueIDTable.h"
#include "TTree.h"
#include "TFile.h"

TFile* RootInterface::openFile(const std::string& file)
{
    return new TFile(file.c_str(), "read");
}

TTree* RootInterface::getHeaderTree(const std::string& path, TFile* file)
{
    std::string name = path;
    if (name[name.size() - 1] == '/') {
        name = name + "header";
    }
    else {
        name = name + "/" + "header";
    }
    return getTree(name, file);
}

TTree* RootInterface::getTree(const std::string& name, TFile* file)
{
    return static_cast<TTree*>(file->Get(name.c_str()));
}

UniqueIDTable* RootInterface::getUniqueIDTable(TFile* file)
{
    return static_cast<UniqueIDTable*>(file->Get("/Meta/UniqueIDTable"));
}

void RootInterface::writeUniqueIDTable(UniqueIDTable* table, TFile* file)
{
    RootInterface::createDirectory("/Meta", file);
    table->Write();    
}

void RootInterface::createDirectory(const std::string& path, TFile* file)
{
    file->cd();
    std::string fullDirName = path;
    size_t pos;
    while(true) {
        pos = fullDirName.find('/');
        std::string dirName = fullDirName.substr(0, pos);
        if (dirName.size() && !gDirectory->GetDirectory(dirName.c_str())) {
            gDirectory->mkdir(dirName.c_str());
        }
        gDirectory->cd(dirName.c_str());
        if (pos == fullDirName.npos || pos == fullDirName.size() - 1) {
            break;
        }
        fullDirName = fullDirName.substr(pos + 1);
    }
}
