#include "CorsikaIO/CorsikaInputStream.h"
#include "CorsikaFileReader.h"
#include "Event/CorsikaHeader.h"
#include "Event/EventObject.h"

CorsikaInputStream::CorsikaInputStream(const std::vector<std::string>& fileList)
    : m_files(fileList)
{
    m_reader = new CorsikaFileReader(fileList);
}

CorsikaInputStream::~CorsikaInputStream()
{
    delete m_reader;
}

bool CorsikaInputStream::next(unsigned int nsteps, bool read)
{
    if (!read || 1 != nsteps) {
        return false;
    }
    return m_reader->readNext();
}

bool CorsikaInputStream::prev(unsigned int nsteps, bool read)
{
    return IInputStream::prev(nsteps, read);
}

bool CorsikaInputStream::first(bool read)
{
    return IInputStream::first(read);
}

bool CorsikaInputStream::last(bool read)
{
    return IInputStream::last(read);
}

EventObject* CorsikaInputStream::get()
{
    return m_reader->get();
}
