#include <ctime>

class StBFChain;        
class StMessMgr;

#pragma cling load("StarRoot")
#pragma cling load("St_base")
#pragma cling load("StChain")
#pragma cling load("StUtilities")
#pragma cling load("StBFChain")

class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *_primary = 0;

class RHICfFilter;
RHICfFilter *filter = 0;

class StarPythia8;
StarPythia8* _pythia8;

class StRHICfSimGenerator;
StRHICfSimGenerator* _RHICfSimGen;

#if !defined(__CINT__) || defined(__CLING__)
// 'chain' is defined in StBFChain library (see StRoot/StBFChain.cxx)
extern StBFChain* chain;
#else
StBFChain* chain = 0;
#endif

void command( TString cmd );

TString CheckIntputFile(TString input);
TString GetOutputFileName(int& modelIdx, TString& runType, TString fileID);

// generator function
void Pythia8Generate();
void RHICfModelsGenerate(TString genFile);

void StarSimulationGenerate(int generatorIdx = 0, TString rhicfRunType = "TOP", Int_t nevents = 10, TString fileID = "")
{ 
  TString fileName = GetOutputFileName(generatorIdx, rhicfRunType, fileID);

  gROOT->ProcessLine(".L bfc.C");
  {
    // additional BFChain and option library load 
    // TString fullSimOpt = "DbV20230620 sdt20170623 y2017a gstar usexgeom agml misalign ReverseField MakeEvent huge tables Tree cmudst McEvOut McEvent bbcSim btofSim vpdSim sti ittf tpcDb TpcFastSim genvtx"; // with TPC and vertex
    TString fullSimOpt = "DbV20230620 sdt20170623 y2017a gstar usexgeom agml misalign ReverseField MakeEvent huge tables Tree cmudst McEvOut McEvent bbcSim btofSim vpdSim"; // without TPC and vertex
    Load(fullSimOpt);

    TString localPath = "/star/u/slee5/.sl73_gcc485/lib/";
    gSystem->Load( localPath+"StBbcSimulationMaker.so" ); // for BBC modified maker

    // ======== StBFChain parts =========
    chain = new StBFChain();
    chain->cd();
    chain->SetDebug(1);
    chain->SetFlags(fullSimOpt);
    if (chain->Load() > kStOk) {
      gMessMgr->Error() << "Problems with loading of shared library(ies)" << endm;
      gSystem->Exit(1);
    }
    chain->Set_IO_Files(0,fileName);
    if (chain->Instantiate() > kStOk)  { 
      gMessMgr->Error() << "Problems with instantiation of Maker(s)" << endm;
      gSystem->Exit(1);
    }
    TAttr::SetDebug(0);
    chain->SetAttr(".Privilege",0,"*"                ); 	  //All  makers are NOT priviliged
    chain->SetAttr(".Privilege",1,"StIOInterFace::*" ); 	  //All IO makers are priviliged
    chain->SetAttr(".Privilege",1,"St_geant_Maker::*"); 	  //It is also IO maker
    chain->SetAttr(".Privilege",1,"StTpcDbMaker::*"); 	  //It is also TpcDb maker to catch trips
    chain->SetAttr(".Privilege",1,"*::tpc_hits"); //May be allowed to act upon excessive events
    chain->SetAttr(".Privilege",1,"*::tpx_hits"); //May be allowed to act upon excessive events
    chain->SetAttr(".Privilege",1,"StTpcHitMover::*"); //May be allowed to act upon corrupt events
    chain->SetAttr(".Privilege",1,"*::tpcChain"); //May pass on messages from sub-makers
    chain->SetAttr(".Privilege",1,"StTriggerDataMaker::*"); //TriggerData could reject event based on corrupt triggers
    chain->SetAttr(".Privilege",1,"StEventMaker::*"); //May be allowed to act upon trigger IDs (filtering)

    // ======== STARSim parts =========
    gSystem->Load( "libVMC.so");
    gSystem->Load( "StarGeneratorUtil.so");
    gSystem->Load( "StarGeneratorEvent.so");
    gSystem->Load( "StarGeneratorBase.so" );
    gSystem->Load( "StarGeneratorDecay.so" );
    gSystem->Load( "Pythia8_3_03.so"  );
    gSystem->Load( "libMathMore.so"   );  
    gSystem->Load("libKinematics.so");
    gSystem->Load("libHepMC2_06_09.so");

    // Force loading of xgeometry
    gSystem->Load( "xgeometry.so"     );
    // gSystem->Load( "/star/u/slee5/.sl73_gcc485/lib/StarGeneratorFilt.so" ); // for local run
    // gSystem->Load( "/star/u/slee5/.sl73_gcc485/lib/StRHICfSimDst.so" ); // for local run
    // gSystem->Include( "/star/u/slee5/.sl73_gcc485/obj/StRoot/StRHICfPool/StRHICfSimDst/StRHICfSimPar.h" ); // for local run
    gSystem->Load( localPath+"StarGeneratorFilt.so" ); // for job run
    gSystem->Load( localPath+"StRHICfSimDst.so" ); // for job run
    gSystem->Load( localPath+"StRHICfSimGenerator.so" ); // for job run

    Int_t iInit = chain->Init();
    if (iInit >=  kStEOF) {chain->FatalErr(iInit,"on init"); return;}
  }

  // Setup RNG seed and map all ROOT TRandom here
  StarRandom::capture();

  _primary = new StarPrimaryMaker();

  _primary -> SetFileName(Form("%s.genevents.root", fileName.Data()));
  chain -> AddBefore( "geant", _primary );

  // =============== Event Generator =================
  int runtype = -1;
  rhicfRunType.ToUpper();
  double beamCenterX = 0.; // [cm]
  double beamCenterY = 0.; // [cm]
  if(rhicfRunType == "TL"){
    runtype = 0;
    beamCenterX = 0.044;
    beamCenterY = 0.186;
  }
  else if(rhicfRunType == "TS"){
    runtype = 1;
    beamCenterX = 0.022;
    beamCenterY = 0.19;
  }
  else if(rhicfRunType == "TOP"){
    runtype = 2;
    beamCenterX = 0.022;
    beamCenterY = -0.053;
  }
  else{
    cout << "Error: No RHICf run type" << endl;
    return;
  }

  if(generatorIdx == -1){
    cout << "Event Generator: Non" << endl;
    return 0;
  }
  else if(generatorIdx == 0){
    cout << "Event Generator: STAR Pythia8 Detroit tune" << endl;
    Pythia8Generate();
  }
  else{
    RHICfModelsGenerate(fileID);
  }

  command("call gstar_part"); 
  // =================================================

  // =============== RHICf Filter =================
  filter = new RHICfFilter();
  filter -> SetRHICfRunType(runtype);
  _primary -> AddFilter( filter );
  // =============================================
  
  int isAllFeepFilter = 0;
  if(!_pythia8){isAllFeepFilter = 1;}
  _primary->SetAttr("FilterKeepAll", isAllFeepFilter);

  if(_pythia8){
    _primary->SetVertex( beamCenterX, beamCenterY, 0. ); // [cm]
    _primary->SetSigma( 0.02, 0.02, 30.0 ); // [cm]
  }
  else{ // for RHICf generator, Note: RHICfSimGenerator.root for CRMC has been applied same vertex fluctuation. Do not set the duplicated fluctuation
    _primary->SetVertex( 0., 0., 0. ); // [cm]
    _primary->SetSigma( 0., 0., 0. ); // [cm]
  }
  _primary -> Init();        

  command("gkine -4 0");

  if(_pythia8){
    _pythia8->Set("211:onMode=1"); // pi+/- stable 0
    _pythia8->Set("111:onMode=1"); // pi0 stable to permit mother/daughter in star record 1
    _pythia8->Set("221:onMode=1"); // eta stable 1
    _pythia8->Set("310:onMode=1"); // K short 1
    _pythia8->Set("130:onMode=1"); // K long 1
    _pythia8->Set("3122:onMode=1"); // Lambda 0  1
    _pythia8->Set("321:onMode=1"); // K+/- stable 1
    _pythia8->Set("3312:onMode=1"); // Xi - 1
    _pythia8->Set("3322:onMode=1"); // Xi 0 1
    _pythia8->Set("3334:onMode=1"); // Omega -
    _pythia8->Set("3212:onMode=1"); // Sigma 0
    _pythia8->Set("3112:onMode=1"); // Sigma -
    _pythia8->Set("3222:onMode=1"); // Sigma + 
  }    

  // ============ Processing ============
  int totalEventNum = -1;
  if(_RHICfSimGen && totalEventNum == -1){totalEventNum = _RHICfSimGen -> GetTotalEventNumber();}
  if(totalEventNum != -1){nevents = totalEventNum;}
  cout << "totalEventNum " << totalEventNum << endl;
  chain->EventLoop(nevents);
  _primary->event()->Print();

  chain->Finish();

  // EXIT starsim
  command("call agexit");  // Make sure that STARSIM exits properly
}

