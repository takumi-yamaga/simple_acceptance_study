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
/// \file of DetectorHit.cc
/// \brief Implementation of the DetectorHit class

#include "DetectorHit.hh"
#include "Constants.hh"
#include "SegmentHit.hh"
#include "ParticleHit.hh"

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

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit()
: G4VHit(), 
  segment_hit_(nullptr), present_particle_(nullptr), incident_momentum_(0) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::~DetectorHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorHit::DetectorHit(const DetectorHit &right)
  : G4VHit(),
  segment_hit_(right.segment_hit_),
  present_particle_(right.present_particle_),
  parent_particles_(right.parent_particles_),
  daughter_particles_(right.daughter_particles_),
  incident_momentum_(right.incident_momentum_),
  hit_times_(right.hit_times_),
  energy_deposits_(right.energy_deposits_),
  hit_positions_(right.hit_positions_)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const DetectorHit& DetectorHit::operator=(const DetectorHit &right)
{
  segment_hit_ = right.segment_hit_;
  present_particle_ = right.present_particle_;
  parent_particles_ = right.parent_particles_;
  daughter_particles_ = right.daughter_particles_;
  incident_momentum_ = right.incident_momentum_;
  hit_times_ = right.hit_times_;
  energy_deposits_ = right.energy_deposits_;
  hit_positions_ = right.hit_positions_;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DetectorHit::operator==(const DetectorHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Draw()
{
  // drawing segment hit
  //segment_hit_->Draw();

  // hit point (only incident position)
  if(hit_positions_.size()){
    auto vis_manager = G4VVisManager::GetConcreteInstance();
    if (! vis_manager) return;
    G4VisAttributes attributes;
    G4Circle circle(hit_positions_[0]);
    circle.SetScreenSize(10);
    circle.SetFillStyle(G4Circle::filled);
    attributes.SetColour(MyColour::Hit());
    circle.SetVisAttributes(attributes);
    vis_manager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorHit::Print()
{
  G4cout << G4endl;
  G4cout << "-------------------------------------" << G4endl;
  G4cout << "Detector Hit ------------------------" << G4endl;

  G4cout << "segmen_hit_ -------------------------" << G4endl;
  segment_hit_->Print();

  G4cout << "present_particle_ -------------------" << G4endl;
  present_particle_->Print();

  G4cout << "daughter_particle_ ------------------" << G4endl;
  for(auto& daughter_particle: daughter_particles_){
    daughter_particle->Print();
  }

  G4cout << "-------------------------------------" << G4endl;
  G4cout << "-------------------------------------" << G4endl;
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
