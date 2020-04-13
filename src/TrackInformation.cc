/// \file src/TrackInformation.cc
/// \brief Implementation of the TrackInformation class

#include "TrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"    

G4ThreadLocal G4Allocator<TrackInformation> * aTrackInformationAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackInformation::TrackInformation()
  : G4VUserTrackInformation(),
  track_id_(-1),
  parent_id_(-1),
  particle_name_(""),
  initial_momentum_(0),
  initial_position_(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackInformation::TrackInformation(const G4Track* track)
  : G4VUserTrackInformation()
{
  track_id_ = track->GetTrackID();
  parent_id_ = track->GetParentID();
  particle_name_ = track->GetParticleDefinition()->GetParticleName();
  initial_momentum_ = track->GetMomentum();
  initial_position_ = track->GetPosition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackInformation::TrackInformation(const TrackInformation* track_information)
  : G4VUserTrackInformation()
{
  track_id_ = track_information->track_id_;
  parent_id_ = track_information->parent_id_;
  particle_name_ = track_information->particle_name_;
  initial_momentum_ = track_information->initial_momentum_;
  initial_position_ = track_information->initial_position_;
  parent_track_ids_ = track_information->parent_track_ids_;
  parent_parent_ids_ = track_information->parent_parent_ids_;
  parent_particle_names_ = track_information->parent_particle_names_;
  parent_initial_momenta_ = track_information->parent_initial_momenta_;
  parent_initial_positions_ = track_information->parent_initial_positions_;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackInformation::~TrackInformation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  TrackInformation& TrackInformation
::operator =(const TrackInformation& track_information)
{
  track_id_ = track_information.track_id_;
  parent_id_ = track_information.parent_id_;
  particle_name_ = track_information.particle_name_;
  initial_momentum_ = track_information.initial_momentum_;
  initial_position_ = track_information.initial_position_;
  parent_track_ids_ = track_information.parent_track_ids_;
  parent_parent_ids_ = track_information.parent_parent_ids_;
  parent_particle_names_ = track_information.parent_particle_names_;
  parent_initial_momenta_ = track_information.parent_initial_momenta_;
  parent_initial_positions_ = track_information.parent_initial_positions_;

  return *this;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackInformation::Print() const
{
  G4cout << "track_id : " << track_id_ << " (" << particle_name_ << ")" << G4endl;
  G4cout << "total parent particles : " << parent_track_ids_.size() << G4endl;
}

