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
/// \copied from B5PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4GenericMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  messenger_(nullptr), phasespace_generator_(nullptr)
{
  total_primaries_=3;
  particle_names_.push_back("lambda");
  particle_names_.push_back("proton");
  particle_names_.push_back("neutron");

  // construction of particle guns
  ConstructParticleGun();

  // mode select
  // 0 : shot (random momentum, direction)
  // 1 : phase-space
  primary_mode_ = 1;

  // maximum momentum for shot-mode
  shot_maximum_momentum_ = 500.*MeV;

  // Lorentz vectors of beam and target for phase-space-mode
  beam_momentum_ = 1.*GeV;
  beam_particle_name_ = "kaon-";
  target_particle_name_ = "He3";
  SetBeamAndTarget();
  
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete phasespace_generator_;
  for(auto* particlegun : particleguns_){
    delete particlegun;
  }
  delete messenger_;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  switch(primary_mode_){

    case 0: // random-shot
      GenerateShot(event);
      break;

    case 1: // phase-space
      GeneratePhaseSpace(event);
      break;

    default:
      break;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::ConstructParticleGun() 
{
  if(particleguns_.size()){
    for(auto* particlegun : particleguns_){
      delete particlegun;
    }
    particleguns_.clear();
    particleguns_.shrink_to_fit();
  }
  if(particle_masses_.size()){
    particle_masses_.clear();
    particle_masses_.shrink_to_fit();
  }
  // construct particlegun
  auto particle_table = G4ParticleTable::GetParticleTable();
  for(auto particle_name : particle_names_){
    G4ParticleDefinition* particle_definition = particle_table->FindParticle(particle_name);
    auto particle_mass = particle_definition->GetPDGMass();
    auto* particlegun = new G4ParticleGun();
    particlegun->SetParticleDefinition(particle_definition);
    particlegun->SetParticleEnergy(particle_mass);
    particlegun->SetParticleMomentum(G4ThreeVector());
    particlegun->SetParticlePosition(G4ThreeVector());
    particleguns_.push_back(particlegun);
    particle_masses_.push_back(particle_mass);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetBeamAndTarget() 
{
  auto particle_table = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* beam_particle = particle_table->FindParticle(beam_particle_name_);
  auto beam_mass = beam_particle->GetPDGMass();
  auto beam_energy = sqrt(beam_mass*beam_mass+beam_momentum_*beam_momentum_);
  beam_lv_ = TLorentzVector(0.,0.,beam_momentum_,beam_energy);
  G4ParticleDefinition* target_particle = particle_table->FindParticle(target_particle_name_);
  auto target_mass = target_particle->GetPDGMass();
  target_lv_ = TLorentzVector(0.,0.,0.,target_mass);
  initial_lv_ = beam_lv_ + target_lv_;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::DefineCommands()
{
  // Define /hodoscope/generator command directory using generic messenger class
  messenger_ 
    = new G4GenericMessenger(this, 
        "/hodoscope/generator/", 
        "Primary generator control");

  // momentum command
  //auto& momentumCmd
  //  = messenger_->DeclarePropertyWithUnit("momentum", "GeV", momentum_, 
  //      "Mean momentum of primaries.");
  //momentumCmd.SetParameterName("p", true);
  //momentumCmd.SetRange("p>=0.");                                
  //momentumCmd.SetDefaultValue("1.");

  // randomizePrimary command
}

//..oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
