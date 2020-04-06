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
/// \file of SegmentHit.cc
/// \brief Implementation of the SegmentHit class

#include "SegmentHit.hh"
#include "Constants.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<SegmentHit>* SegmentHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SegmentHit::SegmentHit()
: G4VHit(), 
  segment_id_(-1), logical_(nullptr), position_(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SegmentHit::~SegmentHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SegmentHit::SegmentHit(const SegmentHit &right)
  : G4VHit(),
  // hit segment
  segment_id_(right.segment_id_),
  segment_logical_(right.segment_logical_),
  segment_position_(right.segment_position_),
  segment_rotation_(right.segment_rotation_),
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SegmentHit& SegmentHit::operator=(const SegmentHit &right)
{
  segment_id_ = right.segment_id_;
  logical_ = right.logical_;
  position_ = right.position_;
  rotation_ = right.rotation_;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SegmentHit::operator==(const SegmentHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SegmentHit::Draw()
{
  auto vis_manager = G4VVisManager::GetConcreteInstance();
  if (! vis_manager) return;
  G4VisAttributes attributes;

  // hit segment
  auto logical_attributes = logical_->GetVisAttributes();
  if (logical_attributes){
    attributes = *logical_attributes;
    attributes.SetColour(MyColour::ScintillatorHasHit());
    attributes.SetForceSolid(true); // drawing solid shape
    G4Transform3D transform(rotation_.inverse(),position_);
    vis_manager->Draw(*logical_,attributes,transform);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SegmentHit::Print()
{
  G4cout << "-------------------------------------" << G4endl;
  G4cout << "SegmentHit --------------------------" << G4endl;
  G4cout << " segment    : " << segment_id_ << G4endl;
  G4cout << " position-x : " << position_ .x()<< G4endl;
  G4cout << " position-y : " << position_ .y()<< G4endl;
  G4cout << " position-z : " << position_ .z()<< G4endl;
  G4cout << "-------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
