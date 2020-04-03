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
/// \copied from B5PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "TGenPhaseSpace.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "Constants.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include <vector>

class G4ParticleGun;
class G4String;
class G4GenericMessenger;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
    
    //inline void SetMomentum(G4double momentum) { momentum_ = momentum; }
    //inline G4double GetMomentum() const { return momentum_; }

  private:
    void ConstructParticleGun();
    void DefineCommands();
    void SetBeamAndTarget();
    inline void GenerateShot(G4Event* event);
    inline void GeneratePhaseSpace(G4Event* event);

    G4GenericMessenger* messenger_;

    G4int primary_mode_;

    G4int total_primaries_;
    std::vector <G4ParticleGun*> particleguns_;
    std::vector <G4String> particle_names_;
    std::vector <G4double> particle_masses_;

    // for shot-mode
    G4double shot_maximum_momentum_;

    // for phase-space-mode
    TGenPhaseSpace* phasespace_generator_;
    G4double maximum_weight_;
    G4double beam_momentum_;
    G4String beam_particle_name_;
    G4String target_particle_name_;
    TLorentzVector beam_lv_;
    TLorentzVector target_lv_;
    TLorentzVector initial_lv_;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::GenerateShot(G4Event* event)
{
  // shot random kinetic energy and direction
  
  // generate particles -------------------------
  for(auto* particlegun : particleguns_){
    auto momentum = G4UniformRand()*shot_maximum_momentum_;
    auto theta = G4UniformRand()*180.*deg;
    auto phi = G4UniformRand()*360.*deg;
    auto mass = particlegun->GetParticleDefinition()->GetPDGMass();
    auto kinetic_energy = std::sqrt(momentum*momentum+mass*mass)-mass;
    particlegun->SetParticleEnergy(kinetic_energy);
    particlegun->SetParticleMomentumDirection(
        G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta)));
    particlegun->GeneratePrimaryVertex(event);
  }
  // --------------------------------------------
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::GeneratePhaseSpace(G4Event* event)
{
  // generate phase space distribution

  // create phasespace_generator if not exist ---
  if(phasespace_generator_==nullptr){
    phasespace_generator_ = new TGenPhaseSpace();
    phasespace_generator_->SetDecay(initial_lv_,total_primaries_,&particle_masses_.front());

    // maximum_weight evaluation ----------------
    G4double diff_maximum_weight = 9999.;
    G4int number_of_generates = 0;
    maximum_weight_=0.;
    while(diff_maximum_weight>1e-5&&number_of_generates>1e6){
      G4double weight = phasespace_generator_->Generate();
      number_of_generates++;
      if(weight>maximum_weight_){
        diff_maximum_weight=weight-maximum_weight_;
        maximum_weight_=weight;
      }
    }
    // ------------------------------------------
  }
  // --------------------------------------------

  // generate phase space distribution ----------
  G4double weight = 0.;
  G4double random_weight = 9999.;
  while(random_weight>weight){
    weight = phasespace_generator_->Generate();
    random_weight = G4UniformRand()*maximum_weight_;
  }
  // --------------------------------------------

  // generate particles -------------------------
  for(G4int i_primary=0; i_primary<total_primaries_; i_primary++){
    auto* particlegun = particleguns_[i_primary];
    TVector3 momentum_vector_tv = phasespace_generator_->GetDecay(i_primary)->Vect();
    G4ThreeVector momentum_vector = ConvertVector(momentum_vector_tv);    
    //G4ThreeVector momentum_vector(
    //    momentum_vector_tv.X(), momentum_vector_tv.Y(), momentum_vector_tv.Z() );
    auto mass = particlegun->GetParticleDefinition()->GetPDGMass();
    auto kinetic_energy = std::sqrt(momentum_vector.mag2()+mass*mass)-mass;
    particlegun->SetParticleEnergy(kinetic_energy);
    particlegun->SetParticleMomentumDirection(momentum_vector/momentum_vector.mag());
    particlegun->GeneratePrimaryVertex(event);
  }
  // --------------------------------------------
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
