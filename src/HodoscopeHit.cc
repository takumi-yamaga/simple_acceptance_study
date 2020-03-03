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
  segment_id_(-1), momentum_(0), polarization_(0), position_(0), logical_(nullptr)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeHit::~HodoscopeHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeHit::HodoscopeHit(const HodoscopeHit &right)
: G4VHit(),
  track_id_(right.track_id_),
  parent_id_(right.parent_id_),
  particle_id_(right.particle_id_),
  segment_id_(right.segment_id_),
  hit_time_(right.hit_time_),
  local_position_(right.local_position_),
  global_position_(right.global_position_),
  momentum_(right.momentum_),
  polarization_(right.polarization_),
  position_(right.position_),
  rotation_(right.rotation_),
  logical_(right.logical_)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HodoscopeHit& HodoscopeHit::operator=(const HodoscopeHit &right)
{
  track_id_ = right.track_id_;
  parent_id_ = right.parent_id_;
  particle_id_ = right.particle_id_;
  segment_id_ = right.segment_id_;
  hit_time_ = right.hit_time_;
  local_position_ = right.local_position_;
  global_position_ = right.global_position_;
  momentum_ = right.momentum_;
  polarization_ = right.polarization_;
  position_ = right.position_;
  rotation_ = right.rotation_;
  logical_ = right.logical_;
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
  G4Circle circle(global_position_);
  circle.SetScreenSize(10);
  circle.SetFillStyle(G4Circle::filled);
  attributes.SetColour(MyColour::Hit());
  circle.SetVisAttributes(attributes);
  vis_manager->Draw(circle);

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

const std::map<G4String,G4AttDef>* HodoscopeHit::GetAttDefs() const
{
  G4bool is_new;
  auto store = G4AttDefStore::GetInstance("HodoscopeHit",is_new);

  if (is_new) {
    (*store)["HitType"] 
      = G4AttDef("HitType","Hit Type","Physics","","G4String");

    (*store)["ID"] 
      = G4AttDef("ID","ID","Physics","","G4int");

    (*store)["Time"] 
      = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");

    (*store)["Pos"] 
      = G4AttDef("Pos", "Position", "Physics","G4BestUnit","G4ThreeVector");
  }

  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* HodoscopeHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values
    ->push_back(G4AttValue("HitType","HodoscopeHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(segment_id_),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(hit_time_,"Time"),""));
  values
    ->push_back(G4AttValue("Position",G4BestUnit(global_position_,"Length"),""));

  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeHit::Print()
{
  G4cout << "-------------------------------------" << G4endl;
  G4cout << " segment    : " << segment_id_ << G4endl;
  G4cout << " global pos : " << global_position_.x() << ", " << global_position_.y() 
    << ", " << global_position_.z() << G4endl;
  G4cout << "-------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
