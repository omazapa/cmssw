
#include "DataFormats/L1THGCal/interface/HGCalCluster.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerGeometryBase.h"
#include "L1Trigger/L1THGCal/interface/HGCalTriggerNtupleBase.h"



class HGCalTriggerNtupleHGCClusters : public HGCalTriggerNtupleBase
{

  public:
    HGCalTriggerNtupleHGCClusters(const edm::ParameterSet& conf);
    ~HGCalTriggerNtupleHGCClusters(){};
    virtual void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) override final;
    virtual void fill(const edm::Event& e, const edm::EventSetup& es) override final;

  private:
    virtual void clear() override final;


    edm::EDGetToken clusters_token_;

    int cl_n_ ;
    std::vector<float> cl_pt_;
    std::vector<float> cl_energy_;
    std::vector<float> cl_eta_;
    std::vector<float> cl_phi_;
    std::vector<int> cl_layer_;
    std::vector<int> cl_ncells_;   

};

DEFINE_EDM_PLUGIN(HGCalTriggerNtupleFactory,
    HGCalTriggerNtupleHGCClusters,
    "HGCalTriggerNtupleHGCClusters" );


HGCalTriggerNtupleHGCClusters::
HGCalTriggerNtupleHGCClusters(const edm::ParameterSet& conf):HGCalTriggerNtupleBase(conf)
{
}

void
HGCalTriggerNtupleHGCClusters::
initialize(TTree& tree, const edm::ParameterSet& conf, edm::ConsumesCollector&& collector)
{
  clusters_token_ = collector.consumes<l1t::HGCalClusterBxCollection>(conf.getParameter<edm::InputTag>("Clusters"));

  tree.Branch("cl_n", &cl_n_, "cl_n/I");
  tree.Branch("cl_pt", &cl_pt_);
  tree.Branch("cl_energy", &cl_energy_);
  tree.Branch("cl_eta", &cl_eta_);
  tree.Branch("cl_phi", &cl_phi_);  
  tree.Branch("cl_layer", &cl_layer_);
  tree.Branch("cl_ncells", &cl_ncells_);
}

void
HGCalTriggerNtupleHGCClusters::
fill(const edm::Event& e, const edm::EventSetup& es)
{

  // retrieve clusters
  edm::Handle<l1t::HGCalClusterBxCollection> clusters_h;
  e.getByToken(clusters_token_, clusters_h);
  const l1t::HGCalClusterBxCollection& clusters = *clusters_h;

  // retrieve geometry
  edm::ESHandle<HGCalTriggerGeometryBase> geometry;
  es.get<IdealGeometryRecord>().get(geometry);

  clear();
  for(auto cl_itr=clusters.begin(0); cl_itr!=clusters.end(0); cl_itr++)
  {
    cl_n_++;
    // physical values 
    cl_pt_.emplace_back(cl_itr->pt());
    cl_energy_.emplace_back(cl_itr->energy());
    cl_eta_.emplace_back(cl_itr->eta());
    cl_phi_.emplace_back(cl_itr->phi());
    cl_layer_.emplace_back(cl_itr->layer());

    /* loop over the trigger cells pertinent to a cluster */
    const edm::PtrVector<l1t::HGCalTriggerCell> pertinentTC = cl_itr->triggercells();
    cl_ncells_.emplace_back(cl_itr->triggercells().size());
  }
}


void
HGCalTriggerNtupleHGCClusters::
clear()
{
  cl_n_ = 0;
  cl_pt_.clear();
  cl_energy_.clear();
  cl_eta_.clear();
  cl_phi_.clear();
  cl_layer_.clear();
  cl_ncells_.clear();
}




