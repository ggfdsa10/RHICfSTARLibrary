
#include "StRHICfSimConvertor.h"

// STAR headers
#include "StChain/StChain.h"
#include "StChain/StChainOpt.h"
#include "StEvent/StBbcTriggerDetector.h"

// StMuEvents headers
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"

// StarGenEvent
#include "StarGenerator/BASE/StarGenerator.h"
#include "StarGenerator/BASE/StarPrimaryMaker.h"
#include "StarGenerator/EVENT/StarGenPPEvent.h"
#include "StarGenerator/EVENT/StarGenEvent.h"
#include "StarGenerator/EVENT/StarGenParticle.h"

// StRHICfSimDst 
#include "StRHICfPool/StRHICfSimDst/StRHICfSimPar.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimDst.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimEvent.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimTrack.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimBBC.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimBTof.h"
#include "StRHICfPool/StRHICfSimDst/StRHICfSimZDC.h"

#include "StEvent/StEnumerations.h"
#include "StEvent/StRHICfCollection.h"
#include "StEvent/StRHICfHit.h"
#include "StEvent/StRHICfPoint.h"

StRHICfSimConvertor::StRHICfSimConvertor(int convertFlag, const char* fileName, const Char_t* name) 
: StMaker(name), mConvertFlag(convertFlag), mInputFile(fileName), mRHICfRunType(-1)
{
}

StRHICfSimConvertor::~StRHICfSimConvertor()
{
}

Int_t StRHICfSimConvertor::Init()
{
    if(mConvertFlag == kMuDst2SimDst){InitMuDst2SimDst();}
    if(mConvertFlag == kSimRecoMode){InitSimRecoMode();}

    return kStOk;
}

Int_t StRHICfSimConvertor::Make()
{
    LOG_INFO << "StRHICfSimConvertor::Make()" << endm;
    if(mConvertFlag == kMuDst2SimDst){ConvertMuDst2SimDst();}
    if(mConvertFlag == kSimRecoMode){
        if(mSimDstTree->GetEntries() <= mEvent){return kStEOF;}
        RecoSimulation();
    }

    return kStOk;
}

Int_t StRHICfSimConvertor::Finish()
{
    if(mConvertFlag == kMuDst2SimDst){
        mSimDstFile -> cd();
        mSimDstTree -> Write();
        mSimDstFile -> Close();
    }

    if(mConvertFlag == kSimRecoMode){
        mOutSimDstFile -> cd();
        mOutSimDstTree -> Write();
        mOutSimDstFile -> Close();
    }

    return kStOk;
}

Int_t StRHICfSimConvertor::clear()
{
    return kStOk;
}

Int_t StRHICfSimConvertor::InitMuDst2SimDst()
{
    mChain = new TChain("genevents");

    TObjArray *tokens;

    TString generatorFile = "";
    if(mInputFile.Length() == 0){
        LOG_ERROR << "Input file is not a existing ... " << endm;
        return kStFatal;
    }
    else{
        int fileNum = 0;
        TString muDstFile = "";
        if( mInputFile.Index(".list") != -1 || mInputFile.Index(".lis") != -1 ){
            std::ifstream inputStream( mInputFile.Data() );
            if(!inputStream) {LOG_ERROR << "ERROR: Cannot open list file " << mInputFile << endm;}

            std::string file;
            size_t pos;
            while(getline(inputStream, file)){
                pos = file.find_first_of(" ");
                if (pos != std::string::npos ) file.erase(pos,file.length()-pos);
                if(file.find("MuDst") != std::string::npos){
                    muDstFile = file;
                    fileNum++;
                }
            }
            if(fileNum != 1){
                LOG_ERROR << "StRHICfSimConvertor::InitMuDst2SimDst() -- Input file must be one MuDst file" << endm;
                return kStFatal;
            }
        }
        if(muDstFile == ""){muDstFile = mInputFile;}
        if(muDstFile.Index("MuDst") != -1) {
            mOutputFile = muDstFile;
            tokens = mOutputFile.Tokenize("/");
            mOutputFile = ((TObjString *) tokens -> At(tokens->GetEntries()-1)) -> GetString();
            mOutputFile.ReplaceAll(".MuDst.root",".RHICfSimDst.root");

            generatorFile = muDstFile;
            generatorFile.ReplaceAll(".MuDst.root", ".genevents.root");
            int valid = mChain -> Add(generatorFile);
            if(valid == 0){
                LOG_ERROR << "StRHICfSimConvertor::InitMuDst2SimDst() -- There is no Generator ROOT file !!! " << endm;
                return kStFatal;
            }
            LOG_INFO << Form("StRHICfSimConvertor::InitMuDst2SimDst() -- Find a Event generator file: %s",generatorFile.Data()) << endm;

            for(int i=0; i<=rGeneratorNum; i++){
                TString generatorName = StRHICfSimPar::GetGeneratorName(i);
                if(mOutputFile.Index(generatorName) != -1){
                    LOG_INFO << Form("StRHICfSimConvertor::InitMuDst2SimDst() -- Find a Event generator name: %s",generatorName.Data()) << endm;
                    mGeneratorIdx = i;
                    break;
                }
            }
            if(mGeneratorIdx >= rGeneratorNum){
                LOG_ERROR << "StRHICfSimConvertor::InitMuDst2SimDst() -- Can not find a event generator type !!!" << endm;
                return kStFatal;
            }
        }
    }

    LOG_INFO << Form("StRHICfSimConvertor::InitMuDst2SimDst() -- Output file name: %s",mOutputFile.Data()) << endm;
    
    mSimDstFile = new TFile(mOutputFile.Data(), "RECREATE");
    mSimDstTree = new TTree("StRHICfSimDst","StRHICfSimDst");

    mSimDst = new StRHICfSimDst();
    mSimDst -> CreateDstArray(mSimDstTree);

    TString generatorType = "StRHICfSimGenerator";
    if(mOutputFile.Index("Pythia8") != -1){generatorType = "Pythia8";}

    mGenPPEvent = new StarGenPPEvent(generatorType);
    mGenEvent = new StarGenEvent("primaryEvent");

    mChain -> SetBranchAddress(generatorType, &mGenPPEvent);
    mChain -> SetBranchAddress("primaryEvent", &mGenEvent);
    mTmpChainEvent = 0;

    InitRHICfGeometry();

    mDatabasePDG = new TDatabasePDG();

    return kStOk;
}

