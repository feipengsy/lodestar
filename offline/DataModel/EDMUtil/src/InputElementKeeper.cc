#include "EDMUtil/InputElementKeeper.h"
#include "SmartRefTable.h"

InputElementKeeper* InputElementKeeper::m_instance = 0;

InputElementKeeper::InputElementKeeper()
    : m_table(new SmartRefTable)
    , m_refCount(1)
{
}

InputElementKeeper::~InputElementKeeper()
{
    delete m_table;
}

bool InputElementKeeper::registerFile(const std::string& filename, TFile* file)
{
    return m_table->newFile(filename, file);
}

void InputElementKeeper::eraseFile(const std::string& filename)
{
    m_table->decFileRef(filename);
}

TBranch* InputElementKeeper::searchBranch(Int_t uid, const std::string& uuid)
{
    return m_table->getBranch(uid, uuid);
}

InputElementKeeper* InputElementKeeper::get(bool create)
{
    if (!m_instance && create) {
        m_instance = new InputElementKeeper; 
    }
    return m_instance;
}

void InputElementKeeper::addRef()
{
    ++m_refCount;
}

void InputElementKeeper::decRef()
{
    if (0 == --m_refCount) {
        delete this;
    }
}
