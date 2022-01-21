#ifndef LHEInvariantMassFilter_h
#define LHEInvariantMassFilter_h
// -*- C++ -*-
//
// Package:    LHEInvariantMassFilter
// Class:      LHEInvariantMassFilter
//
/* 

 Description: Filter to select events based on invariant mass of arbitrary number of particles.
 Implementation: derived from LHEGenericFilter
     
*/
//
// Original Author:  Afiq Anuar
//         Created:  Wed Apr 21 2021
//
//

// system include files
#include <memory>
#include <iostream>
#include <algorithm>

// user include files
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

//
// class declaration
//

class LHEInvariantMassFilter : public edm::EDFilter {
public:
  explicit LHEInvariantMassFilter(const edm::ParameterSet&);
  ~LHEInvariantMassFilter() override;

private:
  bool filter(edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // ----------member data ---------------------------

  edm::EDGetTokenT<LHEEventProduct> src_;
  std::vector<int> particleID_;  // vector of particle IDs to look for
  double minmass_;
  double maxmass_;
  int total_; // counters
  int pass_;
};
#endif
