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
/// \copied from B5HodoscopeHit.cc
/// \brief Implementation of the HodoscopeHit class

#include "HodoscopeHit.hh"
#include "Constants.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<HodoscopeHit>* HodoscopeHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeHit::HodoscopeHit()
: G4VHit(), 
  segment_id_(-1), logical_(nullptr), position_(0), total_hits_(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeHit::~HodoscopeHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeHit::HodoscopeHit(const HodoscopeHit &right)
: G4VHit(),
  segment_id_(right.segment_id_),
  logical_(right.logical_),
  position_(right.position_),
  rotation_(right.rotation_),
  total_hits_(right.total_hits_),

  track_id_(right.track_id_),
  parent_id_(right.parent_id_),
  particle_id_(right.particle_id_),
  hit_time_(right.hit_time_),
  energy_deposit_(right.energy_deposit_),
  local_position_(right.local_position_),
  global_position_(right.global_position_),
  momentum_(right.momentum_),
  polarization_(right.polarization_)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HodoscopeHit& HodoscopeHit::operator=(const HodoscopeHit &right)
{
  segment_id_ = right.segment_id_;
  logical_ = right.logical_;
  position_ = right.position_;
  rotation_ = right.rotation_;
  total_hits_ = right.total_hits_;

  track_id_ = right.track_id_;
  parent_id_ = right.parent_id_;
  particle_id_ = right.particle_id_;
  hit_time_ = right.hit_time_;
  energy_deposit_ = right.energy_deposit_;
  local_position_ = right.local_position_;
  global_position_ = right.global_position_;
  momentum_ = right.momentum_;
  polarization_ = right.polarization_;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HodoscopeHit::operator==(const HodoscopeHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeHit::Draw()
{
  auto vis_manager = G4VVisManager::GetConcreteInstance();
  if (! vis_manager) return;
  G4VisAttributes attributes;

  // hit point
  for(auto hit_position: global_position_){
    G4Circle circle(hit_position);
    circle.SetScreenSize(10);
    circle.SetFillStyle(G4Circle::filled);
    attributes.SetColour(MyColour::Hit());
    circle.SetVisAttributes(attributes);
    vis_manager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeHit::Print()
{
  G4cout << "-------------------------------------" << G4endl;
  G4cout << " segment    : " << segment_id_ << G4endl;
  G4cout << " total hits : " << total_hits_ << G4endl;
  G4cout << "-------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
