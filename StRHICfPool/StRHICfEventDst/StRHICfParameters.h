#ifndef StRHICfParameters_HH
#define StRHICfParameters_HH

enum RHICfRunTypePar{
    kRHICfTLtype = 0,
    kRHICfTStype = 1,
    kRHICfTOPtype = 2
};

enum BeamPar{
    kBeamEast = 0,
    kBeamWest = 1,
    kBeamSideNum = 2
};

enum BTofPar{
    kBTofTrayNum = 120,
    kBTofModuleNum = 32,
    kBTofCellnum = 6
};

enum TPCPar{ 
    kElectron = 0,
    kPositron = 1,
    kPlusPion = 2,
    kMinusPion = 3,
    kPlusKaon = 4,
    kMinusKaon = 5,
    kProton = 6,
    kAntiProton = 7
};

enum BBCPar{
    kBBCLargePmtNum = 8,
    kBBCSmallPmtNum = 16
};

enum VPDPar{
    kVPDEastTray = 121,
    kVPDWestTray = 122,
    kVPDCellNum = 19,
    kVPDPmtNum = 16
};

enum RHICfDetPar{
    kTS = 0,
    kTL = 1,
    kTowerNum = 2,
    kPlateNum = 16,
    kLayerNum = 4,
    kXYNum = 2,
    kSmallBarNum = 20,
    kLargeBarNum = 40
};

enum RHICfPar{
    kPi0 = 0,
    kNeutron = 1
};

enum ZDCPar{
    kZDCPmtNum = 3,
    kSMDNum = 7
};

enum RPSPar{
    kRPSPointNum = 2
};


#endif