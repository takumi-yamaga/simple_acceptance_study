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
/// \copied from B5DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "HodoscopeSD.hh"
#include "Constants.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(), 
  hodoscope1_logical_(nullptr), hodoscope2_logical_(nullptr)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction()
{
  //delete fMessenger;

  for (auto visAttributes: fVisAttributes) {
    delete visAttributes;
  }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Construct materials
  ConstructMaterials();
  auto air = G4Material::GetMaterial("G4_AIR");
  auto scintillator = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //auto vacuum = G4Material::GetMaterial("G4_Galactic");
  //auto argonGas = G4Material::GetMaterial("G4_Ar");
  //auto csI = G4Material::GetMaterial("G4_CESIUM_IODIDE");
  //auto lead = G4Material::GetMaterial("G4_Pb");
  //auto carbon = G4Material::GetMaterial("G4_C");
  //auto hydrogen = new G4Material("hydrogne", 1., 1.01*g/mole, 1.*g/cm3);

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool kCheckOverlaps = true;

  // geometries --------------------------------------------------------------

  // experimental hall (world volume)
  auto world_solid 
    = new G4Box("world_solid",10.*m,3.*m,10.*m);
  auto world_logical
    = new G4LogicalVolume(world_solid,air,"world_logical");
  auto world_physical
    = new G4PVPlacement(0,G4ThreeVector(),world_logical,"world_physical",0,
        false,0,kCheckOverlaps);

  // hodoscope1
  auto hodoscope1_size_x = 100.*mm;
  auto hodoscope1_size_y = 100.*mm;
  auto hodoscope1_thickness = 50.*mm;

  auto hodoscope1_position = G4ThreeVector(0.*mm,0.*mm,-100.*mm);
  auto hodoscope1_solid 
    = new G4Box("hodoscope1_solid",hodoscope1_size_x/2.,hodoscope1_size_y/2.,hodoscope1_thickness/2.);
  hodoscope1_logical_
    = new G4LogicalVolume(hodoscope1_solid,scintillator,"hodoscope1_logical");
  new G4PVPlacement(0,hodoscope1_position,hodoscope1_logical_,"hodoscope1_physical",
      world_logical,false,0,kCheckOverlaps);

  // hodoscope2
  auto hodoscope2_size_x = 100.*mm;
  auto hodoscope2_size_y = 100.*mm;
  auto hodoscope2_thickness = 50.*mm;

  auto hodoscope2_position = G4ThreeVector(0.*mm,0.*mm,100.*mm);
  auto hodoscope2_solid 
    = new G4Box("hodoscope2_solid",hodoscope2_size_x/2.,hodoscope2_size_y/2.,hodoscope2_thickness/2.);
  hodoscope2_logical_
    = new G4LogicalVolume(hodoscope2_solid,scintillator,"hodoscope2_logical");
  new G4PVPlacement(0,hodoscope2_position,hodoscope2_logical_,"hodoscope2_physical",
      world_logical,false,0,kCheckOverlaps);

  // visualization attributes ------------------------------------------------

  auto visAttributes = new G4VisAttributes(G4Colour::White());
  visAttributes->SetVisibility(false);
  world_logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(MyColour::Scintillator());
  hodoscope1_logical_->SetVisAttributes(visAttributes);
  hodoscope2_logical_->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------

  return world_physical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String sensitive_detector_name;

  // sensitive detectors -----------------------------------------------------
  auto hodoscope1 = new HodoscopeSD(sensitive_detector_name="/hodoscope1");
  sdManager->AddNewDetector(hodoscope1);
  hodoscope1_logical_->SetSensitiveDetector(hodoscope1);

  auto hodoscope2 = new HodoscopeSD(sensitive_detector_name="/hodoscope2");
  sdManager->AddNewDetector(hodoscope2);
  hodoscope2_logical_->SetSensitiveDetector(hodoscope2);

}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
  auto nistManager = G4NistManager::Instance();

  // Air 
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Argon gas
  nistManager->FindOrBuildMaterial("G4_Ar");

  // Scintillator
  // (PolyVinylToluene, C_9H_10)
  nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // CsI
  nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

  // Lead
  nistManager->FindOrBuildMaterial("G4_Pb");

  // Carbon
  nistManager->FindOrBuildMaterial("G4_C");

  // Vacuum "Galactic"
  nistManager->FindOrBuildMaterial("G4_Galactic");


  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
