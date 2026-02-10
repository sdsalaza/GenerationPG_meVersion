#ifndef RUNACTION_HH
#define RUNACTION_HH

// Geant4 libraries

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"


// Class
class G4Run;

class RunAction : public G4UserRunAction {

public:
    RunAction();
    virtual ~RunAction();

    virtual G4Run* GenerateRun() override;
    virtual void BeginOfRunAction(const G4Run* run) override;
    virtual void EndOfRunAction(const G4Run* run) override;

    // Interface used by EventAction
    void AddPromptGammas(G4int n);
    void AddEdep(G4double edep);
    void SetBeamEnergy(G4double e) { fBeamEnergy = e; }
    void SetNumEvents(G4int n)    { fNumEvents = n; }

private:
    G4int    fTotalPromptGammas;
    G4double fEdep;
    G4int fNumEvents;     
    G4double fBeamEnergy; 
};

#endif

