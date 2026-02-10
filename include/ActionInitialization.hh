#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "globals.hh"
#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization(G4double beamEnergy);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    G4double fBeamEnergy;
};

#endif

