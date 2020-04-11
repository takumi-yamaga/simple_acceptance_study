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

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <vector>

class SegmentHit;
class ParticleHit;

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

    // segment informations ---------------------------------------------------
    inline void SetSegmentHit(SegmentHit* input) { segment_hit_ = input; }
    inline SegmentHit* GetSegmentHit() const { return segment_hit_; }
    // present particle -------------------------------------------------------
    inline void SetPresentParticle(ParticleHit* input) { present_particle_ = input; }
    inline ParticleHit* GetPresentParticle() const { return present_particle_; }
    // parent particles -------------------------------------------------------
    inline void PushParentParticle(ParticleHit* input) { parent_particles_.push_back(input); }
    inline std::vector <ParticleHit*> GetParentParticles() const { return parent_particles_; }
    inline void SetParentParticle(const G4int i, ParticleHit* input);
    inline ParticleHit* GetParentParticle(const G4int i) const;
    // daughter particles -------------------------------------------------------
    inline void PushDaughterParticle(ParticleHit* input) { daughter_particles_.push_back(input); }
    inline std::vector <ParticleHit*> GetDaughterParticles() const { return daughter_particles_; }
    inline void SetDaughterParticle(const G4int i, ParticleHit* input);
    inline ParticleHit* GetDaughterParticle(const G4int i) const;
    // incident momentum --------------------------------------------------------
    inline void SetIncidentMomentum(const G4ThreeVector input) { incident_momentum_ = input; }
    inline G4ThreeVector GetIncidentMomentum() const { return incident_momentum_; }
    // hit time -----------------------------------------------------------------
    inline void PushHitTime(G4double input) { hit_times_.push_back(input); }
    inline std::vector <G4double> GetHitTimes() const { return hit_times_; }
    inline void SetHitTime(const G4int i, G4double input);
    inline G4double GetHitTime(const G4int i) const;
    // energy deposit -----------------------------------------------------------
    inline void PushEnergyDeposit(G4double input) { energy_deposits_.push_back(input); }
    inline std::vector <G4double> GetEnergyDeposits() const { return energy_deposits_; }
    inline void SetEnergyDeposit(const G4int i, G4double input);
    inline G4double GetEnergyDeposit(const G4int i) const;
    // hit position -------------------------------------------------------------
    inline void PushHitPosition(G4ThreeVector input) { hit_positions_.push_back(input); }
    inline std::vector <G4ThreeVector> GetHitPositions() const { return hit_positions_; }
    inline void SetHitPosition(const G4int i, G4ThreeVector input);
    inline G4ThreeVector GetHitPosition(const G4int i) const;

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
// parent particle --------------------------------------------------
inline void DetectorHit::SetParentParticle(const G4int i, ParticleHit* input){
  if(i<(G4int)parent_particles_.size()){
    parent_particles_[i] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::SetParentParticle()", "", EventMustBeAborted, msg);
  }
}
inline ParticleHit* DetectorHit::GetParentParticle(const G4int i=0) const{
  if(i<(G4int)parent_particles_.size()){
    return parent_particles_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::GetParentParticle(G4int)", "", EventMustBeAborted, msg);
    return nullptr;
  }
}
// daughter particle -------------------------------------------------
inline void DetectorHit::SetDaughterParticle(const G4int i, ParticleHit* input){
  if(i<(G4int)daughter_particles_.size()){
    daughter_particles_[i] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::SetDaughterParticle()", "", EventMustBeAborted, msg);
  }
}
inline ParticleHit* DetectorHit::GetDaughterParticle(const G4int i=0) const{
  if(i<(G4int)daughter_particles_.size()){
    return daughter_particles_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::GetDaughterParticle(G4int)", "", EventMustBeAborted, msg);
    return nullptr;
  }
}
// hit time ----------------------------------------------------------
inline void DetectorHit::SetHitTime(const G4int i, G4double input){
  if(i<(G4int)hit_times_.size()){
    hit_times_[i] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::SetHitTime()", "", EventMustBeAborted, msg);
  }
}
inline G4double DetectorHit::GetHitTime(const G4int i=0) const{
  if(i<(G4int)hit_times_.size()){
    return hit_times_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::GetHitTime(G4int)", "", EventMustBeAborted, msg);
    return -999.;
  }
}
// energy deposit ----------------------------------------------------
inline void DetectorHit::SetEnergyDeposit(const G4int i, G4double input){
  if(i<(G4int)energy_deposits_.size()){
    energy_deposits_[i] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::SetEnergyDeposit()", "", EventMustBeAborted, msg);
  }
}
inline G4double DetectorHit::GetEnergyDeposit(const G4int i=0) const{
  if(i<(G4int)energy_deposits_.size()){
    return energy_deposits_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::GetEnergyDeposit(G4int)", "", EventMustBeAborted, msg);
    return -999.;
  }
}
// hit position ------------------------------------------------------
inline void DetectorHit::SetHitPosition(const G4int i, G4ThreeVector input){
  if(i<(G4int)hit_positions_.size()){
    hit_positions_[i] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::SetHitPosition()", "", EventMustBeAborted, msg);
  }
}
inline G4ThreeVector DetectorHit::GetHitPosition(const G4int i=0) const{
  if(i<(G4int)hit_positions_.size()){
    return hit_positions_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("DetectorHit::GetHitPosition(G4int)", "", EventMustBeAborted, msg);
    return G4ThreeVector(0.);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
