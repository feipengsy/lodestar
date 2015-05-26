#ifndef CORSIKA_INPUT_STREAM
#define CORSIKA_INPUT_STREAM

#include "DataIOSvc/IInputStream.h"
#include <vector>
#include <string>

class EventObject;
class CorsikaFileReader;

class CorsikaInputStream : public IInputStream {

    public:
        CorsikaInputStream(const std::vector<std::string>& fileList);
        ~CorsikaInputStream();

        bool next(unsigned int nsteps, bool read);
        bool prev(unsigned int nsteps, bool read);
        bool first(bool read);
        bool last(bool read);
        EventObject* get();

    private:
        
    private:
        std::vector<std::string>  m_files;
        CorsikaFileReader*        m_reader;

};

#endif
