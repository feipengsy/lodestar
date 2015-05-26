#include "DataIOSvc/DataInputSvc.h"
#include "DataIOSvc/IInputStream.h"
#include "SniperKernel/SvcFactory.h"
#include "RootIOStream/RootInputStream.h"
#include "CorsikaIO/CorsikaInputStream.h"
#include "DataIOUtil.h"

DECLARE_SERVICE(DataInputSvc);

DataInputSvc::DataInputSvc(const std::string& name)
    : SvcBase(name)
{
   declProp("InputStream", m_inputFile);
}


DataInputSvc::~DataInputSvc()
{

}

bool DataInputSvc::initialize()
{
    LogInfo  << "Initializing DataOutputSvc..."
             << std::endl;

    Str2Str::iterator it, end = m_inputFile.end();
    LogInfo  << "Input streams list:"
             << std::endl;

    for (it = m_inputFile.begin(); it != end; ++it) {
        if (!it->second.size()) {
            LogError << "Path: " << it->first << " does not have input files!"
                     << std::endl;
            return false;
        }
        LogInfo  << "Path: " << it->first << "  <<==  "
                 << "File name(s): " << it->second 
                 << std::endl;
        if (m_inputStream.find(it->first) != m_inputStream.end()) {
            LogError << "Found duplicated input stream: " << it->first
                     << std::endl;
            return false;
        }
        std::vector<std::string> fileList = getSepedFileList(it->second);
        IInputStream* iStream = createInputStream(it->first, fileList);
        if (!iStream) return false;
        m_inputStream.insert(std::make_pair(it->first, iStream));
    }

    LogInfo << "Successfully initialized."
            << std::endl;
    return true;
}

bool DataInputSvc::finalize()
{
    Str2Stream::iterator it, end = m_inputStream.end();
    for (it = m_inputStream.begin(); it != end; ++it) {
        delete it->second;
    }

    LogInfo << "Successfully finalized."
            << std::endl;
    return true;
}

std::map<std::string, IInputStream*>& DataInputSvc::inputStream()
{
    return m_inputStream;
}

IInputStream* DataInputSvc::createInputStream(const std::string& path, const std::vector<std::string>& files)
{
    if (getLowerSuffix(files[0]) == "root") {
        LogDebug << "Creating RootInputStream for path: " << path
                 << std::endl;
        return new RootInputStream(path, files);
    }

    if (getLowerSuffix(files[0]) == "corsika") {
        LogDebug << "Creating CorsikaInputStream for path: " << path
                 << std::endl;
        return new CorsikaInputStream(files);
    }

    LogError << "Failed to judge stream type of " << path 
             << std::endl;
    return 0;
}
