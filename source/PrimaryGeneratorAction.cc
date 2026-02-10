//********************************************************************
// \file PrimaryGeneratorAction.cc
// \brief Defines the primary particle source.
//********************************************************************

#include "PrimaryGeneratorAction.hh"

// Geant4
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"


// Constructor
PrimaryGeneratorAction::PrimaryGeneratorAction(G4double beamEnergy)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(nullptr),
  fBeamEnergy(beamEnergy) //Initializes with beamEnergy
{
    // One primary particle per event
    fParticleGun = new G4ParticleGun(1);

    // Define particle type: proton
    G4ParticleDefinition* particle =
        G4ParticleTable::GetParticleTable()->FindParticle("proton");

    fParticleGun->SetParticleDefinition(particle);

    // Fixed beam direction (+Z)
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    
    // Set the energy in MeV
    fParticleGun->SetParticleEnergy(fBeamEnergy * MeV);
}


// Destructor
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}


// Generate primaries
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    // Beam energy
    // Kinetic energy of the proton
    //fParticleGun->SetParticleEnergy(fBeamEnergy * MeV);

    // Beam position
    // Uniform distribution in X and Y
    const G4double beamSize = 5.0 * cm; // Beam of 5.0 cm x 5.0 cm

    // Randomly generate the position within the beam size in X and Y 
    G4double x0 = (G4UniformRand() - 0.5) * beamSize;
    G4double y0 = (G4UniformRand() - 0.5) * beamSize;

    // Fixed Z position (before the target)
    G4double z0 = -25.0 * cm;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // Generate the primary vertex
    fParticleGun->GeneratePrimaryVertex(event);
}

