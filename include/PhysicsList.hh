#ifndef PHYSICS_HH
#define PHYSICS_HH

// Geant4 libraries

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonPhysics.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"

// Physics list class

class physics : public G4VModularPhysicsList {

public:
    physics();
    virtual ~physics();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
    virtual void SetCuts() override;

private:
    G4int uVerbose = 1;

    G4VPhysicsConstructor* plEM;
    G4VPhysicsConstructor* plEMextra;

    G4VPhysicsConstructor* plDecay;
    G4VPhysicsConstructor* plRadioactiveDecay;

    G4VPhysicsConstructor* plHadronElastic;
    G4VPhysicsConstructor* plHadronInelastic;

    G4VPhysicsConstructor* plStopping;

    G4VPhysicsConstructor* plIonElastic;
    G4VPhysicsConstructor* plIonInelastic;
};

#endif

