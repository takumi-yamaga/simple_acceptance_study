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
/// \copied from B5EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "DetectorHit.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

using std::array;
using std::vector;

namespace {

  G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
    auto hce = event->GetHCofThisEvent();
    if (!hce) {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("EventAction::EndOfEventAction()",
          "Code001", JustWarning, msg);
      return nullptr;
    }

    auto hc = hce->GetHC(collId);
    if (!hc) {
      G4ExceptionDescription msg;
      msg << "Hits collection " << collId << " of this event not found." << G4endl; 
      G4Exception("EventAction::EndOfEventAction()",
          "Code001", JustWarning, msg);
    }
    return hc;  
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
  : G4UserEventAction() 
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  for(auto& id: hodoscope_hits_collection_ids_){
    id = -1;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  // Find hit collections and histogram Ids by names (just once)
  // and save them in the data members of this class

  // hodoscope
  if (hodoscope_hits_collection_ids_[0] == -1) {
    auto sd_manager = G4SDManager::GetSDMpointer();

    for (auto i_hodoscope = 0; i_hodoscope < Hodoscope::kTotalNumber; ++i_hodoscope) {
      auto collection_name = Hodoscope::kDetectorNames[i_hodoscope];
      collection_name.append("/hodoscope_hits_collection");
      hodoscope_hits_collection_ids_[i_hodoscope]
        = sd_manager->GetCollectionID(collection_name);
    }
  }

}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Check primary vertex
  auto total_primaries = event->GetNumberOfPrimaryVertex();
  G4cout << "total_primaries : " << total_primaries << G4endl;

  // ======================================================
  // Hodoscopes ===========================================
  // ======================================================
  for(auto i_hodoscope=0; i_hodoscope<Hodoscope::kTotalNumber; ++i_hodoscope){
    auto hits_collection = GetHC(event,hodoscope_hits_collection_ids_[i_hodoscope]);
    if(hits_collection){
      for(G4int i_hit=0; i_hit<(G4int)hits_collection->GetSize(); ++i_hit){
        auto hit = hits_collection->GetHit(i_hit);
        if(hit){
          hit->Print();
        }
      }
    }
  }
  // ======================================================
  // ======================================================


  // ======================================================
  // Fill Tree ============================================
  // ======================================================
  //if(dcin_has_hit){
  //  analysisManager->FillNtupleIColumn(0,dcin_total_hits);
  //  analysisManager->FillNtupleFColumn(1,(G4float)dcin_position.x());
  //  analysisManager->FillNtupleFColumn(2,(G4float)dcin_position.y());
  //  analysisManager->FillNtupleFColumn(3,(G4float)dcin_position.z());
  //  analysisManager->FillNtupleFColumn(4,(G4float)dcin_momentum.x());
  //  analysisManager->FillNtupleFColumn(5,(G4float)dcin_momentum.y());
  //  analysisManager->FillNtupleFColumn(6,(G4float)dcin_momentum.z());
  //}
  //analysisManager->AddNtupleRow();
  // ======================================================
  // ======================================================


  ////
  //// Print diagnostics
  //// 

  //auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  //if ( printModulo == 0 || event->GetEventID() % printModulo != 0) return;

  // Drift chambers
  //for (G4int i_dc = 0; i_dc < kNumberofDCs; ++i_dc) {
  //  auto hc = GetHC(event, dc_hitcollection_id_[i_dc]);
  //  if ( ! hc ) return;
  //  G4cout << "Drift Chamber " << i_dc + 1 << " has " <<  hc->GetSize()  << " hits." << G4endl;
  //}

  // set printing per each event
  if(event->GetEventID()){
    G4int print_progress = (G4int)log10(event->GetEventID());
    G4RunManager::GetRunManager()->SetPrintProgress(pow(10,print_progress));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
