/*********************************************************************************
  *FileName:    DataOutputSvc.h
  *Author:      Teng
  *Date:        2015.5.26
  *Description: Data output service, manager of output streams. 
                DataOutputSvc will configure output files, create and manage
                output streams.
**********************************************************************************/
#ifndef DATA_OUTPUT_SVC_H
#define DATA_OUTPUT_SVC_H

#include "SniperKernel/SvcBase.h"
#include <string>

class EventObject;
class IOutputStream;

class DataOutputSvc : public SvcBase {

    public:
        typedef std::map<std::string, std::string> Str2Str;
        typedef std::map<std::string, IOutputStream*> Str2Stream;

        DataOutputSvc(const std::string& name);
        ~DataOutputSvc();
        
        // Framework interfaces
        bool initialize();
        bool finalize();

        // Write a event to a stream of certain path
        bool write(const std::string& path, EventObject* event);
        // Get the output stream map
        Str2Stream& outputStream();

    private:
        // Given data path and file name, create output stream of a certain type
        IOutputStream* createOutputStream(const std::string& path, const std::string& file);

    private:
        Str2Stream m_outputStream;  // Key: path,  Value: output stream
        Str2Str m_outputFile;       // Key: path,  Value: output file

};

#endif
