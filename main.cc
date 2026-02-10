// Std libraries
#include <iostream>
#include <set>
#include <chrono>
#include <string>
#include <ctime>

// Geant4 libraries
#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4Threading.hh"
#include "Randomize.hh"

// User libraries
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "RunAction.hh"

// Main program
int main(int argc, char** argv)
{
    // Default values
    G4int nThreads = 1;
    G4int nEvents = 1000;
    G4double energyMin = 100.0;
    G4double energyMax = -1.0; // -1 Single energy
    G4double energyStep = 10.0;
    G4int nRunsPerEnergy = 1;
    G4int userSeed = 0;
    G4String macro = "";
    G4bool uiMode = true;
  
    if (argc > 1) uiMode = false;
    
    // Bucle for arguments
    // -e energy -n events -t thread -m macro
    // Single energy ./simulation -e 100 -n 5000
    // A range of energies ./simulation -e 70 -emax 150 -estep 10 -n 1000
    for (int i = 1; i < argc; i++) {
        G4String arg = argv[i];
        if (arg == "-e")      energyMin = std::stod(argv[++i]);
        else if (arg == "-emax")  energyMax = std::stod(argv[++i]);
        else if (arg == "-estep") energyStep = std::stod(argv[++i]);
        else if (arg == "-n")      nEvents = std::stoi(argv[++i]);
        else if (arg == "-t")      nThreads = std::stoi(argv[++i]);
        else if (arg == "-runs")   nRunsPerEnergy = std::stoi(argv[++i]);
        else if (arg == "-seed")   userSeed = std::stoi(argv[++i]);
        else if (arg == "-m")    { macro = argv[++i]; uiMode = false; }
    }
    
    // If emax is not defined -> single energy mode
    if (energyMax < 0) energyMax = energyMin;

  // Run manager
  #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(nThreads);
  #else
    G4RunManager* runManager = new G4RunManager();
  #endif
  
  //Initialization
  
  // Detector
  runManager->SetUserInitialization(new DetectorConstruction());
  
  // Physics
  runManager->SetUserInitialization(new physics());
  
  // User actions
  runManager->SetUserInitialization(new ActionInitialization(energyMin));
  
  // Kernel initialization
  runManager->Initialize();
  
  // Visualization and UI
  
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
  G4UImanager* uiManager = G4UImanager::GetUIpointer();

    if (!uiMode) {
        // Batch mode
        G4Random::setTheSeed(time(NULL)); 
        
        // Execute macro file if provided
        if (macro != "") {
            G4String command = "/control/execute ";
            uiManager->ApplyCommand(command + macro);
        } else {
        // Energy scan loop
            for (G4double en = energyMin; en <= energyMax; en += energyStep) {
                // Multiple runs per energy point
                for (G4int r = 0; r < nRunsPerEnergy; ++r) {
                    // Seed logic: -0 default, real time
                    G4long seed = (userSeed == 0) ? (time(nullptr) + r) : (userSeed + r);
                    G4Random::setTheSeed(seed);
                    
                    G4cout << "\n>>> Processing: " << en << " MeV | Run: " << r << " | Seed: " << seed << " <<<" << G4endl;

                    // Set particle gun energy
                    uiManager->ApplyCommand("/gun/energy " + std::to_string(en) + " MeV");
                    
                    // Update beam energy in RunAction BEFORE starting the run
        RunAction* runAction = const_cast<RunAction*>(
            static_cast<const RunAction*>(runManager->GetUserRunAction()));
        if (runAction) {
            runAction->SetBeamEnergy(en * MeV);
        }

                    runManager->SetRunIDCounter(r);
                    runManager->BeamOn(nEvents);
                }
            }
        }
    }
    else{
        // Interactive mode
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        uiManager->ApplyCommand("/control/execute init_vis.mac"); 
        ui->SessionStart();
        delete ui;
    }
    
  // Clean up
  delete visManager;
  delete runManager;
  

  // Completion message
  G4cout << "Finished. Energy: " << energyMin << " MeV." << G4endl;
  
  return 0;

}
