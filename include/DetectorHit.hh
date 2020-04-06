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
#include <map>

class SegmentHit;
class ParticleHit;

struct pos_and_de{
  G4ThreeVector pos;
  G4double de;
}

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
    inline void SetSegmentHit(const SegmentHit* input) { segment_hit_ = input; }
    inline SegmentHit* GetSegmentHit() const { return segment_hit_; }
    // present particle -------------------------------------------------------
    inline void SetPresentParticle(const ParticleHit* input) { present_particle_ = input; }
    inline ParticleHit* GetPresentParticle() const { return present_particle_; }
    // parent particles -------------------------------------------------------
    inline void PushParentParticle(const ParticleHit* input) { parent_particles_.push_back(input); }
    inline std::vector <ParticleHit*> GetParentParticles() const { return parent_particles_; }
    inline void SetParentParticle(const G4int i, const ParticleHit* input) const;
    inline ParticleHit* GetParentParticle(const ParticleHit i=0) const;
    // daughter particles -------------------------------------------------------
    inline void PushDaughterParticle(const ParticleHit* input) { daughter_particles_.push_back(input); }
    inline std::vector <ParticleHit*> GetDaughterParticles() const { return daughter_particles_; }
    inline void SetDaughterParticle(const G4int i, const ParticleHit* input) const;
    inline ParticleHit* GetDaughterParticle(const ParticleHit i=0) const;
    // incident momentum --------------------------------------------------------
    inline void SetIncidentMomentum(const G4ThreeVector input) { incident_momentum_ = input; }
    inline G4ThreeVector GetIncidentMomentum() const { return incident_momentum_; }
    // incident position --------------------------------------------------------
    inline void SetIncidentPosition(const G4ThreeVector input) { incident_position_ = input; }
    inline G4ThreeVector GetIncidentPosition() const { return incident_position_; }
    // hit data -----------------------------------------------------------------
    inline void PushHitData(const G4double time, const hit_and_pos data ) { hit_data_.emplace(time,hit_and_pos); }
    inline std::multimap <G4double,pos_and_de> GetHitData() const { return hit_data_; }
    inline G4double GetHitTime() const;
    inline G4double GetTotalEnergyDeposit() const;
    inline G4ThreeVector GetHitPosition() const;

  private:
    // segment information
    SegmentHit* segment_hit_;
    // particle informations
    ParticleHit* present_particle_;
    std::vector <ParticleHit*> parent_particles_;
    std::vector <ParticleHit*> daughter_particles_;
    // hit informations
    G4ThreeVector incident_position_;
    G4ThreeVector incident_momentum_;
    std::multimap <G4double,pos_and_de> hit_data_;
};

using HodoscopeHitsCollection = G4THitsCollection<HodoscopeHit>;

extern G4ThreadLocal G4Allocator<HodoscopeHit>* HodoscopeHitAllocator;

inline void* HodoscopeHit::operator new(size_t)
{
  if (!HodoscopeHitAllocator) {
    HodoscopeHitAllocator = new G4Allocator<HodoscopeHit>;
  }
  return (void*)HodoscopeHitAllocator->MallocSingle();
}

inline void HodoscopeHit::operator delete(void* aHit)
{
  HodoscopeHitAllocator->FreeSingle((HodoscopeHit*) aHit);
}

// daughter_ids -----------------------------------------------------
inline void HodoscopeHit::SetDaughterID(const G4int i, const G4int input){
  if(i<daughter_ids_.size()){
    daughter_ids_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterID()", "", EventMustBeAborted, msg);
  }
}
inline G4int HodoscopeHit::GetDaughterID(const G4int i=0) const{
  if(i<daughter_ids_.size()){
    return daughter_ids_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterID(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_names ---------------------------------------------------
inline void HodoscopeHit::SetDaughterName(const G4int i, const G4String input){
  if(i<daughter_names_.size()){
    daughter_names_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterName()", "", EventMustBeAborted, msg);
  }
}
inline G4String HodoscopeHit::GetDaughterName(const G4int i=0) const{
  if(i<daughter_names_.size()){
    return daughter_names_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterName(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_momenta ---------------------------------------------------
inline void HodoscopeHit::SetDaughterMomentum(const G4int i, const G4ThreeVector input){
  if(i<daughter_momenta_.size()){
    daughter_momenta_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterMomentum()", "", EventMustBeAborted, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetDaughterMomentum(const G4int i=0) const{
  if(i<daughter_momenta_.size()){
    return daughter_momenta_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterMomentum(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_initial_momenta ---------------------------------------------------
inline void HodoscopeHit::SetDaughterInitialMomentum(const G4int i, const G4ThreeVector input){
  if(i<daughter_initial_momenta_.size()){
    daughter_initial_momenta_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterInitialMomentum()", "", EventMustBeAborted, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetDaughterInitialMomentum(const G4int i=0) const{
  if(i<daughter_initial_momenta_.size()){
    return daughter_initial_momenta_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterInitialMomentum(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_hit_time ---------------------------------------------------
inline void HodoscopeHit::SetDaughterHitTime(const G4int i, const G4double input){
  if(i<daughter_hit_time_.size()){
    daughter_hit_time_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterHitTime()", "", EventMustBeAborted, msg);
  }
}
inline G4double HodoscopeHit::GetDaughterHitTime(const G4int i=0) const{
  if(i<daughter_hit_time_.size()){
    return daughter_hit_time_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterHitTime(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_energy_deposit ---------------------------------------------------
inline void HodoscopeHit::SetDaughterEnergyDeposit(const G4int i, const G4double input){
  if(i<daughter_energy_deposit_.size()){
    daughter_energy_deposit_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterEnergyDeposit()", "", EventMustBeAborted, msg);
  }
}
inline void HodoscopeHit::AddDaughterEnergyDeposit(const G4int i, const G4double input){
  if(i<daughter_energy_deposit_.size()){
    daughter_energy_deposit_[i_hit] += input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterEnergyDeposit()", "", EventMustBeAborted, msg);
  }
}
inline G4double HodoscopeHit::GetDaughterEnergyDeposit(const G4int i=0) const{
  if(i<daughter_energy_deposit_.size()){
    return daughter_energy_deposit_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterEnergyDeposit(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

// daughter_global_positions ---------------------------------------------------
inline void HodoscopeHit::SetDaughterGlobalPosition(const G4int i, const G4ThreeVector input){
  if(i<daughter_global_positions_.size()){
    daughter_global_positions_[i_hit] = input;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterGlobalPosition()", "", EventMustBeAborted, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetDaughterGlobalPosition(const G4int i=0) const{
  if(i<daughter_global_positions_.size()){
    return daughter_global_positions_[i];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterGlobalPosition(G4int)", "", EventMustBeAborted, msg);
    return -1;
  }
}
// ------------------------------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
