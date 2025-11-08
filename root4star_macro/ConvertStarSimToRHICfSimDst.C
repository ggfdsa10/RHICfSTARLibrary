Int_t CheckIntputFile(TString input);

void ConvertStarSimToRHICfSimDst(TString inputFile = "/star/u/slee5/macros/simulation/StarSim_Pythia8_TL_HitMult1_25092242046_.MuDst.root")
{
    gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");

    gSystem->Load( "libVMC.so");
    gSystem->Load( "St_g2t.so" );
    gSystem->Load( "St_geant_Maker.so" );
    gSystem->Load( "StarGeneratorUtil.so" );
    gSystem->Load( "StarGeneratorEvent.so" );
    gSystem->Load( "StarGeneratorBase.so" );

    TString localPath = "/star/u/slee5/.sl73_gcc485/lib/";
    gSystem->Load( localPath+"StRHICfSimDst.so" ); // for job
    gSystem->Load( localPath+"StRHICfSimConvertor.so" ); // for job

    inputFile = CheckIntputFile(inputFile);

    StChain* chain = new StChain();
    StMuDstMaker* MuDstMaker = new StMuDstMaker(0, 0, "", inputFile, "MuDst", 100);
    MuDstMaker->SetStatus("*", 1);

    StRHICfSimConvertor* simConvertor = new StRHICfSimConvertor(0, inputFile);

    chain->Init();
    Int_t totalEvents = MuDstMaker->chain()->GetEntries();

    cout << " Total Events: " << totalEvents << endl;
    chain->EventLoop(0, totalEvents);
    chain->Finish();
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

                if(dirName.Index("MuDst.root") != -1 && dirName.Index(input) != -1){
                    return dirName;
                }
            }
        }
        return "";
    }
    return input;
}