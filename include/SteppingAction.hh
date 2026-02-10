#ifndef SteppingAction_h
#define SteppingAction_h

// Geant4 libraries
#include "G4UserSteppingAction.hh"
#include "globals.hh"

// Forward declarations
class EventAction;
class G4Step;

// SteppingAction
class SteppingAction : public G4UserSteppingAction {

public:
    // Constructor: with the EventAction
    explicit SteppingAction(EventAction* eventAction);
    ~SteppingAction() override;

    // Calls in each step
    void UserSteppingAction(const G4Step* step) override;

private:
    EventAction* fEventAction; 
};

#endif
