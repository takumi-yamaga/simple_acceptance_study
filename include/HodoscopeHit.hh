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
/// \copied from B5HodoscopeHit.hh
/// \brief Definition of the HodoscopeHit class

#ifndef HodoscopeHit_h
#define HodoscopeHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include <vector>

class G4AttDef;
class G4AttValue;

using std::vector;

/// Drift chamber hit
///
/// It records:
/// - the layer ID
/// - the particle time
/// - the particle local and global positions

class HodoscopeHit : public G4VHit
{
  public:
    HodoscopeHit();
    HodoscopeHit(const HodoscopeHit &right);
    virtual ~HodoscopeHit();

    const HodoscopeHit& operator=(const HodoscopeHit &right);
    G4bool operator==(const HodoscopeHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);

    virtual void Draw();
    virtual void Print();

    // segment informations ---------------------------------------------------
    inline void SetSegmentID(const G4int id) { segment_id_ = id; }
    inline G4int GetSegmentID() const { return segment_id_; }

    inline void SetLogicalVolume(G4LogicalVolume* logical) { logical_ = logical; }
    inline G4LogicalVolume* GetLogicalVolume() const { return logical_; }

    inline void SetPosition(const G4ThreeVector position) { position_ = position; }
    inline G4ThreeVector GetPosition() const { return position_; }

    inline void SetRotation(const G4RotationMatrix rotation) { rotation_ = rotation; }
    inline G4RotationMatrix GetRotation() const { return rotation_; }
    // ------------------------------------------------------------------------

    // hit particle informations ----------------------------------------------
    inline void SetTrackID(const G4int input) { track_id_ = input; }
    inline G4int GetTrackID() const { return track_id_; }

    inline void SetParticleName(const G4String input) { particle_name_ = input; }
    inline G4String GetParticleName() const { return particle_name_; }

    inline void SetMomentum(const G4ThreeVector input) { momentum_ = input; }
    inline G4ThreeVector GetMomentum() const { return momentum_; }

    inline void SetInitialMomentumg(const G4ThreeVector input) { initial_momentum_ = input; }
    inline G4ThreeVector GetInitialMomentumg() const { return initial_momentum_; }

    inline void SetHitTime(const G4double input) { hit_time_ = input; }
    inline G4double GetHitTime() const { return hit_time_; }

    inline void SetEnergyDeposit(const G4double input) { energy_deposit_ = input; }
    inline void AddEnergyDeposit(const G4double input) { energy_deposit_ += input; }
    inline G4double GetEnergyDeposit() const { return energy_deposit_; }

    inline void SetGlobalPositiong(const G4ThreeVector input) { global_position_ = input; }
    inline G4ThreeVector GetGlobalPositiong() const { return global_position_; }
    // ------------------------------------------------------------------------
    
    // parent particle informations -------------------------------------------
    inline void SetParentID(const G4int input) { parent_id_ = input; }
    inline G4int GetParentID() const { return parent_id_; }

    inline void SetParentName(const G4String input) { parent_name_ = input; }
    inline G4String GetParentName() const { return parent_name_; }

    inline void SetParentInitial(const G4ThreeVector input) { parent_initial_momentum_ = input; }
    inline G4ThreeVector GetParentInitial() const { return parent_initial_momentum_; }
    // ------------------------------------------------------------------------

    // daughter particles infromations ----------------------------------------
    inline void PushDaughterID(const G4int input) { daughter_ids_.push_back(input); }
    inline std::vector <G4int> GetDaughterIDs() const { return daughter_ids_; }
    inline G4int SetDaughterID(const G4int i, const G4int input) const;
    inline G4int GetDaughterID(const G4int i=0) const;

    inline void PushDaughterName(const G4int input) { daughter_names_.push_back(input); }
    inline std::vector <G4int> GetDaughterNames() const { return daughter_names_; }
    inline G4int SetDaughterName(const G4int i, const G4int input) const;
    inline G4int GetDaughterName(const G4int i=0) const;

    inline void PushDaughterMomentum(const G4double input) { daughter_momenta_.push_back(input); }
    inline std::vector <G4double> GetDaughterMomenta() const { return daughter_momenta_; }
    inline G4int SetDaughterMomentum(const G4int i, const G4double input) const;
    inline G4int GetDaughterMomentum(const G4int i=0) const;

    inline void PushDaughterInitialMomentum(const G4double input) { daughter_initial_momenta_.push_back(input); }
    inline std::vector <G4double> GetDaughterInitialMomenta() const { return daughter_initial_momenta_; }
    inline G4int SetDaughterInitialMomentum(const G4int i, const G4double input) const;
    inline G4int GetDaughterInitialMomentum(const G4int i=0) const;

    inline void PushDaughterHitTime(const G4double input) { daughter_hit_times_.push_back(input); }
    inline std::vector <G4double> GetDaughterHitTimes() const { return daughter_hit_times_; }
    inline G4int SetDaughterHitTime(const G4int i, const G4double input) const;
    inline G4int GetDaughterHitTime(const G4int i=0) const;

    inline void PushDaughterEnergyDeposit(const G4double input) { daughter_energy_deposits_.push_back(input); }
    inline std::vector <G4double> GetDaughterEnergyDeposits() const { return daughter_energy_deposits_; }
    inline G4int SetDaughterEnergyDeposit(const G4int i, const G4double input) const;
    inline G4int AddDaughterEnergyDeposit(const G4int i, const G4double input) const;
    inline G4int GetDaughterEnergyDeposit(const G4int i=0) const;

