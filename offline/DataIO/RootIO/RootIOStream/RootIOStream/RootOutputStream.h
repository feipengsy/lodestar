#ifndef ROOT_OUTPUT_STREAM_H
#define ROOT_OUTPUT_STREAM_H

#include "DataIOSvc/IOutputStream.h"
#include <string>
#include <vector>

class EventObject;
class OutputTreeHandle;
class RootOutputFileManager;

class RootOutputStream : public IOutputStream {

    public:
        RootOutputStream(const std::string& path, const std::string& file);
        ~RootOutputStream();
  
        bool write(EventObject* header);

    private:
        void init();   

    private:
        std::string                     m_path;
        std::string                     m_outputFile;
        OutputTreeHandle*               m_headerTree;
        std::vector<OutputTreeHandle*>  m_eventTrees;
        RootOutputFileManager*          m_fileMgr;
};

#endif
