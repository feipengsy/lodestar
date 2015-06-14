/*********************************************************************************
  *FileName:    IInputStream.h
  *Author:      Teng
  *Date:        2015.5.26
  *Description: Interface class for input stream.
**********************************************************************************/
#ifndef I_INPUT_STREAM_H
#define I_INPUT_STREAM_H

class EventObject;

class IInputStream {

    public:
        IInputStream() {}
        virtual ~IInputStream() {}

        // Move forward for n steps and read event is read is true
        virtual bool next(unsigned int nsteps=1, bool read=true) = 0;
        // Move backward for nsteps
        virtual bool prev(unsigned int nsteps=1, bool read=true) = 0;
        // Jump to the first event
        virtual bool first(bool read=true) = 0;
        // Jump to the last event
        virtual bool last(bool read=true) = 0;
        // Retrieve current event
        virtual EventObject* get() = 0;

};

// Default implementations, should never be called
inline bool IInputStream::next(unsigned int nsteps, bool read)
{
    return false;
}

inline bool IInputStream::prev(unsigned int nsteps, bool read)
{
    return false;
}

inline bool IInputStream::first(bool read)
{
    return false;
}

inline bool IInputStream::last(bool read)
{
    return false;
}

#endif
