#ifndef ROOT_INPUT_STREAM_H
#define ROOT_INPUT_STREAM_H

#include "DataIOSvc/IInputStream.h"
#include "TObject.h"
#include <vector>
#include <string>

class EventObject;
class TTree;
class TFile;
class InputElementKeeper;

class RootInputStream : public IInputStream {

    public:
        RootInputStream(const std::string& path, const std::vector<std::string>& fileList);
        ~RootInputStream();

        bool next(unsigned int nsteps, bool read);
        bool prev(unsigned int nsteps, bool read);
        bool first(bool read);
        bool last(bool read);
        EventObject* get();

    private:
        void newFile(int fileIndex);
        bool read();

    private:
        std::string               m_path;
        std::vector<std::string>  m_files;
        TTree*                    m_tree;
        TFile*                    m_file;
        Long64_t                  m_fileEntries;
        Long64_t                  m_fileEntry;
        Long64_t                  m_streamEntry;
        int                       m_fileIndex;
        void*                     m_addr;
        InputElementKeeper*       m_keeper;
};


#endif
