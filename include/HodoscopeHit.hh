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

class G4AttDef;
class G4AttValue;

using std::vector

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
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();

    inline void SetTrackID(G4int id) { track_id_ = id; }
    inline void AddTrackID(G4int id) { track_id_ = id; }
    inline G4int GetTrackID() const { return track_id_; }

    inline void SetParentID(G4int id) { parent_id_ = id; }
    inline G4int GetParentID() const { return parent_id_; }

    inline void SetParticleID(G4int id) { particle_id_ = id; }
    inline G4int GetParticleID() const { return particle_id_; }

    inline void SetSegmentID(G4int id) { segment_id_ = id; }
    inline G4int GetSegmentID() const { return segment_id_; }

    inline void SetHitTime(G4double time) { hit_time_ = time; }
    inline G4double GetHitTime() const { return hit_time_; }

    inline void SetEnergyDeposit(G4double de) { energy_deposit_ = de; }
    inline G4double GetEnergyDeposit() const { return energy_deposit_; }

    inline void SetLocalPosition(G4ThreeVector position) { local_position_ = position; }
    inline G4ThreeVector GetLocalPosition() const { return local_position_; }

    inline void SetGlobalPosition(G4ThreeVector position) { global_position_ = position; }
    inline G4ThreeVector GetGlobalPosition() const { return global_position_; }
    
    inline void SetMomentum(G4ThreeVector momentum) { momentum_ = momentum; }
    inline G4ThreeVector GetMomentum() const { return momentum_; }
    
    inline void SetPolarization(G4ThreeVector polarization) { polarization_ = polarization; }
    inline G4ThreeVector GetPolarization() const { return polarization_; }
    
    inline void SetPosition(G4ThreeVector position) { position_ = position; }
    inline G4ThreeVector GetPosition() const { return position_; }
    
    inline void SetRotation(G4RotationMatrix rotation) { rotation_ = rotation; }
    inline G4RotationMatrix GetRotation() const { return rotation_; }

    inline void SetLogicalVolume(G4LogicalVolume* logical) { logical_ = logical; }
    inline G4LogicalVolume* GetLogicalVolume() const { return logical_; }
    
  private:
    G4int track_id_;
    G4int parent_id_;
    G4int particle_id_;
    G4int segment_id_;
    G4double hit_time_;
    G4double energy_deposit_;
    G4ThreeVector local_position_;
    G4ThreeVector global_position_;
    G4ThreeVector momentum_;
    G4ThreeVector polarization_;
    G4ThreeVector position_;
    G4RotationMatrix rotation_;
    G4LogicalVolume* logical_;
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
