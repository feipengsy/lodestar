#include "EDMUtil/BookEDM.h"
#include "EDMUtil/EDMManager.h"

BookEDM::BookEDM(const std::string& headerName, const std::string& eventName, int priority, const std::string& defaultPath)
{
    EDMManager::get()->book(headerName, eventName, priority, defaultPath);
}
