//********************************************************************
// \file PhysicsList.cc
// \brief All the physics processes and models used.
// As well as production cuts.
//********************************************************************

// User libraries
#include "PhysicsList.hh"

// Geant4 libraries
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4NuclearLevelData.hh"
#include "G4DeexPrecoParameters.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4EmParameters.hh"

// Constructor / Destructor
physics::physics()
: G4VModularPhysicsList()
{
    SetVerboseLevel(uVerbose);
    plEM       = new G4EmStandardPhysics_option4(uVerbose);
    plEMextra = new G4EmExtraPhysics(uVerbose);

    plDecay             = new G4DecayPhysics(uVerbose);
    plRadioactiveDecay  = new G4RadioactiveDecayPhysics(uVerbose);

    plHadronElastic     = new G4HadronElasticPhysicsHP(uVerbose);
    plHadronInelastic   = new G4HadronPhysicsQGSP_BIC_HP(uVerbose);

    plStopping          = new G4StoppingPhysics(uVerbose);

    plIonElastic        = new G4IonElasticPhysics(uVerbose);
    plIonInelastic      = new G4IonPhysics(uVerbose);
}

physics::~physics() {}


// Particle definitions
void physics::ConstructParticle()
{
    G4BosonConstructor      pcBosons;       pcBosons.ConstructParticle();
    G4LeptonConstructor     pcLeptons;      pcLeptons.ConstructParticle();
    G4MesonConstructor      pcMesons;       pcMesons.ConstructParticle();
    G4BaryonConstructor     pcBaryons;      pcBaryons.ConstructParticle();
    G4IonConstructor        pcIons;         pcIons.ConstructParticle();
    G4ShortLivedConstructor pcShortLived;   pcShortLived.ConstructParticle();
}


// Process definitions

void physics::ConstructProcess()
{
    AddTransportation();

    // Modular physics
    if (plEM)                plEM->ConstructProcess();
    if (plEMextra)           plEMextra->ConstructProcess();
    if (plDecay)             plDecay->ConstructProcess();
    if (plRadioactiveDecay)  plRadioactiveDecay->ConstructProcess();
    if (plHadronElastic)     plHadronElastic->ConstructProcess();
    if (plHadronInelastic)   plHadronInelastic->ConstructProcess();
    if (plStopping)          plStopping->ConstructProcess();
    if (plIonElastic)        plIonElastic->ConstructProcess();
    if (plIonInelastic)      plIonInelastic->ConstructProcess();


    // Nuclear Deexcitation 
    G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();

    deex->SetMaxLifeTime(1000*s); 
    deex->SetCorrelatedGamma(true);
    deex->SetStoreICLevelData(true);     
    deex->SetInternalConversionFlag(true); 
    deex->SetMinExcitation(0.1*keV);


    // Atomic
    G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    de->SetFluo(true);
    de->SetAuger(false);
    de->SetPIXE(false);
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);

    //  EM 
    G4EmParameters* params = G4EmParameters::Instance();
    params->SetDeexcitationIgnoreCut(true);

    // Target Region
    G4Region* targetRegion = G4RegionStore::GetInstance()->GetRegion("rtarget");
    if (targetRegion) {
        params->SetDeexActiveRegion("rtarget", true, false, false);
    }
}

// Cuts

void physics::SetCuts()
{
    // Default = 1 mm
    SetCutsWithDefault();

    G4double cut = 1.0 * mm;

    SetCutValue(cut, "e-");
    SetCutValue(cut, "e+");
    SetCutValue(cut, "proton");
}

