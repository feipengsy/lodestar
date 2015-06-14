#ifndef CORSIKA_FILE_READER_H
#define CORSIKA_FILE_READER_H

#include <string>
#include <vector>

#define RUNH 1213093202
#define RUNE 1162761554
#define EVTH 1213486661
#define EVTE 1163155013
#define EVHW 1464358469

extern double PI;

namespace LHAASO {
    class CorsikaHeader;
}
class CskRunBuffer;
class CskEventBuffer;
class CskParticleBuffer;

class CorsikaFileReader {

    public:
        CorsikaFileReader(const std::vector<std::string>& filename);
        ~CorsikaFileReader();

        bool readNext();
        LHAASO::CorsikaHeader* get();

    private:
        bool nextFile();
        bool readRunHeader();
        bool readEventHeader();
        bool readEventEnd();
        bool readLongPara();
        bool readReducedEventHeader();
        bool readParticle();
        void calculateParticle(CskParticleBuffer* part);
        bool readSepSign();
        bool read(void* addr, char option, int size);
        bool seek(int size, int option);
        bool handleBlockOverflow();
        int  calculateSep();

    private:
        std::vector<std::string> m_file;
        CskRunBuffer*            m_runBuffer;
        CskEventBuffer*          m_eventBuffer;
        LHAASO::CorsikaHeader*   m_curEvent;
        int                      m_fileIndex;
        int                      m_fileEntry;
        int                      m_streamEntry;
        FILE*                    m_cskFile;
        float                    m_sepBuffer;
        int                      m_index;
        int                      m_length;
};

#endif