    inline void PushDaughterGlobalPosition(const G4ThreeVector input) { daughter_global_positions_.push_back(input); }
    inline std::vector <G4ThreeVector> GetDaughterGlobalPositions() const { return daughter_global_positions_; }
    inline G4int SetDaughterGlobalPosition(const G4int i, const G4ThreeVector input) const;
    inline G4int GetDaughterGlobalPosition(const G4int i=0) const;
    // ------------------------------------------------------------------------

  private:
    // hit segment
    G4int segment_id_;
    G4LogicalVolume* logical_;
    G4ThreeVector position_;
    G4RotationMatrix rotation_;
    // hit particle
    G4int track_id_;
    G4String particle_name_;
    G4ThreeVector momentum_;
    G4ThreeVector initial_momentum_;
    G4double hit_time_;
    G4double energy_deposit_;
    G4ThreeVector global_position_;
    // parent particle
    G4int parent_id_;
    G4String parent_name_;
    G4ThreeVector parent_initial_momentum_;
    // daughter particle
    std::vector <G4int> daughter_ids_;
    std::vector <G4String> daughter_names_;
    std::vector <G4ThreeVector> daughter_momenta_;
    std::vector <G4ThreeVector> daughter_initial_momenta_;
    std::vector <G4double> daughter_hit_times_;
    std::vector <G4double> daughter_energy_deposits_;
    std::vector <G4ThreeVector> daugher_global_positions_;
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

inline void HodoscopeHit::SetDaughterID(const G4int i, const G4int input){
  if(i<daughter_ids_.size()){
    daughter_id_[i_hit] = id;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetDaughterID()", "Code002", JustWarning, msg);
  }
}
inline G4int HodoscopeHit::GetDaughterID(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return track_id_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetDaughterID(G4int)",
        "Code002", JustWarning, msg);
    return -1;
  }
}

inline void HodoscopeHit::SetParentID(const G4int i_hit, const G4int id){
  if(i_hit<total_hits_){
    parent_id_[i_hit] = id;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetParentID()",
        "Code002", JustWarning, msg);
  }
}
inline G4int HodoscopeHit::GetParentID(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return parent_id_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetParentID(G4int)",
        "Code002", JustWarning, msg);
    return -1;
  }
}

inline void HodoscopeHit::SetParticleID(const G4int i_hit, const G4int id){
  if(i_hit<total_hits_){
    parent_id_[i_hit] = id;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetParticleID()",
        "Code002", JustWarning, msg);
  }
}
inline G4int HodoscopeHit::GetParticleID(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return particle_id_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetParticleID(G4int)",
        "Code002", JustWarning, msg);
    return -1;
  }
}

inline void HodoscopeHit::SetHitTime(const G4int i_hit, const G4double hit_time){
  if(i_hit<total_hits_){
    hit_time_[i_hit] = hit_time;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetHitTime()",
        "Code002", JustWarning, msg);
  }
}
inline G4double HodoscopeHit::GetHitTime(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return hit_time_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetHitTime(G4int)",
        "Code002", JustWarning, msg);
    return -1;
  }
}

inline void HodoscopeHit::SetEnergyDeposit(const G4int i_hit, const G4double energy_deposit){
  if(i_hit<total_hits_){
    energy_deposit_[i_hit] = energy_deposit;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetEnergyDeposit()",
        "Code002", JustWarning, msg);
  }
}
inline G4double HodoscopeHit::GetEnergyDeposit(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return energy_deposit_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetEnergyDeposit(G4int)",
        "Code002", JustWarning, msg);
    return -1;
  }
}

inline void HodoscopeHit::SetLocalPosition(const G4int i_hit, const G4ThreeVector local_position){
  if(i_hit<total_hits_){
    local_position_[i_hit] = local_position;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetLocalPosition()",
        "Code002", JustWarning, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetLocalPosition(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return local_position_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetLocalPosition(G4int)",
        "Code002", JustWarning, msg);
    return G4ThreeVector(0);
  }
}

inline void HodoscopeHit::SetGlobalPosition(const G4int i_hit, const G4ThreeVector global_position){
  if(i_hit<total_hits_){
    global_position_[i_hit] = global_position;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetGlobalPosition()",
        "Code002", JustWarning, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetGlobalPosition(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return global_position_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetGlobalPosition(G4int)",
        "Code002", JustWarning, msg);
    return G4ThreeVector(0);
  }
}

inline void HodoscopeHit::SetMomentum(const G4int i_hit, const G4ThreeVector momentum){
  if(i_hit<total_hits_){
    momentum_[i_hit] = momentum;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetMomentum()",
        "Code002", JustWarning, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetMomentum(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return momentum_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetMomentum(G4int)",
        "Code002", JustWarning, msg);
    return G4ThreeVector(0);
  }
}

inline void HodoscopeHit::SetPolarization(const G4int i_hit, const G4ThreeVector polarization){
  if(i_hit<total_hits_){
    polarization_[i_hit] = polarization;
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::SetPolarization()",
        "Code002", JustWarning, msg);
  }
}
inline G4ThreeVector HodoscopeHit::GetPolarization(const G4int i_hit) const{
  if(i_hit<total_hits_){
    return polarization_[i_hit];
  }
  else{
    G4ExceptionDescription msg;
    msg << "No hits found." << G4endl; 
    G4Exception("HodoscopeHit::GetPolarization(G4int)",
        "Code002", JustWarning, msg);
    return G4ThreeVector(0);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