Int_t StRHICfSimConvertor::InitSimRecoMode()
{
    if(mInputFile.Length() == 0){
        LOG_ERROR << "Input file is not a existing ... " << endm;
        return kStFatal;
    }
    else{
        if(mInputFile.Index("rhicfsim.RHICfSimDst.root") != -1 && mInputFile.Index("reco") == -1) {
            LOG_INFO << Form("StRHICfSimConvertor::InitSimRecoMode() -- Find a SimDst file: %s",mInputFile.Data()) << endm;

            mSimDstFile = new TFile(mInputFile.Data(), "READ");
            mSimDstTree = (TTree*)mSimDstFile -> Get("StRHICfSimDst");

            mSimDst = new StRHICfSimDst();
            mSimDst -> ReadDstArray(mSimDstTree);

            mInputFile.ReplaceAll("rhicfsim.RHICfSimDst.root", "reco.rhicfsim.RHICfSimDst.root");
            TString outputFileName = mInputFile;
            mOutSimDstFile = new TFile(outputFileName.Data(), "RECREATE");
            mOutSimDstTree = mSimDstTree -> CloneTree(0);

            LOG_INFO << Form("StRHICfSimConvertor::InitSimRecoMode() -- Output File: %s",outputFileName.Data()) << endm;
        }
        else{
            LOG_ERROR << Form("StRHICfSimConvertor::InitSimRecoMode() -- Input file has a different format !!! %s",mInputFile.Data()) << endm;
            return kStFatal;
        }
    }
    if(!mSimDst){
        LOG_ERROR << Form("StRHICfSimConvertor::InitSimRecoMode() -- Could not read file: %s",mInputFile.Data()) << endm;
        return kStFatal;
    }

    mRHICfPointMaker = new StRHICfPointMaker();
    mRHICfPointMaker -> setMCReco();

    mEvent = 0;

    return kStOk;
}

