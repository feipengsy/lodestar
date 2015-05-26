#include "RootIOStream/RootInputStream.h"
#include "SniperKernel/SniperLog.h"
#include "EDMUtil/InputElementKeeper.h"
#include "RootIOUtil/RootInterface.h"
#include "Event/EventObject.h"

#include "TFile.h"
#include "TTree.h"

RootInputStream::RootInputStream(const std::string& path, const std::vector<std::string>& fileList) 
    : m_path(path)
    , m_files(fileList)
    , m_tree(0)
    , m_file(0)
    , m_fileEntries(0)
    , m_fileEntry(-1)
    , m_streamEntry(-1)
    , m_fileIndex(-1)
    , m_addr(0)
    , m_keeper(InputElementKeeper::get(true))
{
    this->newFile(0);
}

RootInputStream::~RootInputStream()
{
    if (m_file && m_file->IsOpen()) {
         m_file->Close();
         delete m_file;
    }
}

void RootInputStream::newFile(int fileIndex)
{
    if (m_file && m_file->IsOpen()) {
        m_file->Close();
        delete m_file;
    }
    m_file = RootInterface::openFile(m_files[fileIndex]);
    if (!m_file->IsOpen()) {
        LogError << "Failed to open file: " << m_files[fileIndex]
                 << std::endl;
        return;
    }
    m_tree = RootInterface::getHeaderTree(m_path, m_file);
    if (!m_tree) {
        LogError << "Failed to get Header Tree of path: " << m_path
                 << ", file: " << m_files[0]
                 << std::endl;
        return;
    }
    static_cast<TBranch*>(m_tree->GetListOfBranches()->At(0))->SetAddress(&m_addr);
    m_fileEntries = m_tree->GetEntries();
    m_keeper->registerFile(m_files[fileIndex], m_file);
    if (-1 != m_fileIndex) {
        m_keeper->eraseFile(m_files[m_fileIndex]);
    }
    m_fileIndex = fileIndex;
}

bool RootInputStream::next(unsigned int nsteps, bool read)
{
    if (!m_tree) {
        LogError << "No current tree, can't go forward"
                 << std::endl;
        return false;
    }

    LogDebug << "Moving forward for " << nsteps << " step(s)"
             << std::endl;

    while (nsteps) {

        // must leave current file?
        if (m_fileEntry + nsteps >= m_fileEntries) {

            // burn what steps current file provides
            int jump =  m_fileEntries - (m_fileEntry + 1);
            nsteps -= jump;
            m_streamEntry += jump;
            if (m_fileIndex >= m_files.size() - 1) {
                LogDebug << "Already at last file, cannot go next"
                         << std::endl;
                return false;
            }
            this->newFile(m_fileIndex + 1);
            continue;
        }

        // Current nav tree has enough entries left
        m_fileEntry += nsteps;
        m_streamEntry += nsteps;
        nsteps = 0;
        break;
    }
    LogDebug  << "Now at stream entry " << m_streamEntry
              << " (file: " << m_files[m_fileIndex] << " file entry: " << m_fileEntry << ")"
              << std::endl;
    if (read) return this->read();
    return true;
}

bool RootInputStream::prev(unsigned int nsteps, bool read)
{
    if (!m_tree) {
        LogError << "No current tree, can't go backward"
                 << std::endl;
        return false;
    }

    LogError << "Moving forward for " << nsteps << "step(s)"
             << std::endl;

    while (nsteps) {

        // must leave current file?
        if (m_fileEntry-nsteps < 0) {

            // Burn what steps this file provides
            int jump = 1 + m_fileEntry;
            nsteps -= jump;
            m_streamEntry -= jump;
            if (0 == m_fileIndex) {
                LogError <<  "Already at first file, cannot go prev"
                         << std::endl;
                return false;
            }
            this->newFile(m_fileIndex - 1);
            continue;
        }

        // Can stay in current file
        m_fileEntry -= nsteps;
        m_streamEntry -= nsteps;
        nsteps = 0;
        break;
    }

    LogDebug << "Now at stream entry " << m_streamEntry
             << " (file: " << m_files[m_fileIndex] << " file entry: " << m_fileEntry << ")"
             << std::endl;

    if (read) return this->read();
    return true;
}

bool RootInputStream::first(bool read)
{
    m_streamEntry = 0;
    m_fileEntry = 0;
    if (0 != m_fileIndex) {
        this->newFile(0);
    }

    LogDebug << "Now at stream entry " << m_streamEntry
             << " (file: " << m_files[m_fileIndex] << " file entry: " << m_fileEntry << ")"
             << std::endl;

    if (read) return this->read();
    return true;
}

bool RootInputStream::last(bool read)
{
    if (m_fileIndex != m_files.size() - 1) {
        this->newFile(m_files.size() - 1);
    }
    m_fileEntry = m_fileEntries - 1;

    LogDebug << "Now at last stream entry of file: " << m_files[m_fileIndex]
             << ". And stream entry will no longer be valid."
             << std::endl;

    if (read) return this->read();
    return true;
}

bool RootInputStream::read()
{
    m_addr = 0;
    if (!m_tree) {
        LogError << "No current nav tree"
                 << std::endl;
        return false;
    }

    bool ok = m_tree->GetEntry(m_fileEntry);
    LogDebug << "Read event data at entry " << m_streamEntry
             << " from file " << m_files[m_fileIndex]
             << std::endl;
    return ok;
}

EventObject* RootInputStream::get()
{
    if (!m_addr) {
        LogError << "No data is loaded" << std::endl;
    }
    return static_cast<EventObject*>(m_addr);
}
