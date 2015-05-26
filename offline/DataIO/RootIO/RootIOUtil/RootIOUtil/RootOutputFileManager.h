#ifndef ROOT_OUTPUT_FILE_MANAGER
#define ROOT_OUTPUT_FILE_MANAGER

#include <map>
#include <string>

class TFile;

class RootOutputFileManager {

    public:
        typedef std::map<std::string, std::pair<int, TFile*> > FileMap;
        
        static RootOutputFileManager* get();
        TFile* newFile(const std::string& name);
        TFile* getFile(const std::string& name);
        void occupyFile(const std::string& name);
        void releaseFile(const std::string& name);
        void addRef();
        void decRef();

    private:
        RootOutputFileManager();
        ~RootOutputFileManager();

    private:
        FileMap m_file;
        int m_refCount;
        static RootOutputFileManager* m_instance;

};

#endif
