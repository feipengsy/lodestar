/*********************************************************************************
  *FileName:    IOutputStream.h
  *Author:      Teng
  *Date:        2015.5.26
  *Description: Interface class for output stream
**********************************************************************************/
#ifndef I_OUTPUT_STREAM_H
#define I_OUTPUT_STREAM_H

class EventObject;

class IOutputStream {

    public:
        IOutputStream() {}
        virtual ~IOutputStream() {}
        // Write one event
        virtual bool write(EventObject* event) = 0;

};

#endif
