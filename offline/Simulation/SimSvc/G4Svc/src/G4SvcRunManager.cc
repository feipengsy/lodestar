#include "G4SvcRunManager.h"

G4SvcRunManager::G4SvcRunManager()
    : G4RunManager()
{
}

G4SvcRunManager::~G4SvcRunManager()
{
}

bool G4SvcRunManager::RunInitialize()
{
    G4bool cond = this->ConfirmBeamOnCondition();
    if (!cond) 
        return false;

    this->ConstructScoringWorlds();
    this->RunInitialization();
    return true;
}

bool G4SvcRunManager::SimulateEvent(int i)
{
    this->currentEvent = this->GenerateEvent(i);
    this->eventManager->ProcessOneEvent(currentEvent);
    this->AnalyzeEvent(currentEvent);
    this->UpdateScoring();
    this->StackPreviousEvent(currentEvent);
    this->currentEvent = 0;

    if (this->runAborted)
        return false;
    return true;
}

bool G4SvcRunManager::RunTerminate()
{
    this->RunTermination();
    eturn true;
}
