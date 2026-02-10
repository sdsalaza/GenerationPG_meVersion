#ifndef EVENTACTION_HH
#define EVENTACTION_HH

// Geant4 libraries
#include "G4UserEventAction.hh"
#include "G4AnalysisManager.hh"
#include "globals.hh"


// Forward declarations
class RunAction;


// EventAction class
class EventAction : public G4UserEventAction {

public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event) override;
    virtual void EndOfEventAction(const G4Event* event) override;

    // SteppingAction
    void AddPromptGamma(G4double energy, G4double time, 
                    G4ThreeVector pos, G4String mat, G4String proc);

private:
    RunAction* fRunAction;

    G4double fEdep;
    G4int    fPromptGammaCount;
    
};

#endif

