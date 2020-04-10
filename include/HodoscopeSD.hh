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
/// \copied from B5HodoscopeSD.hh
/// \brief Definition of the HodoscopeSD class

#ifndef HodoscopeSD_h
#define HodoscopeSD_h 1

#include "G4VSensitiveDetector.hh"

#include "DetectorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Drift chamber sensitive detector

class HodoscopeSD : public G4VSensitiveDetector
{
  public:
    HodoscopeSD(G4String name);
    virtual ~HodoscopeSD();
    
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
    
  private:
    DetectorHitsCollection* hits_collection_;
    G4int hits_collection_id_;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
