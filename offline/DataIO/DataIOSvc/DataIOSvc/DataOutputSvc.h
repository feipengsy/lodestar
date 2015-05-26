#ifndef DATA_OUTPUT_SVC
#define DATA_OUTPUT_SVC

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
        
        bool initialize();
        bool finalize();

        bool write(const std::string& path, EventObject* event);
        Str2Stream& outputStream();

    private:
        IOutputStream* createOutputStream(const std::string& path, const std::string& file);

    private:
        Str2Stream m_outputStream;
        Str2Str m_outputFile;

};

#endif
