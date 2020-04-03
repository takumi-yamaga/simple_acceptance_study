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
/// \copied from B5HodoscopeSD.cc
/// \brief Implementation of the Hodoscope class

#include "HodoscopeSD.hh"
#include "HodoscopeHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeSD::HodoscopeSD(G4String name)
: G4VSensitiveDetector(name), 
  hits_collection_(nullptr), hits_collection_id_(-1)
{
  collectionName.insert("hodoscope_hitscollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeSD::~HodoscopeSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeSD::Initialize(G4HCofThisEvent* collection)
{
  hits_collection_
    = new HodoscopeHitsCollection(SensitiveDetectorName,collectionName[0]);

  if (hits_collection_id_<0) { 
    hits_collection_id_ = G4SDManager::GetSDMpointer()->GetCollectionID(hits_collection_); 
  }
  collection->AddHitsCollection(hits_collection_id_,hits_collection_);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HodoscopeSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  auto track = step->GetTrack();
  auto track_id = track->GetTrackID();
  auto particle = track->GetParticleDefinition();
  auto particle_name = particle->GetParticleName();
  auto parent_id = track->GetParentID();

  // only proton, neutron, and pi- are detected.
  //if((particle_name!="proton")&&(particle_name!="neutron")&&(particle_name!="pi-")) return true;

  G4bool track_already_hit = false;
  G4bool daughter_already_hit = false;
  for(auto i_hit=0; i_hit<hits_collection_->entries(); i_hit++){
    auto hit = (*hits_collection_)[i_hit];

    // if the track has already created hit,
    // energy-deposit is added to exsisting hit.
    if(hit->GetTrackID()==track_id){
      track_already_hit = true;
      hit->AddEnergyDeposit(energy_deposit);
    }

  }
  // if there is no hit by the track, create new hit.
  if(!track_already_hit){
    auto hit = new HodoscopeHit();
    hit->SetTrackID(track_id);
    hit->SetParentID(parent_id);
    hit->SetHitTime(hit_time);
    hit->SetEnergyDeposit(energy_deposit);
    hit->SetGlobalPosition(global_position);
    hit->SetMomentum(momentum);
    hit->SetInitialMomentum(initial_momentum);
    hits_collection_->insert(hit);
  }

  return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
