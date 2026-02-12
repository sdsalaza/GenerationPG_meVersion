### Prompt Gamma Generation Simulation
Geant4 simulation of Prompt Gamma emission from proton beam interacting with a target.  
- Target dimensions: Box of 5.0 x 5.5 x 50.0 cm.
- The material is water but can be changed to other tissue-equivalent materials such as:
  - G4_BONE_CORTICAL_ICRP
  - G4_A-150_TISSUE
- The same target acts as the sensitive detector.
- Protons are shot from one end of the target.

#### Run
With Geant4 setup correctly, you cab compile the code and run a single energy simulation using:  
 `./simulation -e 'energy' -n 'numberProtons' -t 'threads'`  
 This command shoots `n` protons with `e` MeV of energy using `t` threads.  
 
If you want a energy scan:  
 `./simulation -e 'energyMin' -emax 'energyMax' -estep 'energyStep' -n 'numberProtons' -t 'threads'`  
Runs simulations from `energyMin` MeV to `energyMax` MeV in steps of `energyStep` MeV

Even if you want multiple runs for statistical purposes :  
 `./simulation -e 'energy' -n 'numberProtons' -runs 'numberRuns'-t 'threads'`  
Executes `numberRuns` independent runs at `e` MeV with different random seeds

For interactive mode (GUI and 3D visualization):  
`./simulation`
*You require a Geant4 build with Qt5_Enable*

#### Output
The end of the simulation generates an outputfile named: `results_<energy>Mev_<numberProtons>events.root`  
This file contains a histogram `E_PG` that you can use to view the prompt gamma energy spectrum.  

*It is setup with 1000 bins of 0-10 MeV range but you can modify these values as you needed.*

It also contains a NTuple `prompt_gamma_run` summarizing data in columns:  
  -`runnID`: Run identification number
  - `total_prompt_gammas`: Total number of prompt gammas detected
  - `total_edep`: Total energy deposited (MeV)
  - `primary_energy_MeV`: Beam energy (MeV)
  - `n_events`: Number of events simulated
