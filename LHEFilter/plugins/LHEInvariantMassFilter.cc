#include "GeneratorEventFilter/LHEFilter/plugins/LHEInvariantMassFilter.h"

using namespace edm;

LHEInvariantMassFilter::LHEInvariantMassFilter(const edm::ParameterSet& iConfig)
    : particleID_(iConfig.getParameter<std::vector<int>>("ParticleID")),
      minmass_(iConfig.getParameter<double>("minMass")),
      maxmass_(iConfig.getParameter<double>("maxMass")),
      total_(0),
      pass_(0) {
  // here do whatever other initialization is needed
  src_ = consumes<LHEEventProduct>(iConfig.getParameter<edm::InputTag>("src"));
}

LHEInvariantMassFilter::~LHEInvariantMassFilter() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
}

// ------------ method called to skim the data  ------------
bool LHEInvariantMassFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<LHEEventProduct> evt;
  iEvent.getByToken(src_, evt);

  ++total_;

  const lhef::HEPEUP hepeup_ = evt->hepeup();
  const uint nup_ = hepeup_.NUP; // number of particles in event
  if (nup_ < particleID_.size())
    return false;

  const std::vector<int> idup_ = hepeup_.IDUP;
  const std::vector<int> idx_ = [this, &idup_] () {
    std::vector<int> idx_;

    for (auto &ip : particleID_) {
      auto id = std::find(idup_.begin(), idup_.end(), ip);

      while (id != idup_.end()) {
        auto idx = std::distance(idup_.begin(), id);
        if (std::find(idx_.begin(), idx_.end(), idx) == idx_.end())
          idx_.push_back(idx);

        id = std::find(std::next(id), idup_.end(), ip);
      }
    }

    return (idx_.size() == particleID_.size()) ? idx_ : std::vector<int>{};
  }();

  if (idx_.empty())
    return false;

  const std::vector<lhef::HEPEUP::FiveVector> pup_ = hepeup_.PUP;
  ROOT::Math::PxPyPzEVector sump4_(0., 0., 0., 0.);
  for (auto &i : idx_)
    sump4_ += ROOT::Math::PxPyPzEVector(pup_[i][0], pup_[i][1], pup_[i][2], pup_[i][3]);
  const double mass_ = sump4_.mass();

  if (mass_ < minmass_ or mass_ > maxmass_)
    return false;

  ++pass_;
  return true;
}

// ------------ method called once each job just after ending the event loop  ------------
void LHEInvariantMassFilter::endJob() {
  edm::LogInfo("LHEInvariantMassFilter") << "=== Results of LHEInvariantMassFilter: passed " << pass_ << "/" << total_
                                         << " events" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(LHEInvariantMassFilter);
