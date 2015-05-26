#ifndef I_OUTPUT_STREAM_H
#define I_OUTPUT_STREAM_H

class EventObject;

class IOutputStream {

    public:
        IOutputStream() {}
        virtual ~IOutputStream() {}
        virtual bool write(EventObject* event) = 0;

};

#endif