Int_t StRHICfSimConvertor::ConvertMuDst2SimDst()
{
    mSimDst -> Clear();
    mRHICfGammaIdx.clear();
    mRHICfNeuIdx.clear();

    // =================== DST Set up ======================
    mMuDst = (StMuDst*) GetInputDS("MuDst"); // from DST
    if(!mMuDst) {LOG_ERROR << "no StMuDst" << endm;  return kStFatal;};

    mMuEvent = mMuDst->event(); // from muDST
    if(!mMuEvent) {LOG_ERROR << "no StMuEvent" << endm;  return kStFatal;};

    // Get the event info
    mSimEvent = mSimDst -> GetSimEvent();
    int eventNumber = mMuEvent -> eventNumber();

    mSimEvent -> SetEventNumber(eventNumber);
    mSimEvent -> SetRHICfRunType(mRHICfRunType);
    if(mGeneratorIdx < rGeneratorNum){mSimEvent -> SetGeneratorIdx(mGeneratorIdx);}

    // ======================= Get MuMcTrack ============================
    mMcVtxArray = mMuDst -> mcArray(0);
    mMcTrkArray = mMuDst -> mcArray(1); 

    int primaryTrkNum = 0;
    int propagatedTrkNum = 0;
    int mcTrkNum = mMcTrkArray -> GetEntriesFast();
    for(int i=0; i<mcTrkNum; i++){
        mMcTrk = (StMuMcTrack*)mMcTrkArray -> UncheckedAt(i);
        int VtxIdStart = mMcTrk -> IdVx();
        int VtxIdEnd = mMcTrk -> IdVxEnd();
        bool isPrimary = (VtxIdStart == 1)? true : false;
        if(isPrimary){primaryTrkNum++;}

        double posStart[3];
        mMcVtx = (StMuMcVertex*)mMcVtxArray -> UncheckedAt(VtxIdStart-1);
        posStart[0] = mMcVtx->XyzV().x();
        posStart[1] = mMcVtx->XyzV().y();
        posStart[2] = mMcVtx->XyzV().z();

        int parentTrkId = mMcVtx -> IdParTrk() - 1; // starting of parentTrk Id from 0 index
        int daughterNum = mMcVtx -> NoDaughters();

        double posEnd[3] = {-9999., -9999., -9999.};
        if(VtxIdEnd != 0){
            mMcVtx = (StMuMcVertex*)mMcVtxArray -> UncheckedAt(VtxIdEnd-1);
            posEnd[0] = mMcVtx->XyzV().x();
            posEnd[1] = mMcVtx->XyzV().y();
            posEnd[2] = mMcVtx->XyzV().z();
        }

        int trkId = mMcTrk -> Id() -1; // starting of track Id from 0 index
        int gePid = mMcTrk -> GePid();
        int pid = GetGePid2PDG(gePid);
        if(pid == 0){continue;} // for invaild PDG Encoding
        
        double energy = mMcTrk -> E();
        double mom[3];
        mom[0] = mMcTrk -> Pxyz().x();
        mom[1] = mMcTrk -> Pxyz().y();
        mom[2] = mMcTrk -> Pxyz().z();

        // Save track info into StRHICfSimTrack
        mSimTrk = mSimDst -> GetSimTrack(mSimDst->GetSimTrackNum());
        mSimTrk -> SetId(trkId);
        mSimTrk -> SetPid(pid);
        mSimTrk -> SetParentId(parentTrkId);
        mSimTrk -> SetDaughterNum(daughterNum);
        mSimTrk -> SetEnergy(energy);
        mSimTrk -> SetMomentum(mom[0], mom[1], mom[2]);   
        mSimTrk -> SetVertexStart(posStart[0], posStart[1], posStart[2]);
        mSimTrk -> SetVertexEnd(posEnd[0], posEnd[1], posEnd[2]);
        if(isPrimary){mSimTrk -> SetIsPrimary();}
        
        if(IsSimPropagate(mSimTrk)){
            mSimTrk->SetIsSimPropagate();
            propagatedTrkNum++;
        }

        // find a RHICf gamma or neutron
        if(pid == 22 || pid == 2112){
            int hit = GetRHICfGeoHit(posStart[0], posStart[1], posStart[2], mom[0], mom[1], mom[2], energy);
            if(hit != -1){
                if(pid == 22){mRHICfGammaIdx.push_back(mSimDst->GetSimTrackNum()-1);} // gamma
                if(pid == 2112){
                    mRHICfNeuIdx.push_back(mSimDst->GetSimTrackNum()-1);} // neutron
            }
        }
    }

    mSimEvent -> SetPrimaryTrkNum(primaryTrkNum);

    GetGeneratorData();

    // ======================= Get BBC ============================
    mSimBBC = mSimDst -> GetSimBBC();
    StBbcTriggerDetector bbc = mMuEvent -> bbcTriggerDetector();
    bbc.setYear(2017);
    int bbcPMTNum = bbc.numberOfPMTs();
    for(int i=0; i<bbcPMTNum; i++){
        int adc = bbc.adc(i);
        int dir = (i < bbcPMTNum/2)? rEast : rWest;

        // small tile
        if((i < rBBCSmallPmtNum) || (bbcPMTNum/2 <= i && i < bbcPMTNum/2 + rBBCSmallPmtNum)){
            int pmtIdx = (dir == rEast)? i : i - bbcPMTNum/2;
            mSimBBC -> SetSmallADC(dir, pmtIdx, adc);
        }
        else{ // large tile
            int pmtIdx = (dir == rEast)? i - rBBCSmallPmtNum : i - (bbcPMTNum/2 + rBBCSmallPmtNum);
            mSimBBC -> SetLargeADC(dir, pmtIdx, adc);
        }
    }

    // ======================= Get B-TOF ============================
    Int_t bTofTrayMult = mMuEvent -> btofTrayMultiplicity();
    Int_t bTofMult = mMuDst -> GetNBTofHit();
    for (int i=0; i<bTofMult; ++i) {
        mMuBTofHit = (StMuBTofHit*)mMuDst -> btofHit(i);
        if (!mMuBTofHit) continue;
        if (mMuBTofHit->tray() > 120) continue; // BTof has only 120 trays, over the 120 index will be VPD hit info
        int trayId = mMuBTofHit -> tray();
        int moduleId = mMuBTofHit -> module();
        int cellId = mMuBTofHit -> cell();
        int idTruth = mMuBTofHit -> idTruth();

        mSimBTof = mSimDst -> GetSimBTof(mSimDst -> GetSimBTofNum());
        mSimBTof -> SetTray(trayId);
        mSimBTof -> SetModule(moduleId);
        mSimBTof -> SetCell(cellId);
        mSimBTof -> SetSimTrkId(idTruth);
    }

    mSimDstTree -> Fill();

    // Event Summary print
    LOG_INFO << "StRHICfSimConvertor::ConvertMuDst2SimDst() -- Event Summary" << endm;
    LOG_INFO << "Process Id: " << mSimEvent -> GetProcessId() << endm;
    LOG_INFO << "Primary Trk Num: " << mSimEvent->GetPrimaryTrkNum() << ", Generate level Priamry Trk Num: " << mSimEvent->GetGenFinalParNum() << ",  Propagated Trk Num: " << propagatedTrkNum << endm;
    LOG_INFO << "RHICf Gamma Num: " << mRHICfGammaIdx.size() << ",  RHICf Neutron Num: " << mRHICfNeuIdx.size() << endm;

    return kStOk;
}

