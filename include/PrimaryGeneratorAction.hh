#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(G4double beamEnergy);
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);
    
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
    G4ParticleGun* fParticleGun;
    G4double       fBeamEnergy;
};

#endif

