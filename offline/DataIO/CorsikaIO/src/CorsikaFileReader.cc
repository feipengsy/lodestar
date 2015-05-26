#include "CorsikaFileReader.h"
#include "CorsikaBuffer.h"
#include "Event/CorsikaHeader.h"

CorsikaFileReader::CorsikaFileReader(const std::vector<std::string>& filenames)
    : m_file(filenames)
    , m_runBuffer(0)
    , m_eventBuffer(0)
    , m_curEvent(0)
    , m_fileIndex(-1)
    , m_fileEntry(-1)
    , m_streamEntry(-1)
    , m_cskFile(0)
    , m_sepBuffer(0)
    , m_index(0)
    , m_length(0)
{
    this->nextFile();
}

CorsikaFileReader::~CorsikaFileReader()
{
    if (m_eventBuffer) delete m_eventBuffer;
    if (m_runBuffer) delete m_runBuffer;
    if (m_cskFile != 0) {
        fclose(m_cskFile);
    }
}

bool CorsikaFileReader::readNext()
{
    if (m_eventBuffer) {
        delete m_eventBuffer;
    }
    m_eventBuffer = new CskEventBuffer;

    // Read event header
    bool ok;
    if (0 == this->caculateSep()) {
        ok = this->readSepSign();
        if (!ok) return false;
    }
    switch (this->caculateSep()) {

        // Normal header
        case EVTH:    ok = readEventHeader();
                      if (!ok) return false;
                      break;
        // Reduced header
        case EVHW:    ok = readReducedEventHeader();
                      if (!ok) return false;
                      break;
        // No events left in this file
        case RUNE:    ok = this->nextFile();
                      if (!ok)  return false;
                      return readNext();
        // Bad case
        default:      return false;

    }

    // Read particles
    bool cont = true;  // Whether continue to read particles
    while (cont) {
        ok = this->readSepSign();
        if (!ok) return false;
        switch (this->caculateSep()) {
            
            // Already passed particle section, record last words and jump back later
            case EVTE:
            case EVTH:
            case RUNE:
            case EVHW:    cont = false;
                          break;
            // Skip 6 words and check again
            case 0:       while ( this->caculateSep() == 0 ) {
                             this->seek(24, SEEK_CUR);
                             this->readSepSign();
                          }
                          cont = false;
                          break;
            // Yeah! It's a particle
            default:      ok = this->readParticle();
                          if (!ok) return false;
        }
    }
    if (this->caculateSep() == EVTE) {
        ok = this->readEventEnd() && this->readLongPara();
        if (!ok) return false;
        m_sepBuffer = 0;
    }
    // TODO Read EndRun here
    return true;
}

LHAASO::CorsikaHeader* CorsikaFileReader::get()
{
    if (!m_runBuffer || !m_eventBuffer) {
        return 0;
    }
    m_curEvent = new LHAASO::CorsikaHeader;
    m_curEvent->setRunID(m_runBuffer->run_number);
    m_curEvent->setEventID(m_eventBuffer->event_number);
    
    LHAASO::CorsikaEvent* event = new LHAASO::CorsikaEvent;
    m_curEvent->setEvent(event);
    for (std::vector<CskParticleBuffer*>::iterator it = m_eventBuffer->Particle.begin(); it != m_eventBuffer->Particle.end(); ++it) {
        LHAASO::CorsikaParticle* particle = new LHAASO::CorsikaParticle;
        particle->setParticle((*it)->particle);
        particle->setGeneration((*it)->generation);
        particle->setObs_level((*it)->obs_level);
        particle->setPx((*it)->px);
        particle->setPy((*it)->py);
        particle->setPz((*it)->pz);
        particle->setX((*it)->x);
        particle->setY((*it)->y);
        particle->setTime((*it)->time);
        //particle->setHeight((*it)->height);
        particle->setTheta((*it)->theta);
        particle->setPhi((*it)->phi);
        particle->setTheta_deg((*it)->theta_deg);
        particle->setPhi_deg((*it)->phi_deg);
        particle->setP((*it)->p);
        event->addParticle(particle);
    }
    return m_curEvent;
}

