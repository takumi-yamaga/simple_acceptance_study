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
    // segment_id -------------------------------------------------------------
    inline void SetSegmentID(const G4int input) { segment_id_ = input; }
    inline G4int GetSegmentID() const { return segment_id_; }
    // segment_logical --------------------------------------------------------
    inline void SetLogicalVolume(G4LogicalVolume* input) { segment_logical_ = input; }
    inline G4LogicalVolume* GetLogicalVolume() const { return segment_logical_; }
    // segment_position -------------------------------------------------------
    inline void SetPosition(const G4ThreeVector input) { segment_position_ = input; }
    inline G4ThreeVector GetPosition() const { return segment_position_; }
    // segment_rotation -------------------------------------------------------
    inline void SetRotation(const G4RotationMatrix input) { segment_rotation_ = input; }
    inline G4RotationMatrix GetRotation() const { return segment_rotation_; }
    // ------------------------------------------------------------------------

    // hit particle informations ----------------------------------------------
    // track_id ---------------------------------------------------------------
    inline void SetTrackID(const G4int input) { track_id_ = input; }
    inline G4int GetTrackID() const { return track_id_; }
    // particle_name ----------------------------------------------------------
    inline void SetParticleName(const G4String input) { particle_name_ = input; }
    inline G4String GetParticleName() const { return particle_name_; }
    // momentum ---------------------------------------------------------------
    inline void SetMomentum(const G4ThreeVector input) { momentum_ = input; }
    inline G4ThreeVector GetMomentum() const { return momentum_; }
    // initial_momentum -------------------------------------------------------
    inline void SetInitialMomentumg(const G4ThreeVector input) { initial_momentum_ = input; }
    inline G4ThreeVector GetInitialMomentumg() const { return initial_momentum_; }
    // hit_time ---------------------------------------------------------------
    inline void SetHitTime(const G4double input) { hit_time_ = input; }
    inline G4double GetHitTime() const { return hit_time_; }
    // energy_deposit ---------------------------------------------------------
    inline void SetEnergyDeposit(const G4double input) { energy_deposit_ = input; }
    inline void AddEnergyDeposit(const G4double input) { energy_deposit_ += input; }
    inline G4double GetEnergyDeposit() const { return energy_deposit_; }
    // global_position --------------------------------------------------------
    inline void SetGlobalPosition(const G4ThreeVector input) { global_position_ = input; }
    inline G4ThreeVector GetGlobalPosition() const { return global_position_; }
    // ------------------------------------------------------------------------
    
    // parent particle informations -------------------------------------------
    // parent_id --------------------------------------------------------------
    inline void SetParentID(const G4int input) { parent_id_ = input; }
    inline G4int GetParentID() const { return parent_id_; }
    // parent_name ------------------------------------------------------------
    inline void SetParentName(const G4String input) { parent_name_ = input; }
    inline G4String GetParentName() const { return parent_name_; }
    // parent_initial_momentum ------------------------------------------------
    inline void SetParentInitial(const G4ThreeVector input) { parent_initial_momentum_ = input; }
    inline G4ThreeVector GetParentInitial() const { return parent_initial_momentum_; }
    // ------------------------------------------------------------------------

    // daughter particles infromations ----------------------------------------
    // daughter_id ------------------------------------------------------------
    inline void PushDaughterID(const G4int input) { daughter_ids_.push_back(input); }
    inline std::vector <G4int> GetDaughterIDs() const { return daughter_ids_; }
    inline G4int SetDaughterID(const G4int i, const G4int input) const;
    inline G4int GetDaughterID(const G4int i=0) const;
    // daughter_name ----------------------------------------------------------
    inline void PushDaughterName(const G4String input) { daughter_names_.push_back(input); }
    inline std::vector <G4String> GetDaughterNames() const { return daughter_names_; }
    inline G4int SetDaughterName(const G4int i, const G4String input) const;
    inline G4String GetDaughterName(const G4int i=0) const;
    // daughter_momentum ------------------------------------------------------
    inline void PushDaughterMomentum(const G4double input) { daughter_momenta_.push_back(input); }
    inline std::vector <G4double> GetDaughterMomenta() const { return daughter_momenta_; }
    inline G4int SetDaughterMomentum(const G4int i, const G4double input) const;
    inline G4int GetDaughterMomentum(const G4int i=0) const;
    // daughter_initial_momentum ----------------------------------------------
    inline void PushDaughterInitialMomentum(const G4double input) { daughter_initial_momenta_.push_back(input); }
    inline std::vector <G4double> GetDaughterInitialMomenta() const { return daughter_initial_momenta_; }
    inline G4int SetDaughterInitialMomentum(const G4int i, const G4double input) const;
    inline G4int GetDaughterInitialMomentum(const G4int i=0) const;
    // daughter_hit_time ------------------------------------------------------
    inline void PushDaughterHitTime(const G4double input) { daughter_hit_times_.push_back(input); }
    inline std::vector <G4double> GetDaughterHitTimes() const { return daughter_hit_times_; }
    inline G4int SetDaughterHitTime(const G4int i, const G4double input) const;
    inline G4int GetDaughterHitTime(const G4int i=0) const;
    // daughter_energy_deposit ------------------------------------------------
    inline void PushDaughterEnergyDeposit(const G4double input) { daughter_energy_deposits_.push_back(input); }
    inline std::vector <G4double> GetDaughterEnergyDeposits() const { return daughter_energy_deposits_; }
    inline G4int SetDaughterEnergyDeposit(const G4int i, const G4double input) const;
    inline G4int AddDaughterEnergyDeposit(const G4int i, const G4double input) const;
    inline G4int GetDaughterEnergyDeposit(const G4int i=0) const;
    // daughter_global_position -----------------------------------------------
    inline void PushDaughterGlobalPosition(const G4ThreeVector input) { daughter_global_positions_.push_back(input); }
    inline std::vector <G4ThreeVector> GetDaughterGlobalPositions() const { return daughter_global_positions_; }
    inline G4int SetDaughterGlobalPosition(const G4int i, const G4ThreeVector input) const;
    inline G4int GetDaughterGlobalPosition(const G4int i=0) const;
    // ------------------------------------------------------------------------

  private:
    // hit segment
    G4int segment_id_;
    G4LogicalVolume* segment_logical_;
    G4ThreeVector segment_position_;
    G4RotationMatrix segment_rotation_;
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
