#ifndef I_ANALYSIS_ELEMENT_H
#define I_ANALYSIS_ELEMENT_H 1

#include "G4ClassificationOfNewTrack.hh"

class G4Run;
class G4Event;
class G4Track;
class G4Step;

class IAnalysisElement {

    public:
        virtual ~IAnalysisElement() {}

        // Run Action
        virtual void BeginOfRunAction(const G4Run*) {}
        virtual void EndOfRunAction(const G4Run*) {}

        // Event Action
        virtual void BeginOfEventAction(const G4Event*) {}
        virtual void EndOfEventAction(const G4Event*) {}

        // Stacking Action
        virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack) {}
        virtual void NewStage() {}
        virtual void PrepareNewEvent() {}

        // Tracking Action
        virtual void PreUserTrackingAction(const G4Track*) {}
        virtual void PostUserTrackingAction(const G4Track*) {}

        // Stepping Action
        virtual void UserSteppingAction(const G4Step*) {}
};

#endif
