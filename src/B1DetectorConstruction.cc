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
// $Id: B1DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Material
  G4Element* elementMg = new G4Element("Magnesium", "Mg",12., 24.3050*g/mole);
  G4Element* elementAl = new G4Element("Aluminium", "Al",13., 26.9815*g/mole);
  G4Element* elementFe = new G4Element("Iron",      "Fe",26.,  55.845*g/mole);
  G4Element* elementSi = new G4Element("Silicon",   "Si",14., 28.0855*g/mole);
  G4Element* elementH  = new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
  G4Element* elementC  = new G4Element("Carbon"  ,"C" , 6., 12.01*g/mole);
  G4Element* elementN  = new G4Element("Nitrogen","N" , 7., 14.01*g/mole);
  G4Element* elementO  = new G4Element("Oxygen"  ,"O" , 8., 16.00*g/mole);
  G4Element* elementCa = new G4Element("Calcium", "Ca", 20., 40.078*g/mole);
  G4Element* elementK = new G4Element("Kalium", "K", 19., 39.0983*g/mole);
  G4Element* elementNa = new G4Element("Natrium", "Na", 11., 22.989*g/mole);
  
  // Mars regolith
  G4Material* regolith_mat = new G4Material("Mars_regolith",1.7*g/cm3, 4);
  G4Material* SiO2 = nist -> FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Material* Fe2O3 = nist -> FindOrBuildMaterial("G4_FERRIC_OXIDE");
  G4Material* H2O = nist-> FindOrBuildMaterial("G4_WATER");
  
  G4Material* Al2CaK2MgNa2O7 = new G4Material("Al2CaK2MgNa2O7", 1.1811*g/cm3, 6);
  Al2CaK2MgNa2O7->AddElement(elementAl, 2);
  Al2CaK2MgNa2O7->AddElement(elementCa,1);
  Al2CaK2MgNa2O7->AddElement(elementK, 2);
  Al2CaK2MgNa2O7->AddElement(elementMg, 1);
  Al2CaK2MgNa2O7->AddElement(elementNa, 2);
  Al2CaK2MgNa2O7->AddElement(elementO, 7);
  
  regolith_mat->AddMaterial(SiO2, 0.512);
  regolith_mat->AddMaterial(Fe2O3, 0.093);  
  regolith_mat->AddMaterial(Al2CaK2MgNa2O7, 0.321);
  regolith_mat->AddMaterial(H2O, 0.074);
  
  // Mars Atmosphere
  G4Material* atmosphere_mat = new G4Material("Mars_atmosphere",0.3298*g/cm3,3);
  atmosphere_mat->AddMaterial(nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE"),0.957);
  atmosphere_mat->AddMaterial(nist->FindOrBuildMaterial("G4_lN2"),0.027);
  atmosphere_mat->AddMaterial(nist->FindOrBuildMaterial("G4_Ar"), 0.016);

  //Aluminium alloy 6061-T6
  G4Material* Al6061 = new G4Material("Al6061",2.70*g/cm3, 4);
  Al6061->AddElement(elementAl, 0.980);
  Al6061->AddElement(elementMg, 0.010);
  Al6061->AddElement(elementSi, 0.006);
  Al6061->AddElement(elementFe, 0.004);
  
  // Propane-based TE Gas
  G4double temperature = 293.15*kelvin; //room temperature
  G4double pressure = 0.0372*atmosphere; // 28.27 Torr
  G4double density = 0.0000667*g/cm3; // 2um simulated tissue
  G4Material* TEGas = new G4Material("propaneGas", density, 4, kStateGas, temperature, pressure);

  TEGas->AddElement(elementH, 10.27*perCent);
  TEGas->AddElement(elementC, 56.89*perCent);
  TEGas->AddElement(elementN, 3.5*perCent);  
  TEGas->AddElement(elementO, 29.34*perCent);

/*  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
*/
  //     
  // World : 50cm*50cm*50cm
  //
  G4Material* world_mat = nist -> FindOrBuildMaterial("G4_AIR");
  G4double world_size = 0.5*m;

  G4Box* solidWorld =    
    new G4Box("World",                       // its name
              0.5*world_size,                // half x
              0.5*world_size,                // half y
              0.5*world_size);               // half z
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);                 //overlaps checking
  
  //-----------------------------------------------------
  // Regolith
  
  G4double regolith_depth = 0.99*0.25*m;
  G4Box* solidRegolith = new G4Box("Regolith", 0.4999*world_size, 0.4999*world_size, 0.5*regolith_depth);
  G4LogicalVolume* logicRegolith = new G4LogicalVolume(solidRegolith, regolith_mat, "Regolith");
  logicRegolith->SetUserLimits(new G4UserLimits(10*cm));
  new G4PVPlacement(0,G4ThreeVector(0,0, -0.5*regolith_depth), logicRegolith, "Regolith", logicWorld, false,5, true);
  
  //------------------------------------------------------
  //Atmosphere
  G4Sphere* solidAtmosphere = new G4Sphere("Atmosphere", 6.980001*cm, 0.4999*world_size, 0, 360*degree, 0, 90*degree);
  G4LogicalVolume* logicAtmosphere = new G4LogicalVolume(solidAtmosphere,atmosphere_mat, "Atmosphere");
  logicAtmosphere->SetUserLimits(new G4UserLimits(10*cm));
  new G4PVPlacement(0, G4ThreeVector(0,0,0.00001*cm), logicAtmosphere,"Atmosphere",logicWorld,false,6, true);

  //------------------------------------------------------
  // Shield1 - Aluminium (6.8mm - ISS)
  G4Sphere* solidShield = new G4Sphere("Shield_Al", 6.300001*cm,6.98*cm, 0, 360*degree, 0, 90*degree);
  G4LogicalVolume* logicShield = new G4LogicalVolume(solidShield, nist->FindOrBuildMaterial("G4_Al"), "Shield_Al");
  logicShield->SetUserLimits(new G4UserLimits(1*mm));
  new G4PVPlacement(0, G4ThreeVector(0,0,0.00001*cm), logicShield, "Shield_Al", logicWorld, false, 7, true);
   
  //------------------------------------------------------
  // Shield2 - Mars regolith 1cm
  G4Sphere* solidShield2 = new G4Sphere("Shield_Mars_regolith", 5.3*cm,6.3*cm,0,360*degree,0, 90*degree);
  G4LogicalVolume* logicShield2 = new G4LogicalVolume(solidShield2, regolith_mat, "Shield_Mars_Regolith");
  logicShield->SetUserLimits(new G4UserLimits(1*mm));
  new G4PVPlacement(0, G4ThreeVector(0,0,0.00001*cm), logicShield2, "Shield_Mars_Regolith", logicWorld, false, 8, true);
   
  // -----------------------------------------------------
  // Detector - Al6061
  G4ThreeVector TEPC_center = G4ThreeVector(0, 0, 26.50000001*mm);
  G4Material* detector_mat = Al6061;
  G4Material* A_150_mat = nist -> FindOrBuildMaterial("G4_A-150_TISSUE");

  G4Sphere* solidDetector =    
    new G4Sphere("Al6061",
              26*mm,
              26.5*mm,
              0*degree,
              360*degree,
              0*degree,
              180*degree);
      
  G4LogicalVolume* logicDetector =                         
    new G4LogicalVolume(solidDetector,
                        detector_mat,
                        "Al6061");
  logicDetector -> SetUserLimits(new G4UserLimits(0.1*mm));                                 
    new G4PVPlacement(0,
                      TEPC_center,
                      logicDetector,
                      "Al6061",
                      logicWorld,
                      false,
                      1,
                      true);
  
  //=================Outer Propane================================
  G4Sphere* solidOuterPropane =
    new G4Sphere("OuterPropane",
   19*mm,
   25.999999999999*mm,
   0*degree,
   360*degree,
   0*degree,
   180*degree);
  
  G4LogicalVolume* logicOuterPropane =
    new G4LogicalVolume(solidOuterPropane,TEGas,"OuterPropane");  
  logicOuterPropane->SetUserLimits(new G4UserLimits(1*mm));
  new G4PVPlacement(0, TEPC_center, logicOuterPropane, "OuterPropane", logicWorld, false, 2, true);
  
  //========= A-150================================
  G4Sphere* solidA_150 = new G4Sphere("A-150", 15*mm, 18.9999999999999*mm, 0*degree, 360*degree, 
  0*degree, 180*degree);

  G4LogicalVolume* logicA_150 = new G4LogicalVolume(solidA_150,A_150_mat ,"A-150");
  logicA_150->SetUserLimits(new G4UserLimits(1*mm));
  new G4PVPlacement(0, TEPC_center, logicA_150, "A-150", logicWorld, false, 3, true);
  //=========Inner Propane========================================
  G4Orb* solidInnerPropane = new G4Orb("InnerPropane", 14.99999999999*mm);
  G4LogicalVolume* logicInnerPropane = new G4LogicalVolume(solidInnerPropane,TEGas,"InnerPropane");
  logicInnerPropane->SetUserLimits(new G4UserLimits(1*mm));
  new G4PVPlacement(0, TEPC_center, logicInnerPropane, "InnerPropane", logicWorld, false, 4, true);
  
 // Set Inner Propane as scoring volume
  //
  fScoringVolume = logicInnerPropane;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