bool CorsikaFileReader::nextFile()
{
    if (++m_fileIndex > m_file.size() - 1) {
        return false;
    }
    if (m_cskFile != 0) {
        fclose(m_cskFile);
        m_cskFile = 0;
    }
    m_cskFile = fopen(m_file[m_fileIndex].c_str(), "r");
    if (!m_cskFile) {
        return false;
    }
    m_index = 0;

    if (m_runBuffer) {
        delete m_runBuffer;
    }
    this->readSepSign();
    if (this->caculateSep() == RUNH) {
        m_runBuffer = new CskRunBuffer;
        m_sepBuffer = 0;
        return this->readRunHeader();
    }
    return false;
}

bool CorsikaFileReader::readRunHeader()
{
    bool ok = (
        // Read run number
        this->read( &m_runBuffer->run_number, 'i', 1)   && 
        //Read Date and version
        this->read( &m_runBuffer->date, 'f', 2)         && 
        //Read nr of and 10 obs. levels
        this->read( &m_runBuffer->nr_obs_level, 'i', 1) && 
        this->read( m_runBuffer->obs_level_cm, 'f', 10) && 
        // read slope, e_min and max
        this->read( &m_runBuffer->slope, 'f', 3)        && 
        // read egs and nkg flag
        this->read( &m_runBuffer->egs_flag, 'i', 2)     && 
        // read cut-offs
        this->read( &m_runBuffer->cut_hadr, 'f', 4)     && 
        // read 50 c-const, 20 cc-const, 40 cka-const, 5 ceta-const and 11 cstrba const
        // 50 + 20 + 40 + 5 + 11 = 126
        this->read( m_runBuffer->c, 'f', 126)           && 
        // skip 4 unused floats
        this->seek(16, SEEK_CUR)                        && 
        // read 50 can, 50 cann, 5 aatm, 5 batm, 5 catm consts
        // 50 +50 +5 +5 +5 = 115
        this->read( m_runBuffer->can, 'f', 115)         && 
        // read nfl ...
        this->read( &m_runBuffer->nflain, 'i', 4)        
    );
    return ok;
}

bool CorsikaFileReader::readEventHeader()
{
    bool ok = ( 
        // Read event number and particle id 
        this->read( &m_eventBuffer->event_number, 'i', 2)          && 
        // Read ener, altitude ... 
        this->read( &m_eventBuffer->energy, 'f', 2)                &&
        // read target 
        this->read( &m_eventBuffer->target, 'i', 1)                && 
        this->read( &m_eventBuffer->interaction_height, 'f', 6)    && 
        this->read( &m_eventBuffer->sequence_number, 'i', 1)
    );
    if (!ok) return false;

    for (int i=0; i<10; i++ ) {
        ok =  ok && (
            this->read( &m_eventBuffer->seed[i][0], 'i', 1)  && 
            this->read( &m_eventBuffer->seed[i][1], 'i', 1)  && 
            this->read( &m_eventBuffer->seed[i][2], 'f', 1) 
        );
        if (!ok) return false;
    }

    //skip 22 unused or dupilicatefloats
    ok = ok && (
        this->seek(124, SEEK_CUR)                                      && 
        this->read(&m_eventBuffer->gheisha_flag, 'i', 6)  && 
        this->read(&m_eventBuffer->theta_min, 'f', 4)     && 
        this->seek(36, SEEK_CUR)                                       && 
        this->read(&m_eventBuffer->muon_flag, 'i', 1)     && 
        this->seek(176, SEEK_CUR)                                      && 
        this->read(&m_eventBuffer->sybill_flag, 'i', 8)   && 
        this->seek(8, SEEK_CUR)                                        && 
        this->seek(492, SEEK_CUR)                                      &&
        this->seek(8, SEEK_CUR)
    );
    m_eventBuffer->reduced = 0;

    return ok;
}

bool CorsikaFileReader::readEventEnd()
{
    // For now, we just skip event end
    //int* temp = new int;
    //bool ok = this->read(temp, 'i', 6) && 
     bool ok = this->seek(1016, SEEK_CUR);
    //delete temp;
    return ok;
}

bool CorsikaFileReader::readLongPara()
{
    // For now, we just skip long para
    //float* temp = new float[7];
    //bool ok = this->read(temp, 'f', 7) && 
    bool ok = this->seek(72, SEEK_CUR);
    //delete [] temp;
    return ok;
}

