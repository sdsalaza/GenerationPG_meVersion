//********************************************************************
// \file SteppingAction.cc
// \brief Defines actions executed at each particle step.
// i.e. Interactions, energy deposition and step-level conditions.
//********************************************************************

// STD libraries
#include <set>

//Geant4 libraries
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

// Constructor
// Links SteppingAction to the EventAction
SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

// Destructor
SteppingAction::~SteppingAction() = default;

// Method called for each step of the particle
void SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Get the track associated with the current step
    G4Track* track = step->GetTrack();
  
    // Only gammas
    if (track->GetDefinition() != G4Gamma::Definition())
        return;

    // Only the particle's creation step
    if (track->GetCurrentStepNumber() != 1)
        return;

    // Get the global time of the gamma
    const G4double time_ns = track->GetGlobalTime() / ns;
    
    // Ignore gammas created after 10 ns
    if (time_ns > 10.0) 
        return;
        
    // Creator process
    const G4VProcess* creator = track->GetCreatorProcess();
    if (!creator)
        return;
        
    // Get the type and process name that created the gamma        
    G4int processType = creator->GetProcessType();
    const G4String& processName = creator->GetProcessName();

    // Only hadronic processes or decay
    if (processType == fHadronic || processType == fDecay) {
        
        // Info from the step
        const G4double energy_MeV = track->GetKineticEnergy() / MeV;
        const G4ThreeVector position_mm = step->GetPreStepPoint()->GetPosition() / mm;
        const G4String materialName = step->GetPreStepPoint()->GetMaterial()->GetName();

        // To EventAction
        if (fEventAction) {
            fEventAction->AddPromptGamma(
                energy_MeV,
                time_ns,
                position_mm,
                materialName,
                processName
            );
        }
    }
}
