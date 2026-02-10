//********************************************************************
// \file RunAction.cc
// \brief Defines actions performed at the beginning and end of each run.
// Collect and save of global simulation results and writing a summary to file
// at the end of each run.
//********************************************************************

// User libraries
#include "RunAction.hh"

// Geant4 libraries
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

// User libraries
#include "PrimaryGeneratorAction.hh"

// Constructor
// Initializes the RunAction variables
RunAction::RunAction()
: G4UserRunAction(),
  fTotalPromptGammas(0),
  fEdep(0.),
  fBeamEnergy(0.)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root"); //  Results file format - root
  analysisManager->SetVerboseLevel(1);
  
  analysisManager->SetNtupleMerging(true); // Important. Enable ntuple merging across runs
  

  // Create a histogram for PG energy spectrum
  // 4000 bins, range [0,10]MeV
  analysisManager->CreateH1("E_PG", "Prompt Gamma Energy Spectrum", 4000, 0., 10.*MeV);
  
  // Ntuple to store summary data for each run  
  analysisManager->CreateNtuple("prompt_gamma_run", "Summary per run");
  analysisManager->CreateNtupleIColumn("runID");                
  analysisManager->CreateNtupleIColumn("total_prompt_gammas");  
  analysisManager->CreateNtupleDColumn("total_edep");           
  
  analysisManager->CreateNtupleDColumn("primary_energy_MeV");   
  analysisManager->CreateNtupleIColumn("n_events");             
  
  analysisManager->FinishNtuple();
}


// Destructor
RunAction::~RunAction() {
}

// Begin of Run
G4Run* RunAction::GenerateRun()
{
    return new G4Run();
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    // Reset accumulated values at the start of each new run
    fTotalPromptGammas = 0;
    fEdep = 0.;
    
    // Get current beam energy from PrimaryGeneratorAction
    const auto* primaryGenerator = static_cast<const PrimaryGeneratorAction*>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    
    if (primaryGenerator) {
        fBeamEnergy = primaryGenerator->GetParticleGun()->GetParticleEnergy();
    }
    
    G4int nEvents = run->GetNumberOfEventToBeProcessed();
    auto analysisManager = G4AnalysisManager::Instance();
    
    // Saving parameters - convert to MeV for filename
    std::string energy = std::to_string(static_cast<int>(fBeamEnergy / MeV));
    std::string numEvents = std::to_string(nEvents);
    
    // Output file name
    std::string baseFileName = "results_" + energy + "MeV_" + numEvents + "events";
    
    // If the name exists, create a new index
    int fileIndex = 1;
    std::string fileName = baseFileName + ".root";
    std::ifstream f(fileName);
    
    while (f.good()) {
        fileIndex++;
        f.close();
        fileName = baseFileName + "_" + std::to_string(fileIndex) + ".root";
        f.open(fileName);
    }
    
    // Open the ROOT file
    analysisManager->OpenFile(fileName);
    
    // Open the ROOT file
    analysisManager->OpenFile(fileName);
}

// End of Run 
void RunAction::EndOfRunAction(const G4Run* run)
{
    auto analysisManager = G4AnalysisManager::Instance();
    G4int nEvents = run->GetNumberOfEventToBeProcessed();

    // Fill run-level Ntuple
    analysisManager->FillNtupleIColumn(0, 0, run->GetRunID());
    analysisManager->FillNtupleIColumn(0, 1, fTotalPromptGammas);
    analysisManager->FillNtupleDColumn(0, 2, fEdep);
    
    analysisManager->FillNtupleDColumn(0, 3, fBeamEnergy);
    analysisManager->FillNtupleIColumn(0, 4, nEvents);

    analysisManager->AddNtupleRow(0);

    // Write & close file
    analysisManager->Write();
    analysisManager->CloseFile();
}

// Interface for RunAction
// Accumulate statistics during the run.
void RunAction::AddPromptGammas(G4int n)
{
    fTotalPromptGammas += n; // Total PG count
}

void RunAction::AddEdep(G4double edep)
{
    fEdep += edep;  // Add the energy deposition to the total
}
