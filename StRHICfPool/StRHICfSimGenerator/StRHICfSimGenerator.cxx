#include "StRHICfSimGenerator.h"
ClassImp(StRHICfSimGenerator);

#include "StarGenerator/UTIL/StarParticleData.h"
#include "TParticlePDG.h"

#include "StarGenerator/UTIL/StarRandom.h" 
#include "StarGenerator/EVENT/StarGenEvent.h"
#include "StarGenerator/EVENT/StarGenPPEvent.h"

StRHICfSimGenerator::StRHICfSimGenerator(const char *name) : StarGenerator(name)
{
    mFileName = "";
    mGeneratorId = -1;
    mRHICfRunType = -1;
    mSetEventNumber = -1;
    mTotalEventNumber = 0;
    mEventIdx = 0;

    SetBlue("proton");
    SetYell("proton");  
}

int StRHICfSimGenerator::Init()
{
    mEvent = new StarGenPPEvent();
    if( mBlue == "proton"  && mYell == "proton") {LOG_INFO << "pp mode detected" << endm;} 
    else{LOG_INFO << "AA (or eA) mode detected... event record will still be a pp event record." << endm;}

    if(mGeneratorId > rQGSJETIII || mGeneratorId < rHerwig7){
        LOG_ERROR << "StRHICfSimGenerator::Init() -- Invalid generator id: " << mGeneratorId << endm;
        return kStFatal;
    }
    if(mRHICfRunType < rTLtype || mRHICfRunType > rTOPtype){
        LOG_ERROR << "StRHICfSimGenerator::Init() -- Invalid RHICf run type: " << mGeneratorId << endm;
        return kStFatal;
    }

    InitTree();

    return StMaker::Init();
}

int StRHICfSimGenerator::Generate()
{ 
    int eventNum = mEvent -> GetEventNumber();
    mGenTree -> GetEntry(eventNum);

    FillPP( mEvent );

    mNumberOfParticles = mParticleArr -> GetEntries();

    LOG_INFO << "StRHICfSimGenerator::Generate() -- event: " << eventNum << ", number of particles: " << mNumberOfParticles << endl;

    for(int idx=0; idx < mNumberOfParticles; idx++){
        mParticle = (TParticle*)mParticleArr -> At(idx);
        int id        = mParticle -> GetPdgCode();
        int stat      = mParticle -> GetStatusCode();
        int mother1   = mParticle -> GetFirstMother();
        int mother2   = mParticle -> GetSecondMother();
        int daughter1 = mParticle -> GetFirstDaughter();
        int daughter2 = mParticle -> GetLastDaughter();
        double px     = mParticle -> Px();    // [GeV/c]
        double py     = mParticle -> Py();    // [GeV/c]
        double pz     = mParticle -> Pz();    // [GeV/c]
        double energy = mParticle -> Energy();// [GeV]
        double mass   = mParticle -> GetCalcMass();
        double vx     = mParticle -> Vx(); // [mm]
        double vy     = mParticle -> Vy(); // [mm]
        double vz     = mParticle -> Vz(); // [mm]
        double vt     = mParticle -> T();  // [mm/c]

        mEvent -> AddParticle( stat, id, mother1, mother2, daughter1, daughter2, px, py, pz, energy, mass, vx, vy, vz, vt );
    }

  return kStOK;
}

void StRHICfSimGenerator::SetGenFile(TString file){mFileName = file;}
void StRHICfSimGenerator::SetGeneratorModel(int generatorId){mGeneratorId = generatorId;}
void StRHICfSimGenerator::SetRHICfRunType(int runtype){mRHICfRunType = runtype;}