Int_t StRHICfSimConvertor::GetGeneratorData()
{
    int simDstEvent = mSimEvent -> GetEventNumber();

    for(int event=mTmpChainEvent; event<mChain->GetEntriesFast(); event++){
        mChain -> GetEntry(event);

        if(mGenEvent -> GetFilterResult() != 1){continue;}
        if(simDstEvent == mGenEvent->GetEventNumber()){
            mTmpChainEvent = event;

            // Set the Event gernerator information
            int processId = mGenPPEvent -> process;
            double xParton1 = mGenPPEvent -> xParton1;
            double xParton2 = mGenPPEvent -> xParton2;
            double xPdf1 =  mGenPPEvent -> xPdf1;
            double xPdf2 =  mGenPPEvent -> xPdf2;
            double Q2Fac =  mGenPPEvent -> Q2fac;
            double Q2Ren =  mGenPPEvent -> Q2ren;
            double sHat =  mGenPPEvent -> sHat;
            double tHat =  mGenPPEvent -> tHat;
            double uHat =  mGenPPEvent -> uHat;
            double ptHat =  mGenPPEvent -> ptHat;

            int finalParNum = 0;
            int finalChargedParNum = 0;

            mSimEvent -> SetProcessId(processId);
            mSimEvent -> SetXParton1(xParton1);
            mSimEvent -> SetXParton2(xParton2);
            mSimEvent -> SetXPdf1(xPdf1);
            mSimEvent -> SetXPdf2(xPdf2);
            mSimEvent -> SetQ2Fac(Q2Fac);
            mSimEvent -> SetQ2Renorm(Q2Ren);
            mSimEvent -> SetsHat(sHat);
            mSimEvent -> SettHat(tHat);
            mSimEvent -> SetuHat(uHat);
            mSimEvent -> SetPtHat(ptHat);

            // Find a RHICf particle mothers
            vector<pair<int, int>> pairRHICfParticles;

            int RHICfGammaNum = mRHICfGammaIdx.size();
            int RHICfNeuNum = mRHICfNeuIdx.size();
            int totalRHICfParNum  = RHICfGammaNum + RHICfNeuNum;

            int parNum = mGenEvent -> GetNumberOfParticles();
            for(int par=0; par<parNum; par++){
                mGenParticle = (*mGenEvent)[par];
                int status = mGenParticle -> GetStatus();
                int pid = mGenParticle -> GetId();

                // Diffraction informations
                if(status == 15){
                    double diffMass = mGenParticle -> GetMass();
                    double px = mGenParticle -> GetPx();
                    double py = mGenParticle -> GetPy();
                    double pz = mGenParticle -> GetPz();
                    double p = sqrt(px*px + py*py + pz*pz);
                    double cosTheta = (p == 0.)? 1. : pz/p;
                    double diffEta = -0.5*TMath::Log((1. - cosTheta)/(1. + cosTheta));
                    if(par == 3){
                        mSimEvent -> SetDiffractionAMass(diffMass);
                        mSimEvent -> SetDiffractionAEta(diffEta);
                    }
                    if(par == 4){
                        mSimEvent -> SetDiffractionBMass(diffMass);
                        mSimEvent -> SetDiffractionBEta(diffEta);
                    }
                    if(par == 5){
                        mSimEvent -> SetDiffractionCMass(diffMass);
                        mSimEvent -> SetDiffractionCEta(diffEta);
                    }
                }
                // Get the number of final state particle 
                if(status == 1){
                    finalParNum++;
                    if(abs(pid) == 11 || abs(pid) == 13 || pid == 22 || abs(pid) == 211 || abs(pid) == 321 || abs(pid) == 2212){
                        finalChargedParNum++;
                    }
                }

                if(pid != 22 && pid != 2112){continue;}

                int genPtInt = int(mGenParticle -> pt()*10000);
                int genEnergyInt = int(mGenParticle -> GetEnergy()*100);

                for(int r=0; r<totalRHICfParNum; r++){
                    int simTrkIdx = (r < RHICfGammaNum)? mRHICfGammaIdx[r] : mRHICfNeuIdx[r-RHICfGammaNum];
                    mSimTrk = mSimDst -> GetSimTrack(simTrkIdx);

                    double px = mSimTrk -> GetPx();
                    double py = mSimTrk -> GetPy();
                    int simTrkPtInt = int(sqrt(px*px + py*py)*10000);
                    int simTrkEnergyInt = int(mSimTrk->GetE()*100);
                    int simTrkPid = mSimTrk -> GetPid();

                    // Getting the same RHICf particle 
                    if(genEnergyInt == simTrkEnergyInt && genPtInt == simTrkPtInt && pid == simTrkPid){
                        pairRHICfParticles.push_back(make_pair(simTrkIdx, par));
                    }
                }
                if(int(pairRHICfParticles.size()) == totalRHICfParNum){break;}
            }

            // Set the generator level number of particles
            mSimEvent -> SetGenFinalParNum(finalParNum);
            mSimEvent -> SetGenFinalChargedParNum(finalChargedParNum);

            if(pairRHICfParticles.size() == 0){break;}

            int tmpSimTrkNum = mSimDst -> GetSimTrackNum();
            for(int i=0; i<totalRHICfParNum; i++){
                int simTrkIdx = pairRHICfParticles[i].first;
                int genParIdx = pairRHICfParticles[i].second;

                bool duplicateFlag = false;
                while(true){
                    mSimTrk = mSimDst -> GetSimTrack(simTrkIdx);
                    int simTrkParentId = mSimTrk -> GetParentId();

                    mGenParticle = (*mGenEvent)[genParIdx];
                    int genParentId1 = mGenParticle -> GetFirstMother();
                    int genParentId2 = mGenParticle -> GetLastMother();

                    if(duplicateFlag){duplicateFlag = false; break;}
                    if(simTrkParentId < 0){
                        if(genParentId2 != 0){ // primary particle case
                            mSimTrk -> SetIsPrimary();
                            mSimEvent -> SetPrimaryTrkNum(mSimEvent->GetPrimaryTrkNum()+1);
                            break; 
                        }
                        else{ // decayed particle case: but MuDst not include mother particle
                            int motherSimTrkId = mSimDst->GetSimTrackNum();
                            double StartVx = mSimTrk->GetVxStart();
                            double StartVy = mSimTrk->GetVyStart();
                            double StartVz = mSimTrk->GetVzStart();

                            mGenParticle = (*mGenEvent)[genParentId1];

                            // Find the duplicated mother particle in SimTrack
                            int duplicatedMotherIdx = -1;
                            int genPID = mGenParticle -> GetId();
                            int genPtInt = int(mGenParticle -> pt()*10000);
                            int genEnergyInt = int(mGenParticle -> GetEnergy()*10000);

                            for(int i=tmpSimTrkNum-1; i<mSimDst->GetSimTrackNum(); i++){
                                mSimTrk = mSimDst -> GetSimTrack(i);
                                double px = mSimTrk -> GetPx();
                                double py = mSimTrk -> GetPy();
                                int simTrkPtInt = int(sqrt(px*px + py*py)*10000);
                                int simTrkEnergyInt = int(mSimTrk->GetE()*10000);
                                int simTrkPID = mSimTrk -> GetPid();

                                if(genEnergyInt == simTrkEnergyInt && genPtInt == simTrkPtInt && genPID == simTrkPID){
                                    duplicatedMotherIdx = i;
                                    break;
                                }
                            }

                            mSimTrk = mSimDst -> GetSimTrack(simTrkIdx);
                            if(duplicatedMotherIdx > 0){
                                mSimTrk -> SetParentId(duplicatedMotherIdx); 
                                simTrkIdx = duplicatedMotherIdx;
                                genParIdx = genParentId1;
                                duplicateFlag = true;
                                continue;
                            }
                            else{
                                mSimTrk = mSimDst -> GetSimTrack(simTrkIdx);
                                mSimTrk -> SetParentId(motherSimTrkId); // for duaghter particle 
                            }

                            mSimTrk = mSimDst -> GetSimTrack(motherSimTrkId); // set mother particle
                            mSimTrk -> SetId(motherSimTrkId);
                            mSimTrk -> SetPid(mGenParticle->GetId()); 
                            mSimTrk -> SetDaughterNum(2);
                            mSimTrk -> SetEnergy(mGenParticle->GetEnergy());
                            mSimTrk -> SetMomentum(mGenParticle->GetPx(), mGenParticle->GetPy(), mGenParticle->GetPz());

                            double motherStartVx = mGenParticle->GetVx();
                            double motherStartVy = mGenParticle->GetVy();
                            double motherStartVz = mGenParticle->GetVz();
                            mSimTrk -> SetVertexStart(motherStartVx, motherStartVy, motherStartVz);
                            mSimTrk -> SetVertexEnd(StartVx, StartVy, StartVz);
                            mSimTrk -> SetIsPrimary();
                            mSimEvent -> SetPrimaryTrkNum(mSimEvent->GetPrimaryTrkNum()+1);

                            // set the grandmother particle index
                            simTrkIdx = motherSimTrkId;
                            genParIdx = genParentId1;
                        }
                    }
                    else{ // decayed particle case: MuDst have a mother particle
                        if(genParentId2 != 0){ // primary particle case
                            mSimTrk -> SetIsPrimary();
                            mSimEvent -> SetPrimaryTrkNum(mSimEvent->GetPrimaryTrkNum()+1);
                            break;  
                        }
                        else{
                            // set the grandmother particle index
                            simTrkIdx = simTrkParentId;
                            genParIdx = genParentId1;
                        }
                    }
                }
            }
            break;
        }
    }
    return kStOk;
}

