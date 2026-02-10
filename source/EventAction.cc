//********************************************************************
// \file EventAction.cc
// \brief Runs at the beginning and end of each event. 
// Including event-by-event data initialization and collection.                
//********************************************************************

// User libraries

#include "EventAction.hh"
#include "RunAction.hh"

// Geant4 libraries
#include "G4Event.hh"
#include "G4RunManager.hh"

// Constructor
// Initializes event action and event data
EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),  // RunAction pointer for accumulating results
  fEdep(0.),
  fPromptGammaCount(0) 
{}

// Destructor
EventAction::~EventAction() {}

// Begin of event
// Reset event variables at the start of each event
void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
    fEdep = 0.;
    fPromptGammaCount = 0;
 } 
 
// End of event
void EventAction::EndOfEventAction(const G4Event* /*event*/)
{
    // Accumulate event statistics in RunAction
    if (fRunAction) {
        fRunAction->AddEdep(fEdep);
        fRunAction->AddPromptGammas(fPromptGammaCount);
    }
}

// Stepping Action
void EventAction::AddPromptGamma(G4double energy, G4double time, G4ThreeVector pos, G4String mat, G4String proc)
{
    ++fPromptGammaCount; // Counter
    
    // Store info in a histogram H1
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillH1(0, energy); 
    
    // Update RunAction with the new gamma count
    if (fRunAction) {
        fRunAction->AddPromptGammas(1); 
    }
}

