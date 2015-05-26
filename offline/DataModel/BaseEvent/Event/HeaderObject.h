#ifndef BASEEVENT_HEADEROBJECT_H
#define BASEEVENT_HEADEROBJECT_H

#include "EventObject.h"
#include <string>

class HeaderObject: public EventObject {
  
    public:
        // Default Constructor
        HeaderObject() : m_EventID(0),
                         m_RunID(0) {}
        // Default Destructor
        virtual ~HeaderObject() {}
        // Retrieve const id of event
        int EventID() const;
        // Update id of event
        void setEventID(int value);
        // Retrieve const id of run
        int RunID() const;
        // Update id of run
        void setRunID(int value);
        // Retrieve event
        virtual EventObject* event(const std::string& eventName) = 0;
        // Update event entry
        virtual void setEventEntry(const std::string& eventName, Long64_t& entry) = 0;

    private:
        int m_EventID; // ID of event
        int m_RunID;   // ID of run

    ClassDef(HeaderObject,1);
  

};

inline int HeaderObject::EventID() const 
{
  return m_EventID;
}

inline void HeaderObject::setEventID(int value) 
{
  m_EventID = value;
}

inline int HeaderObject::RunID() const 
{
  return m_RunID;
}

inline void HeaderObject::setRunID(int value) 
{
  m_RunID = value;
}

#endif
