#include "RWExample.h"
#include "RootWriter.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

RWExample::RWExample(const std::string& name)
    : AlgBase(name)
{

}

RWExample::~RWExample()
{

}

bool RWExample::initialize()
{
    SniperPtr<RootWriter> rw(getScope(), "rw");
    if (!rw.isValid()) {
        LogError << "Failed to get RootWriter" << std::endl;
        return false;
    }

    m_rootWriter = rw.data();
    return true;
}

bool RWExample::execute()
{
    return true;
}

bool RWExample::finalize()
{

}
