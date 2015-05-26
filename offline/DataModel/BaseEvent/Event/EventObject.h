#ifndef BASEEVENT_EVENTOBJECT_H
#define BASEEVENT_EVENTOBJECT_H

#include "TObject.h"

class EventObject: public TObject {

    public:
        // Default Constructor
        EventObject() : m_RefCount(0) {}
        // Default Destructor
        virtual ~EventObject() {}
        // Add the reference count by one
        unsigned int AddRef();
        // Minus the reference count by one
        unsigned int DesRef();

    private:
        unsigned int m_RefCount; //! reference count
  
    ClassDef(EventObject,1);
  
};

inline unsigned int EventObject::AddRef()
{
  return ++m_RefCount;
}

inline unsigned int EventObject::DesRef()
{
  if (m_RefCount > 0) return --m_RefCount;
  return 0;
}


#endif
