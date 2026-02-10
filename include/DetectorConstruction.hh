#ifndef DETECTOR_CONSTRUCTION_HH
#define DETECTOR_CONSTRUCTION_HH

// Geant4 libraries

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"

// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction {

public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct() override;

    // Access to target logical volume
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume;}

private:
    G4LogicalVolume* fScoringVolume = nullptr;
};

#endif