bool StRHICfSimConvertor::IsSimPropagate(StRHICfSimTrack* simTrk)
{
    float startVx = simTrk->GetVxStart();
    float startVy = simTrk->GetVyStart();
    float startVz = simTrk->GetVzStart();
    float endVz = simTrk->GetVzEnd();

    // 1st cut: if track is not alive 
    if(endVz > -9990.){return false;} 

    // 2nd cut: if track eta < 5
    float eta = simTrk -> GetEta();
    if(eta < 5){return false;}

    // 3nd cut: startVz larger than 10m (DX magnet)
    if(startVz > 1000){return false;}

    float px = simTrk->GetPx();
    float py = simTrk->GetPy();
    float pz = simTrk->GetPz();
    float p = sqrt(px*px + py*py + pz*pz);

    float unitX = px/p;
    float unitY = py/p;
    float unitZ = pz/p;

    // 4th cut: if Z diraction is negative
    if(unitZ < 0.){return false;}

    float startR = sqrt(startVx*startVx + startVy*startVy);
    
    // 5th cut: if startR > DX magnet diameter and unitX and unitY are positive.
    if(startR > 30.){
        if(unitX > 0. && unitY > 0.){return false;}
    }

    return true;
}

Int_t StRHICfSimConvertor::GetGePid2PDG(int gepid)
{
    if(gepid == 45){return 1000010020;} // Deuteron case
    if(gepid == 46){return 1000010030;} // Triron case
    if(gepid == 47){return 1000020040;} // Alpha case

    return mDatabasePDG -> ConvertGeant3ToPdg(gepid);
}

