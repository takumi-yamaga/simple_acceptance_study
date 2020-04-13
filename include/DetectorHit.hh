//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file of DetectorHit.hh
/// \brief Definition of the DetectorHit class

#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "SegmentHit.hh"
#include "ParticleHit.hh"
#include "TrackInformation.hh"

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <vector>

class DetectorHit : public G4VHit
{
  public:
    DetectorHit();
    DetectorHit(const DetectorHit &right);
    virtual ~DetectorHit();

    const DetectorHit& operator=(const DetectorHit &right);
    G4bool operator==(const DetectorHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual void Print();

    // setter -----------------------------------------------------------------
    inline void SetSegmentHit(const G4VTouchable* touchable);
    inline void SetPresentParticle(const G4Track* track);
    inline void PushDaughterParticle(const G4Track* track);
    inline void SetIncidentMomentum(const G4ThreeVector input) { incident_momentum_ = input; }
    inline void PushHitTime(G4double input) { hit_times_.push_back(input); }
    inline void PushEnergyDeposit(G4double input) { energy_deposits_.push_back(input); }
    inline void PushHitPosition(G4ThreeVector input) { hit_positions_.push_back(input); }
    // getter -----------------------------------------------------------------
    inline SegmentHit* GetSegmentHit() const { return segment_hit_; }
    inline ParticleHit* GetPresentParticle() const { return present_particle_; }
    inline std::vector <ParticleHit*> GetParentParticles() const { return parent_particles_; }
    inline std::vector <ParticleHit*> GetDaughterParticles() const { return daughter_particles_; }
    inline G4ThreeVector GetIncidentMomentum() const { return incident_momentum_; }
    inline std::vector <G4double> GetHitTimes() const { return hit_times_; }
    inline std::vector <G4double> GetEnergyDeposits() const { return energy_deposits_; }
    inline std::vector <G4ThreeVector> GetHitPositions() const { return hit_positions_; }
    // ------------------------------------------------------------------------

  private:
    // segment information
    SegmentHit* segment_hit_;
    // particle informations
    ParticleHit* present_particle_;
    std::vector <ParticleHit*> parent_particles_;
    std::vector <ParticleHit*> daughter_particles_;
    // hit informations
    G4ThreeVector incident_momentum_;
    std::vector <G4double> hit_times_;
    std::vector <G4double> energy_deposits_;
    std::vector <G4ThreeVector> hit_positions_;
};

using DetectorHitsCollection = G4THitsCollection<DetectorHit>;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo....//
// inline functions  //
//....oooOO0OOooo....//

// new & delete -----------------------------------------------------
inline void* DetectorHit::operator new(size_t)
{
  if (!DetectorHitAllocator) {
    DetectorHitAllocator = new G4Allocator<DetectorHit>;
  }
  return (void*)DetectorHitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void* aHit)
{
  DetectorHitAllocator->FreeSingle((DetectorHit*) aHit);
}
// segment hit ------------------------------------------------------
inline void DetectorHit::SetSegmentHit(const G4VTouchable* touchable){
  if(touchable){
    if(segment_hit_)  delete segment_hit_;
    segment_hit_ = new SegmentHit(touchable);
  }
  else{
    G4ExceptionDescription msg;
    msg << "No touchable" << G4endl; 
    G4Exception("DetectorHit::SetSegmentHit()", "", EventMustBeAborted, msg);
  }
}
// present particle -------------------------------------------------
inline void DetectorHit::SetPresentParticle(const G4Track* track){
  if(track){
    if(present_particle_)  delete present_particle_;
    present_particle_ = new ParticleHit(track);
    TrackInformation* track_information = (TrackInformation*)track->GetUserInformation();
    for(G4int generation = 0; generation<present_particle_->GetGeneration(); generation++){
      ParticleHit* parent_particle = new ParticleHit();
      parent_particle->SetGeneration(generation);
      parent_particle->SetTrackID(track_information->GetParentTrackIDs()[generation]);
      parent_particle->SetParentID(track_information->GetParentParentIDs()[generation]);
      parent_particle->SetParticleName(track_information->GetParentParticleNames()[generation]);
      parent_particle->SetInitialMomentum(track_information->GetParentInitialMomenta()[generation]);
      parent_particle->SetInitialMomentum(track_information->GetParentInitialMomenta()[generation]);
      parent_particle->SetInitialPosition(track_information->GetParentInitialPositions()[generation]);
      parent_particles_.push_back(parent_particle);
    }
  }
  else{
    G4ExceptionDescription msg;
    msg << "No track" << G4endl; 
    G4Exception("DetectorHit::SetPresentParticle()", "", EventMustBeAborted, msg);
  }
}
// daughter article -------------------------------------------------
inline void DetectorHit::PushDaughterParticle(const G4Track* track){
  if(track){
    ParticleHit* daughter_particle = new ParticleHit(track);
    daughter_particles_.push_back(daughter_particle);
  }
  else{
    G4ExceptionDescription msg;
    msg << "No track" << G4endl; 
    G4Exception("DetectorHit::SetDaughterParticle()", "", EventMustBeAborted, msg);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
