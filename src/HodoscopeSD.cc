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
/// \file of HodoscopeSD.cc
/// \brief Implementation of the HodoscopeSD class

#include "HodoscopeSD.hh"
#include "SegmentHit.hh"
#include "ParticleHit.hh"

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
  collectionName.insert("hodoscope_hits_collection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HodoscopeSD::~HodoscopeSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HodoscopeSD::Initialize(G4HCofThisEvent* collection)
{
  hits_collection_
    = new DetectorHitsCollection(SensitiveDetectorName,collectionName[0]);

  if (hits_collection_id_ == -1) { 
    hits_collection_id_ = G4SDManager::GetSDMpointer()->GetCollectionID(hits_collection_); 
  }
  collection->AddHitsCollection(hits_collection_id_,hits_collection_);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HodoscopeSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // for segment hit informations -----------------------------------
  auto touchable = step->GetPreStepPoint()->GetTouchable();
  // ----------------------------------------------------------------

  // for particle hit informations ----------------------------------
  auto track = step->GetTrack();
  auto track_id = track->GetTrackID();
  auto parent_id = track->GetParentID();
  auto initial_logical_volume = track->GetLogicalVolumeAtVertex();
  // ----------------------------------------------------------------

  // for data informations ------------------------------------------
  auto hit_time = step->GetPreStepPoint()->GetGlobalTime();
  auto hit_position = step->GetPreStepPoint()->GetPosition();
  auto energy_deposit = step->GetTotalEnergyDeposit();
  // ----------------------------------------------------------------

  G4bool track_already_hit = false;
  G4bool daughter_already_hit = false;
  G4bool daughter_new_hit = false;

  // check existing hits --------------------------------------------
  for(auto i_hit=0; i_hit<hits_collection_->entries(); ++i_hit){
    auto hit = (*hits_collection_)[i_hit];

    // check if this track has hit as present particle --------------
    if(hit->GetPresentParticle()->GetTrackID() == track_id){
      track_already_hit = true;
      hit->PushHitTime(hit_time);
      hit->PushEnergyDeposit(energy_deposit);
      hit->PushHitPosition(hit_position);
      break;
    }
    // --------------------------------------------------------------

    // check if this track has hit as daughter particle -------------
    auto daughter_particles = hit->GetDaughterParticles();
    for(auto daughter_particle: daughter_particles){
      if(daughter_particle->GetTrackID() == track_id){
        daughter_already_hit = true;
        hit->PushHitTime(hit_time);
        hit->PushEnergyDeposit(energy_deposit);
        hit->PushHitPosition(hit_position);
        break;
      }
    } 
    if(daughter_already_hit) break;
    // --------------------------------------------------------------

    // check if this track is daughter of present or dauther particle
    if(hit->GetSegmentHit()->GetLogicalVolume() == initial_logical_volume){ 
      if(hit->GetPresentParticle()->GetTrackID() == parent_id){
        daughter_new_hit = true;
      }
      else{
        for(auto* daughter_particle: daughter_particles){
          if(daughter_particle->GetTrackID() == parent_id){
            daughter_new_hit = true;
            break;
          }
        } 
      }
      if(daughter_new_hit){
        hit->PushDaughterParticle(track);
        hit->PushHitTime(hit_time);
        hit->PushEnergyDeposit(energy_deposit);
        hit->PushHitPosition(hit_position);
        break;
      }
    } 
    // ----------------------------------------------------------------
  }

  // if there is no hit of this track, create new hit ---------------
  if(!track_already_hit&&!daughter_already_hit&&!daughter_new_hit){
    auto hit = new DetectorHit();

    hit->SetSegmentHit(touchable);
    hit->SetPresentParticle(track);
    hit->SetIncidentMomentum(track->GetMomentum());
    hit->PushHitTime(hit_time);
    hit->PushEnergyDeposit(energy_deposit);
    hit->PushHitPosition(hit_position);

    hits_collection_->insert(hit);
  }
  // ----------------------------------------------------------------

  return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