Int_t StRHICfSimConvertor::RecoSimulation()
{
    FillMCData();

    mRHICfPointMaker -> InitRun(18178002); // this run number is for initialization of RHICfDbMaker, it doesn't dependent to reco
    mRHICfPointMaker -> Make();
    SaveRecoData();

    return kStOk;
}

Int_t StRHICfSimConvertor::FillMCData()
{
    LOG_INFO << "StRHICfSimConvertor::FillMCData() -- Start the RHICf simulation reconstruction, event: " << mEvent << " / " << mSimDstTree->GetEntries()-1 << endl;
    mSimDstTree -> GetEntry(mEvent); 

    mRHICfColl = new StRHICfCollection();
    mRHICfColl -> isAllSave();

    mRHICfHit = mRHICfColl -> hitCollection();
    mRHICfHit -> initDataArray();
    mSimRHICfHit = mSimDst -> GetSimRHICfHit();

    mSimEvent = mSimDst -> GetSimEvent();
    mRHICfColl -> setRunType(mSimEvent -> GetRHICfRunType());

    for(int it=0; it<kRHICfNtower; it++){
        for(int ip=0; ip<kRHICfNplate; ip++){
            Float_t plateEnergy = mSimRHICfHit -> GetPlatedE(it, ip);
            mRHICfHit -> setPlateEnergy(it, ip, plateEnergy);
        }
        for(int il=0; il<kRHICfNlayer; il++){
            for(int ixy=0; ixy<kRHICfNxy; ixy++){
                int chSize = (it == 0)? 20 : 40;
                for(int ich=0; ich<chSize; ich++){
                    Float_t gsobarEnergy = mSimRHICfHit -> GetGSOBardE(it, il, ixy, ich);
                    mRHICfHit -> setGSOBarEnergy(it, il, ixy, ich, gsobarEnergy);
                }
            }
        }
    }
    mRHICfPointMaker -> setMCCollection(mRHICfColl);

    mEvent++;
    return kStOk;
}

