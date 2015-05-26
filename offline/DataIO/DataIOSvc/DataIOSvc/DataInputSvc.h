#ifndef DATA_INPUT_SVC_H
#define DATA_INPUT_SVC_H

#include "SniperKernel/SvcBase.h"
#include <string>
#include <map>

class IInputStream;

class DataInputSvc : public SvcBase {

    public:
        typedef std::map<std::string, std::string> Str2Str;
        typedef std::map<std::string, IInputStream*> Str2Stream;

        DataInputSvc(const std::string& name);
        ~DataInputSvc();

        bool initialize();
        bool finalize();

        std::map<std::string, IInputStream*>& inputStream();

    private:
        IInputStream* createInputStream(const std::string& path, const std::vector<std::string>& files);

    private:
        Str2Stream   m_inputStream;  // Path to input stream map
        Str2Str      m_inputFile; // Path to input file list map
};


#endif