int StRHICfSimGenerator::InitTree()
{
    if(mFileName.Index(".lis") != -1 || mFileName.Index(".list") != -1 || mFileName.Index(".txt") != -1){
        int fileNum = 0;
        std::ifstream inputStream( mFileName.Data() );
        if(!inputStream) {LOG_ERROR << "ERROR: Cannot open list file " << mFileName << endm;}

        std::string file;
        size_t pos;
        while(getline(inputStream, file)){
            pos = file.find_first_of(" ");
            if (pos != std::string::npos ) file.erase(pos,file.length()-pos);
            if(file.find("RHICfSimGen.root") != std::string::npos){
                mFileName = file;
                fileNum++;
            }
        }
        if(fileNum != 1){
            LOG_ERROR << "StRHICfSimGenerator::InitTree() -- Input file must be one RHICfSimGen.root file" << endm;
            return kStFatal;
        }
    }
    if(mFileName.Index("RHICfSimGen.root") == -1){
        LOG_ERROR << "Input generator file don't match the RHICfSimGen.root !!!" << endm;
        return kStFatal;
    }

    TString generatorName = StRHICfSimPar::GetGeneratorName(mGeneratorId);
    if(mFileName.Index(generatorName) == -1){
        LOG_ERROR << "Input generator is not " << generatorName << endm;
        return kStFatal;
    }
    else{
        LOG_INFO << generatorName << " generator file processing... " << endm;
    }

    if(mRHICfRunType == rTLtype){
        if(mFileName.Index("TL") == -1){
            LOG_ERROR << "Input generator is not RHICf TL run type file !!!" << endm;
            return kStFatal;
        }
    }
    if(mRHICfRunType == rTStype){
        if(mFileName.Index("TS") == -1){
            LOG_ERROR << "Input generator is not RHICf TS run type file !!!" << endm;
            return kStFatal;
        }
    }
    if(mRHICfRunType == rTOPtype){
        if(mFileName.Index("TOP") == -1){
            LOG_ERROR << "Input generator is not RHICf TOP run type file !!!" << endm;
            return kStFatal;
        }
    }

    mGenFile = new TFile(mFileName, "read");
    mGenTree = (TTree*)mGenFile -> Get("Event");

    mParticleArr = new TClonesArray("TParticle");
    mGenTree -> SetBranchAddress("particle",&mParticleArr);
    mGenTree -> SetBranchAddress("process", &mEventProcess);
    mGenTree -> SetBranchAddress("sHat", &mEvent_sHat);
    mGenTree -> SetBranchAddress("tHat", &mEvent_tHat);
    mGenTree -> SetBranchAddress("uHat", &mEvent_uHat);

    mTotalEventNumber = mGenTree -> GetEntries();

    if(mSetEventNumber != -1){
        if(mSetEventNumber > mTotalEventNumber){
            LOG_INFO << "SetTotalEventNumber(" << mSetEventNumber << ") > Input file entries. Set the total event number to " << mTotalEventNumber << endm;
            mSetEventNumber = mTotalEventNumber;
        }
        mTotalEventNumber = mSetEventNumber;
    }    

    return kStOk;
}

void StRHICfSimGenerator::SetTotalEventNumber(int num){mSetEventNumber = num;}
int StRHICfSimGenerator::GetTotalEventNumber(){return mTotalEventNumber;}

void StRHICfSimGenerator::FillPP( StarGenEvent *myevent )
{
  StarGenPPEvent *event = (StarGenPPEvent *)myevent;

    event -> idBlue     = 2212;     // Proton Beam
    event -> idYell     = 2212;     // Proton Beam
    event -> process    = mEventProcess;
    event -> subprocess = -999;

    event -> idParton1  = -999;
    event -> idParton2  = -999;
    event -> xParton1   = -999;
    event -> xParton2   = -999;
    event -> xPdf1      = -999;
    event -> xPdf2      = -999;
    event -> Q2fac      = -999;        // factorization scale
    event -> Q2ren      = -999;        // renormalization scale
    event -> valence1   = -999;
    event -> valence2   = -999;
    
    event -> sHat       = mEvent_sHat;
    event -> tHat       = mEvent_tHat;
    event -> uHat       = mEvent_uHat;
    event -> ptHat      = -999;
    event -> thetaHat   = -999;
    event -> phiHat     = -999;

    event -> weight     = -999;
}

StarGenStats StRHICfSimGenerator::Stats()
{
    StarGenStats stats( GetName(), "StRHICfSimGenerator Run Statistics" );

    if(!mGenTree){return stats;}
    stats.nTried         = mGenTree->GetEntries();
    stats.nSelected      = mTotalEventNumber;
    stats.nAccepted      = mTotalEventNumber;
    stats.sigmaGen       = -1;
    stats.sigmaErr       = -1;
    stats.sumWeightGen   = -1;

    stats.nFilterSeen    = stats.nAccepted;
    stats.nFilterAccept  = stats.nAccepted;

    stats.Dump();

    // Return a copy of the class we just created
    return stats;
}