Int_t StRHICfSimConvertor::SaveRecoData()
{
    mRHICfHit = mRHICfColl -> hitCollection();
    mSimRHICfHit = mSimDst -> GetSimRHICfHit();

    // RHICf Hit
    for(int it=0; it<kRHICfNtower; it++){
        for(int io=0; io<2; io++){
            Int_t gsoMaxLayer = mRHICfHit -> getGSOMaxLayer(it, io);
            mSimRHICfHit -> SetGSOMaxLayer(it, io, gsoMaxLayer);
        }

        Float_t l20 = mRHICfHit -> getL20(it);
        Float_t l90 = mRHICfHit -> getL90(it);
        mSimRHICfHit -> SetL20(it, l20);
        mSimRHICfHit -> SetL90(it, l90);
        
        Int_t multiHitNum = mRHICfHit -> getMultiHitNum(it);
        mSimRHICfHit -> SetMultiHitNum(it, multiHitNum);
        
        for(int il=0; il<kRHICfNlayer; il++){
            for(int ixy=0; ixy<kRHICfNxy; ixy++){

                Int_t singleHitNum = mRHICfHit -> getSingleHitNum(it, il, ixy);
                Int_t maxPeakBin = mRHICfHit -> getMaxPeakBin(it, il, ixy);
                Float_t singleHitPos = mRHICfHit -> getSingleHitPos(it, il, ixy);
                Float_t singlePeakHeight = mRHICfHit -> getSinglePeakHeight(it, il, ixy);
                Float_t singleChi2 = mRHICfHit -> getSingleFitChi2(it, il, ixy);
                Float_t multiChi2 = mRHICfHit -> getMultiFitChi2(it, il, ixy);

                mSimRHICfHit -> SetSingleHitNum(it, il, ixy, singleHitNum);
                mSimRHICfHit -> SetMaxPeakBin(it, il, ixy, maxPeakBin);
                mSimRHICfHit -> SetSingleHitPos(it, il, ixy, singleHitPos);
                mSimRHICfHit -> SetSinglePeakHeight(it, il, ixy, singlePeakHeight);
                mSimRHICfHit -> SetSingleFitChi2(it, il, ixy, singleChi2);
                mSimRHICfHit -> SetMultiFitChi2(it, il, ixy, multiChi2);

                for(int io=0; io<2; io++){
                Float_t multiHitPos = mRHICfHit -> getMultiHitPos(it, il, ixy, io);
                Float_t multiPeakRaw = mRHICfHit -> getMultiPeakRaw(it, il, ixy, io);
                Float_t multiPeakHeight = mRHICfHit -> getMultiPeakHeight(it, il, ixy, io);
                Float_t multiEnergySum = mRHICfHit -> getMultiEnergySum(it, il, ixy, io);
                    
                mSimRHICfHit -> SetMultiHitPos(it, il, ixy, io, multiHitPos);
                mSimRHICfHit -> SetMultiPeakRaw(it, il, ixy, io, multiPeakRaw);
                mSimRHICfHit -> SetMultiPeakHeight(it, il, ixy, io, multiPeakHeight);
                mSimRHICfHit -> SetMultiEnergySum(it, il, ixy, io, multiEnergySum);
                }
            }
        }
    }

    // RHICf Point
    for(unsigned int i=0; i<mRHICfColl->numberOfPoints(); i++){
        Int_t towerIdx = mRHICfColl -> pointCollection()[i] -> getTowerIdx();
        Int_t pid = mRHICfColl -> pointCollection()[i] -> getPID();
        Float_t posX = mRHICfColl -> pointCollection()[i] -> getPointPos(0);
        Float_t posY = mRHICfColl -> pointCollection()[i] -> getPointPos(1);
        Float_t photonE = mRHICfColl -> pointCollection()[i] -> getPointEnergy(0);
        Float_t hadronE = mRHICfColl -> pointCollection()[i] -> getPointEnergy(1);
        Float_t towerEAll = mRHICfColl -> pointCollection()[i] -> getTowerSumEnergy(0);
        Float_t towerEPart = mRHICfColl -> pointCollection()[i] -> getTowerSumEnergy(1);

        StRHICfSimRHICfPoint* simRHICfPoint = mSimDst -> GetSimRHICfPoint(i);
        simRHICfPoint -> SetTowerIdx(towerIdx);
        simRHICfPoint -> SetPID(pid);
        simRHICfPoint -> SetPointPos(posX, posY);
        simRHICfPoint -> SetPointEnergy(photonE, hadronE);
        simRHICfPoint -> SetTowerSumEnergy(towerEAll, towerEPart);
    }

    LOG_INFO << " StRHICfSimConvertor::SaveRecoData() -- RHICfPoint number: " << mRHICfColl->numberOfPoints() << endm;

    mOutSimDstTree -> Fill();
  
    return kStOk;
}