void command( TString cmd )
{
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> Do( cmd );
}

void Pythia8Generate()
{
  StarPythia8 *pythia8 = new StarPythia8();    
  _pythia8 = pythia8;

  pythia8->SetFrame("CMS", 510.0);
  pythia8->SetBlue("proton");
  pythia8->SetYell("proton");    

  pythia8->Set("SoftQCD:inelastic = on");
  pythia8->Set("Main:numberOfEvents = 1000000000");

  _primary -> AddGenerator( pythia8 );  
}

void RHICfModelsGenerate(TString genFile)
{
  TString RHICfSimGenFile = CheckIntputFile(genFile);
  _RHICfSimGen = new StRHICfSimGenerator();
  _RHICfSimGen -> SetGenFile(RHICfSimGenFile);
  _primary -> AddGenerator( _RHICfSimGen );  
}

TString GetOutputFileName(int& modelIdx, TString& runType, TString fileID)
{
  TString generatorName[6] = {"Pythia8", "QGSJETIII01" , "SIBYLL", "EPOSLHCR", "EPOSLHCR_FAST", "QGSJETII04"};

  if(modelIdx == -1){
    TString RHICfSimGenFile = CheckIntputFile(fileID);
    TFile* file = new TFile(RHICfSimGenFile, "read");
    if(!file){cout << "No RHICfSimGenerator file..." << endl; return "";}

    TTree* runTree = (TTree*)file->Get("Run");
    int runTypeIdx = -1;
    int modelTypeIdx = -1;
    runTree -> SetBranchAddress("RHICfRunType", &runTypeIdx);
    runTree -> SetBranchAddress("ModelType", &modelTypeIdx);
    runTree -> GetEntry(0);

    modelIdx = modelTypeIdx;
    if(runTypeIdx == 0){runType = "TL";}
    if(runTypeIdx == 1){runType = "TS";}
    if(runTypeIdx == 2){runType = "TOP";}
    file->Close();
  }

  TString jobTime = "";
  time_t timer;
  timer = time(NULL); 
  struct tm* t = localtime(&timer); 
  int date = (t->tm_year -100)*10000 + (t->tm_mon+1)*100 + t->tm_mday;
  int time = t->tm_hour*10000 + t->tm_min*100 + t->tm_sec;
  jobTime = Form("%i%i", date, time);

  TString fileName = "StarSim_" + generatorName[modelIdx] + "_" + runType + "_" + jobTime + "_" + fileID;
  return fileName;
}

TString CheckIntputFile(TString input)
{
  if(input.Index(".list") != -1 || input.Index(".lis") != -1 || input.Index(".txt") != -1 || input.Index(".root") != -1){
    return input; // input exist for MuDst
  }
  else if(input.Index(".") == -1){
    // Find a MuDst in current directory
    TString currentPath = gSystem -> pwd();
    TSystemDirectory dir("dir", currentPath);

    TList *listOfDirs = dir.GetListOfFiles();
    TObject *objDir;
    TIter next(listOfDirs);
    while((objDir = next())){
      TSystemFile* dirPtr = dynamic_cast<TSystemFile*>(objDir);
      if(dirPtr && !dirPtr->IsDirectory()){
        TString dirName = dirPtr->GetName();

        if(dirName.Index("RHICfSimGenerator.root") != -1 && dirName.Index(input) != -1){
          if(currentPath[currentPath.Sizeof()-1] != "/"){currentPath = currentPath +"/";}
          return currentPath+dirName;
        }
      }
    }
    return "";
  }
  return input;
}