#include "DataIOSvc/DataOutputSvc.h"
#include "DataIOSvc/IOutputStream.h"
#include "RootIOStream/RootOutputStream.h"
#include "SniperKernel/SvcFactory.h"
#include "DataIOUtil.h"

DECLARE_SERVICE(DataOutputSvc);

DataOutputSvc::DataOutputSvc(const std::string& name)
    : SvcBase(name)
{
    declProp("OutputStream", m_outputFile);
}

DataOutputSvc::~DataOutputSvc()
{
}

bool DataOutputSvc::initialize()
{
    LogDebug << "Initializing DataOutputSvc..."
             << std::endl;

    Str2Str::iterator it, end = m_outputFile.end();
    LogDebug << "Output stream list:"
             << std::endl;
    for (it = m_outputFile.begin(); it != end; ++it) {
        LogDebug << "Path: " << it->first << "  ==>  "
                 << "File name: " << it->second
                 << std::endl;
        IOutputStream* oStream = createOutputStream(it->first, it->second);
        m_outputStream.insert(std::make_pair(it->first, oStream));
    }

    LogDebug << "Successfully initialized."
             << std::endl;
    return true;
}

bool DataOutputSvc::finalize()
{
    Str2Stream::iterator it, end = m_outputStream.end();
    for (it = m_outputStream.begin(); it != end; ++it) {
        delete it->second;
    }

    LogInfo << "Successfully finalized."
            << std::endl;
    return true;
}

bool DataOutputSvc::write(const std::string& path, EventObject* event)
{
    Str2Stream::iterator pos = m_outputStream.find(path);
    if (pos == m_outputStream.end()) {
        LogInfo << "Skipping path: " << path
                << std::endl;
        return true;
    }
    return pos->second->write(event);
}

DataOutputSvc::Str2Stream& DataOutputSvc::outputStream()
{
    return m_outputStream;
}

IOutputStream* DataOutputSvc::createOutputStream(const std::string& path, const std::string& file)
{
    if (getLowerSuffix(file) == "root") {
        return new RootOutputStream(path, file);
    }
    return 0;
}