void StRHICfSimConvertor::InitRHICfGeometry()
{
    double mRHICfTowerBoundary[2][4][2]; // [TS, TL][bound square][x, y]
    double mRHICfTowerCenterPos[2]; // [TS, TL] y pos

    double tsDetSize = 2.; // [cm]
    double tlDetSize = 4.; // [cm]
    double detBoundCut = 0.0; // [cm]
    double distTStoTL = 4.74; // [cm]
    double detBeamCenter = 0.; // [cm]

    if(mRHICfRunType < rTLtype || mRHICfRunType > rTOPtype){
        LOG_ERROR << "StRHICfSimConvertor::InitRHICfGeometry() warning!!! RHICf run type is not setted!!!" << endm;
    }

    if(mRHICfRunType == rTLtype){detBeamCenter = -4.74;} // TL
    if(mRHICfRunType == rTStype){detBeamCenter = 0.;} // TS
    if(mRHICfRunType == rTOPtype){detBeamCenter = 2.16;} // TOP

    mRHICfTowerBoundary[0][0][0] = sqrt(2)*((tsDetSize - detBoundCut*2.)/2.); 
    mRHICfTowerBoundary[0][0][1] = 0.;
    mRHICfTowerBoundary[0][1][0] = 0.; 
    mRHICfTowerBoundary[0][1][1] = sqrt(2)*((tsDetSize - detBoundCut*2.)/2.); 
    mRHICfTowerBoundary[0][2][0] = -1.*sqrt(2)*((tsDetSize - detBoundCut*2.)/2.); 
    mRHICfTowerBoundary[0][2][1] = 0.; 
    mRHICfTowerBoundary[0][3][0] = 0.; 
    mRHICfTowerBoundary[0][3][1] = -1.*sqrt(2)*((tsDetSize - detBoundCut*2.)/2.); 

    mRHICfTowerBoundary[1][0][0] = sqrt(2)*((tlDetSize - detBoundCut*2.)/2.);
    mRHICfTowerBoundary[1][0][1] = 0.;
    mRHICfTowerBoundary[1][1][0] = 0.;
    mRHICfTowerBoundary[1][1][1] = sqrt(2)*((tlDetSize - detBoundCut*2.)/2.);
    mRHICfTowerBoundary[1][2][0] = -1.*sqrt(2)*((tlDetSize - detBoundCut*2.)/2.);
    mRHICfTowerBoundary[1][2][1] = 0.;
    mRHICfTowerBoundary[1][3][0] = 0.;
    mRHICfTowerBoundary[1][3][1] = -1.*sqrt(2)*((tlDetSize - detBoundCut*2.)/2.);
    
    mRHICfTowerCenterPos[0] = detBeamCenter;
    mRHICfTowerCenterPos[1] = distTStoTL + detBeamCenter;

    mRHICfPoly = new TH2Poly();
    mRHICfPoly -> SetName("RHICfPoly");
    mRHICfPoly -> SetStats(0);

    double x[4];
    double y[4];

    for(int t=0; t<2; t++){
        for(int i=0; i<4; i++){
        double xPos = mRHICfTowerBoundary[t][i][0];
        double yPos = mRHICfTowerCenterPos[t] + mRHICfTowerBoundary[t][i][1];
        x[i] = xPos;
        y[i] = yPos;
        }
        mRHICfPoly -> AddBin(4, x, y);
    }
}

Int_t StRHICfSimConvertor::GetRHICfGeoHit(double posX, double posY, double posZ, double px, double py, double pz, double e)
{
  if(e < 10.){return -1;} // energy cut

  double momMag = sqrt(px*px + py*py + pz*pz);
  double unitVecX = px/momMag;
  double unitVecY = py/momMag;
  double unitVecZ = pz/momMag;

  if(unitVecZ < 0){return -1;} // opposite side cut

  double mRHICfDetZ = 1780.; // [cm]
  double z = mRHICfDetZ - posZ;
  if(z < 0.){return -1;} // create z-position cut

  double x = z * (unitVecX/unitVecZ) + posX;
  double y = z * (unitVecY/unitVecZ) + posY;

  int type = mRHICfPoly -> FindBin(x, y);
  if(type < 1 || type > 2){return -1;} // RHICf geometrical hit cut

  return type;
}

ClassImp(StRHICfSimConvertor)