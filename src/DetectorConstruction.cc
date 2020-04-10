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
#include "SolenoidMagneticField.hh"
#include "HodoscopeSD.hh"

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

G4ThreadLocal SolenoidMagneticField* DetectorConstruction::magnetic_field_ = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::field_manager_ = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(), 
  magnetic_logical_(nullptr), cdh_logical_(nullptr)
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
  auto liquid_He3 = G4Material::GetMaterial("liquid_He3");
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

  // magnetic field
  //auto magnetic_radius = 1822.*mm/2.;
  //auto magnetic_length = 3320.*mm;
  auto magnetic_radius = 830.*mm + kSpace;
  auto magnetic_length = 2870.*mm + kSpace;
  auto magnetic_solid
      = new G4Tubs("magnetic_solid",0.*mm,magnetic_radius,magnetic_length/2.,0.*deg,360.*deg);
  magnetic_logical_ = new G4LogicalVolume(magnetic_solid,air,"magnetic_logical");
  new G4PVPlacement(0,G4ThreeVector(),magnetic_logical_,"magnetic_physical",
      world_logical,false,0,kCheckOverlaps);
  // set step limit in magnetic field  
  G4UserLimits* magnetic_userlimits = new G4UserLimits(magnetic_radius);
  magnetic_logical_->SetUserLimits(magnetic_userlimits);

  // target
  auto target_radius = 30.*mm;
  auto target_length = 150.*mm;
  auto target_solid 
    = new G4Tubs("target_solid",0.*mm,target_radius,target_length/2.,0.*deg,360.*deg);
  auto target_logical = new G4LogicalVolume(target_solid,liquid_He3,"target_logical");
  new G4PVPlacement(0,G4ThreeVector(),target_logical,"target_physical",
      magnetic_logical_,false,0,kCheckOverlaps);

  // cdh
  auto cdh_radius = 530.*mm;
  auto cdh_length = 2570.*mm;
  auto cdh_thickness = 30.*mm;
  auto cdh_solid 
    = new G4Tubs("cdh_solid",cdh_radius-cdh_thickness/2.,cdh_radius+cdh_thickness/2.,cdh_length/2.,0.*deg,360.*deg);
  cdh_logical_ = new G4LogicalVolume(cdh_solid,scintillator,"cdh_logical");
  new G4PVPlacement(0,G4ThreeVector(),cdh_logical_,"cdh_physical",
      magnetic_logical_,false,0,kCheckOverlaps);

  // disc
  auto disc_inner_radius = 15.*cm;
  auto disc_outer_radius = cdh_radius-cdh_thickness/2.;
  auto disc_thickness = 30.*mm;
  auto disc_solid 
    = new G4Tubs("disc_solid",disc_inner_radius,disc_outer_radius,disc_thickness/2.,0.*deg,360.*deg);
  disc_logical_ = new G4LogicalVolume(disc_solid,scintillator,"disc_logical");
  auto disc_segment1_transform = G4ThreeVector(0.*mm,0.*mm,-cdh_length/2.-disc_thickness/2.-kSpace);
  new G4PVPlacement(0,disc_segment1_transform,disc_logical_,"disc_segment1_physical",
      magnetic_logical_,false,0,kCheckOverlaps);
  auto disc_segment2_transform = G4ThreeVector(0.*mm,0.*mm,+cdh_length/2.+disc_thickness/2.+kSpace);
  new G4PVPlacement(0,disc_segment2_transform,disc_logical_,"disc_segment2_physical",
      magnetic_logical_,false,1,kCheckOverlaps);

  // visualization attributes ------------------------------------------------

  auto visAttributes = new G4VisAttributes(MyColour::Transparent());
  visAttributes->SetVisibility(false);
  world_logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  // magnetic
  visAttributes = new G4VisAttributes(MyColour::Magnetic());
  magnetic_logical_->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  // target
  visAttributes = new G4VisAttributes(MyColour::Target());
  target_logical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  // cdh
  visAttributes = new G4VisAttributes(MyColour::Scintillator());
  cdh_logical_->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  // disc
  visAttributes = new G4VisAttributes(MyColour::Scintillator());
  disc_logical_->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------

  return world_physical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{

  // sensitive detectors -----------------------------------------------------
  auto sdManager = G4SDManager::GetSDMpointer();
  G4String sensitive_detector_name;
  auto cdh = new HodoscopeSD(sensitive_detector_name="cdh");
  sdManager->AddNewDetector(cdh);
  cdh_logical_->SetSensitiveDetector(cdh);
  auto disc = new HodoscopeSD(sensitive_detector_name="disc");
  sdManager->AddNewDetector(disc);
  disc_logical_->SetSensitiveDetector(disc);
  // -------------------------------------------------------------------------

  // magnetic field ----------------------------------------------------------
  magnetic_field_ = new SolenoidMagneticField();
  field_manager_ = new G4FieldManager();
  field_manager_->SetDetectorField(magnetic_field_);
  field_manager_->CreateChordFinder(magnetic_field_);
  G4bool force_to_all_daughters = true; // if true, all daughters have the same field
  magnetic_logical_->SetFieldManager(field_manager_, force_to_all_daughters);
  // -------------------------------------------------------------------------
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

  // liquid-He3
  new G4Material("liquid_He3",2.,3.016029*g/mole,81.2*mg/cm3,kStateLiquid);

  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
