//********************************************************************
// \file TrackingAction.cc
// \brief Defines actions performed at the start and end of tracking 
// for each particle track.
//********************************************************************

#include "TrackingAction.hh"

// Geant4 libraries
#include "G4Track.hh"

// Constructor 

TrackingAction::TrackingAction()
: G4UserTrackingAction()
{}

// Destructor
TrackingAction::~TrackingAction() {}

// Called at the beginning of each track
void TrackingAction::PreUserTrackingAction(const G4Track* /*track*/)
{}


// Called at the end of each track
void TrackingAction::PostUserTrackingAction(const G4Track* /*track*/)
{}

