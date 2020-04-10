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
/// \implement of SegmentHit.cc
/// \brief Definition of the SegmentHit class

#ifndef SegmentHit_h
#define SegmentHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class SegmentHit : public G4VHit
{
  public:
    SegmentHit();
    SegmentHit(const SegmentHit &right);
    virtual ~SegmentHit();

    const SegmentHit& operator=(const SegmentHit &right);
    G4bool operator==(const SegmentHit &right) const;

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
    inline void SetTranslation(const G4ThreeVector input) { segment_translation_ = input; }
    inline G4ThreeVector GetTranslation() const { return segment_translation_; }
    // segment_rotation -------------------------------------------------------
    inline void SetRotation(const G4RotationMatrix input) { segment_rotation_ = input; }
    inline G4RotationMatrix GetRotation() const { return segment_rotation_; }
    // ------------------------------------------------------------------------

  private:
    // hit segment
    G4int segment_id_;
    G4LogicalVolume* segment_logical_;
    G4ThreeVector segment_translation_;
    G4RotationMatrix segment_rotation_;
};

using SegmentHitsCollection = G4THitsCollection<SegmentHit>;

extern G4ThreadLocal G4Allocator<SegmentHit>* SegmentHitAllocator;

inline void* SegmentHit::operator new(size_t)
{
  if (!SegmentHitAllocator) {
    SegmentHitAllocator = new G4Allocator<SegmentHit>;
  }
  return (void*)SegmentHitAllocator->MallocSingle();
}

inline void SegmentHit::operator delete(void* aHit)
{
  SegmentHitAllocator->FreeSingle((SegmentHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
