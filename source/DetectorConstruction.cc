//********************************************************************
// \file DetectorConstruction.cc
// \brief Defines the detector geometry, materials, sensitive volumes 
//  and physical regions.               
//********************************************************************

#include "DetectorConstruction.hh"

// Geant4
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"

// Constructor
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(nullptr)
{}

// Destructor
DetectorConstruction::~DetectorConstruction() {}

// Construct the detector
G4VPhysicalVolume* DetectorConstruction::Construct()
{

    // MATERIALS
    // Definition using NIST database
    G4NistManager* nist = G4NistManager::Instance();
    
    // Material for the world
    G4Material* worldMat  = nist->FindOrBuildMaterial("G4_AIR");

    // Material for the target
    G4Material* targetMat = nist->FindOrBuildMaterial("G4_WATER"); 

    // DIMENSIONS
    G4double envSizeXY = 5.0 * cm;
    G4double envSizeZ  = 50.0 * cm;

    // WORLD DIMENSIONS
    // (20% larger than the target volume)
    G4double worldSizeXY = 1.2 * envSizeXY;
    G4double worldSizeZ  = 1.2 * envSizeZ;

    // WORLD VOLUME
    // Box 6.0 x 6.0 x 60.0 cm
    auto solidWorld = new G4Box(
        "World",
        0.5 * worldSizeXY,
        0.5 * worldSizeXY,
        0.5 * worldSizeZ
    );

    // Logic World
    auto logicWorld = new G4LogicalVolume(
        solidWorld,   // Solid object
        worldMat,
        "World"
    );

    // Physics World
    auto physWorld = new G4PVPlacement(
        nullptr,
        G4ThreeVector(),   // At origin (0,0,0)
        logicWorld,        // Logical volume
        "World",
        nullptr,           // This is the root volume
        false, 
        0,
        true
    );

    // TARGET VOLUME
    // Box 5.0 x 5.0 x 50.0 cm
    auto solidTarget = new G4Box(
        "Target",
        0.5 * envSizeXY,
        0.5 * envSizeXY,
        0.5 * envSizeZ
    );

    auto logicTarget = new G4LogicalVolume(
        solidTarget,        // Solid object
        targetMat,
        "Target"
    );

    new G4PVPlacement(
        nullptr,
        G4ThreeVector(),   // At origin
        logicTarget,       // Logical Volume
        "Target",
        logicWorld,        // Mother volume
        false,
        0,
        true
    );
    
    // Region for target volume
    G4Region* targetRegion = new G4Region("rtarget");
    targetRegion->AddRootLogicalVolume(logicTarget);

    // Define scoring volume
    fScoringVolume = logicTarget;

    return physWorld;
}

// Accesor for the scoring volume
// G4LogicalVolume* DetectorConstruction::GetScoringVolume() const
// {
//    return fScoringVolume;
// }