bool CorsikaFileReader::readReducedEventHeader()
{
    bool ok = ( 
        // Read event number and particle id
        this->read(&m_eventBuffer->event_number, 'i', 2)         &&
        // Read ener, altitude ...
        this->read(&m_eventBuffer->energy, 'f', 2)               &&
        // read target
        this->read(&m_eventBuffer->target, 'i', 1)               &&
        this->read(&m_eventBuffer->interaction_height, 'f', 6)
    );
    m_eventBuffer->reduced = 1;
    return ok;
}

bool CorsikaFileReader::readParticle()
{
    CskParticleBuffer* part = new CskParticleBuffer;
    m_eventBuffer->Particle.push_back(part);
    
    bool ok;
    switch( caculateSep() ) {

        case 9999:   // particles from interaction stack
            ok = ( 
                this->read( &part->particle,   'i', 1)  &&
                this->read( &part->p,          'f', 1)  &&
                this->read( &part->theta,      'f', 2)  &&
                this->read( &part->height,     'f', 1)  &&
                this->read( &part->time,       'f', 1)  &&
                this->read( &part->x,          'f', 2)  &&
                this->read( &part->generation, 'i', 1)  &&
                this->read( &part->number,     'i', 2)
            );

            part->inter = 1;
            //part->mother = 0;

            return ok;

        default:
            //part->mother = 0;
            part->inter = 0;

            int i = caculateSep();

            part->particle = i / 1000;        // Read the particles data
            i = i - part->particle * 1000;
            part->generation = i / 10;
            i = i- part->generation * 10;
            part->obs_level = i;

            // We don't save mother particle infomation for now
            if( part->particle == 75 || part->particle == 76) {
                /*
                part->mother = (struct Mother *) malloc( sizeof( struct Mother ) );
                if ( part->mother == NULL ) {
                    printf( "No Mem in ReadParticle\n" );
                    exit (1);
                }

                part->mother->particle = part->particle;
                part->mother->generation = part->generation;
                part->mother->obs_level = part->obs_level;
                */

                float mpx = 0;
                ok = (
                    this->read(&mpx, 'f', 6)  &&
                    this->read(&mpx, 'f', 1)
                );
                if (!ok) return false;

                /*
                i = ftoi( f ) ;
                part->particle = i / 1000;        // Read the particles data/
                i = i - part->particle * 1000;
                part->generation = i / 10;
                i = i- part->generation * 10;
                part->obs_level = i;
                */
            }

        ok = this->read(&part->px, 'f', 6);
        return ok; 
    } 
}

bool CorsikaFileReader::readSepSign()
{
    return this->read(&m_sepBuffer, 'f', 1);
}

bool CorsikaFileReader::read(void* addr, char option, int size)
{
    if ( m_index == 0 ) {
        fread( &m_length, 4, 1, m_cskFile );
    }

    switch ( option )  {
        case 'f': fread( addr, 4, size, m_cskFile );
                  break;

        case 'i': float f;
                  for ( int i=0; i<size ; i++ ) {
                      fread( &f, 4, 1, m_cskFile );
                      ((int *)addr)[i] = (int)*((float *)&f);
                  }
                  break;

        default: return false;
    }

    m_index += size*4;
    return this->handleBlockOverflow();
}

bool CorsikaFileReader::seek(int size, int option)
{
    if ( m_index == 0 ) {
        fread( &m_length, 4, 1, m_cskFile );
    }

    fseek( m_cskFile, size, option);
    m_index += size;
    return handleBlockOverflow();
}

bool CorsikaFileReader::handleBlockOverflow()
{
    if ( m_index > m_length ) {
        //printf("Internal error, Read over block end\n");
        return false;
    }

    if ( m_index == m_length ) {
        int i;
        fread( &i, 4, 1, m_cskFile );
        if ( i == m_length ) {
            m_index = 0;
        }
        else {
            //printf("Internal Error Block length different at begin and end\n");
            return false;
        }
    }
    return true;
}

int CorsikaFileReader::caculateSep()
{
    return *((int*)&m_sepBuffer);
}
