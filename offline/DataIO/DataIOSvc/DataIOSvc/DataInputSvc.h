/*********************************************************************************
  *FileName:    DataInputSvc.h
  *Author:      Teng
  *Date:        2015.5.26
  *Description: Data input service, manager of input streams. 
                DataInputSvc will configure input files, accordingly create and
                manage input streams. 
**********************************************************************************/
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

        // Framework interfaces
        bool initialize();
        bool finalize();

        // Get the input stream map
        Str2Stream& inputStream();

    private:
        // Create an input stream
        IInputStream* createInputStream(const std::string& path, const std::vector<std::string>& files);

    private:
        Str2Stream   m_inputStream;  // Key: path, Value: input stream
        Str2Str      m_inputFile;    // Key: path, Value: input file list 
};

#endif
