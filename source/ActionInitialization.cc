//********************************************************************
// \file ActionInitialization.cc
// \brief Initializes and registers all user actions                
//********************************************************************

#include "ActionInitialization.hh"

// User actions
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4RunManager.hh"

// Geant4
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4VUserActionInitialization.hh"


// Constructor 
ActionInitialization::ActionInitialization(G4double beamEnergy)
: G4VUserActionInitialization(),
  fBeamEnergy(beamEnergy)
{}

// Destructor
ActionInitialization::~ActionInitialization() {
}


// Master thread actions

void ActionInitialization::BuildForMaster() const
{
    RunAction* runAction = new RunAction();
    runAction->SetBeamEnergy(fBeamEnergy);

    G4int nEvents = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
    runAction->SetNumEvents(nEvents);
    
    SetUserAction(runAction);
}

// Worker threads actions
void ActionInitialization::Build() const
{

    // Primary generator
    auto* generator = new PrimaryGeneratorAction(fBeamEnergy);
    SetUserAction(generator);

    // Run action
    auto* runAction = new RunAction();
    runAction->SetBeamEnergy(fBeamEnergy);
        
    G4int nEvents = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
    runAction->SetNumEvents(nEvents);
    
    SetUserAction(runAction);

    // Event action
    auto* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    // Stepping action
    auto* steppingAction = new SteppingAction(eventAction);
    SetUserAction(steppingAction);

    // Stacking action
    //auto* stackingAction = new StackingAction();
    //SetUserAction(stackingAction);
}

