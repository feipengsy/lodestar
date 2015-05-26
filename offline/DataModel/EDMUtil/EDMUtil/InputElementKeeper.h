#ifndef INPUT_ELEMENT_KEEPER
#define INPUT_ELEMENT_KEEPER

#include <string>
#include "TObject.h"

class TFile;
class TBranch;
class SmartRefTable;

class InputElementKeeper {

    public:
        ~InputElementKeeper();

        bool registerFile(const std::string& filename, TFile* file);
        void eraseFile(const std::string& filename);
        TBranch* searchBranch(Int_t uid, const std::string& uuid);
        static InputElementKeeper* get(bool create = false);
        void addRef();
        void decRef();

    private:
        // Singleton
        InputElementKeeper();
        // Not supported
        InputElementKeeper(const InputElementKeeper& keeper);
        InputElementKeeper& operator=(const InputElementKeeper& keeper);

    private:
        SmartRefTable*              m_table;
        static InputElementKeeper*  m_instance;
        int                         m_refCount;
};

#endif
