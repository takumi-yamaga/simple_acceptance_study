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
/// \file of ParticleHit.hh
/// \brief Definition of the ParticleHit class

#ifndef ParticleHit_h
#define ParticleHit_h 1

#include "TrackInformation.hh"

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class ParticleHit : public G4VHit
{
  public:
    ParticleHit();
    ParticleHit(const ParticleHit &right);
    ParticleHit(const G4Track* track);
    virtual ~ParticleHit();

    const ParticleHit& operator=(const ParticleHit &right);
    G4bool operator==(const ParticleHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Print();

    // setter -----------------------------------------------------------------
    inline void SetParticleHit(const G4Track* track);
    inline void SetGeneration(const G4int input){ generation_=input; }
    inline void SetTrackID(const G4int input){ track_id_=input; }
    inline void SetParentID(const G4int input){ parent_id_=input; }
    inline void SetParticleName(const G4String input){ particle_name_=input; }
    inline void SetInitialMomentum(const G4ThreeVector input){ initial_momentum_=input; }
    inline void SetInitialPosition(const G4ThreeVector input){ initial_position_=input; }
    // getter -----------------------------------------------------------------
    inline G4int GetGeneration() const { return generation_; }
    inline G4int GetTrackID() const { return track_id_; }
    inline G4int GetParentID() const { return parent_id_; } inline G4String GetParticleName() const { return particle_name_; }
    inline G4ThreeVector GetInitialMomentum() const { return initial_momentum_; }
    inline G4ThreeVector GetInitialPosition() const { return initial_position_; }
    // ------------------------------------------------------------------------
    
  private:
    // hit particle
    G4int generation_;
    G4int track_id_;
    G4int parent_id_;
    G4String particle_name_;
    G4ThreeVector initial_momentum_;
    G4ThreeVector initial_position_;
};

using ParticleHitsCollection = G4THitsCollection<ParticleHit>;

extern G4ThreadLocal G4Allocator<ParticleHit>* ParticleHitAllocator;

inline void* ParticleHit::operator new(size_t)
{
  if (!ParticleHitAllocator) {
    ParticleHitAllocator = new G4Allocator<ParticleHit>;
  }
  return (void*)ParticleHitAllocator->MallocSingle();
}

inline void ParticleHit::operator delete(void* aHit)
{
  ParticleHitAllocator->FreeSingle((ParticleHit*) aHit);
}

inline void ParticleHit::SetParticleHit(const G4Track* track)
{
  TrackInformation* track_information = (TrackInformation*)track->GetUserInformation();
  generation_ = (G4int)track_information->GetParentTrackIDs().size();
  track_id_ = track_information->GetTrackID();
  parent_id_ = track_information->GetParentID();
  particle_name_ = track_information->GetParticleName();
  initial_momentum_ = track_information->GetInitialMomentum();
  initial_position_ = track_information->GetInitialPosition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
