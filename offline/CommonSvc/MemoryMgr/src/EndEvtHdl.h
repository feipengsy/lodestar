#ifndef END_EVENT_HANDLER_H
#define END_EVENT_HANDLER_H

#include "SniperKernel/IIncidentHandler.h"

class Task;
class EvtBlockBuffer;
class DataOutputSvc;

class EndEvtHdl : public IIncidentHandler
{
    public :

        EndEvtHdl(Task* par);

        bool handle(Incident& incident);

    private :

        Task*             m_par;
        EvtBlockBuffer*   m_buf;
        DataOutputSvc*    m_oSvc;
};

#endif
