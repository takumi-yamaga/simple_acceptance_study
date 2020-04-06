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
/// \file of ParticleHit.cc
/// \brief Implementation of the ParticleHit class

#include "ParticleHit.hh"
#include "Constants.hh"

#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<ParticleHit>* ParticleHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParticleHit::ParticleHit()
: G4VHit(), 
  generation_(-1), track_id_(-1), parent_id(-1), particle_name_(""), initial_momentum_(0), initial_position(0) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParticleHit::~ParticleHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ParticleHit::ParticleHit(const ParticleHit &right)
  : G4VHit(),
  // hit particle
  generation_(right.generation_),
  track_id_(right.track_id_),
  parent_id_(right.parent_id_),
  particle_name_(right.particle_name_),
  initial_momentum_(right.initial_momentum_),
  initial_position_(right.initial_position_),
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ParticleHit& ParticleHit::operator=(const ParticleHit &right)
{
  generation_ = right.generation_;
  track_id_ = right.track_id_;
  parent_id_ = right.parent_id_;
  particle_name_ = right.particle_name_;
  initial_momentum_ = right.initial_momentum_;
  initial_position_ = right.initial_position_;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ParticleHit::operator==(const ParticleHit &/*right*/) const
{
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ParticleHit::Print()
{
  G4cout << "ParticleHit -------------------------" << G4endl;
  G4cout << " track_id      : " << track_id_ << G4endl;
  G4cout << " particle_name : " << particle_name_ << G4endl;
  G4cout << "-------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
