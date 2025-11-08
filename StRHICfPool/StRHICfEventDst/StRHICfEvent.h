#ifndef StRHICfEvent_HH
#define StRHICfEvent_HH

#include "TObject.h"

class StRHICfEvent : public TObject
{
    public: 
        StRHICfEvent();
        ~StRHICfEvent();

        void Clear(Option_t *option = "");

        void SetRunNumber(unsigned int num);
        void SetEventNumber(unsigned int num);
        void SetEventTime(unsigned int time);
        void SetFillNumber(int num);
        void SetBunchNumber(int num);
        void SetSpinBit(int num);
        void SetBeamEnergy(float energy);

        void SetRHICfTrig(bool trig){mTriggerFlag[0] = trig;}
        void SetRHICfDiffractiveTrig(bool trig){mTriggerFlag[1] = trig;}
        void SetRHICfVPDMB30Trig(bool trig){mTriggerFlag[2] = trig;}
        void SetRHICfTPCTrig(bool trig){mTriggerFlag[3] = trig;}
        void SetRPSDTTrig(bool trig){mTriggerFlag[4] = trig;}
        void SetRPETTrig(bool trig){mTriggerFlag[5] = trig;}
        void SetRPCPTnoBBCLTrig(bool trig){mTriggerFlag[6] = trig;}
        void SetRPCPT2Trig(bool trig){mTriggerFlag[7] = trig;}
        void SetRPCPT2noBBCLTrig(bool trig){mTriggerFlag[8] = trig;}
        void SetVPDMB30Trig(bool trig){mTriggerFlag[9] = trig;}
        void SetVPDMB100Trig(bool trig){mTriggerFlag[10] = trig;}
        void SetBHT3Trig(bool trig){mTriggerFlag[11] = trig;}
        void SetTofHighMultTrig(bool trig){mTriggerFlag[12] = trig;}
        void SetEPDTrig(bool trig){mTriggerFlag[13] = trig;}
        void SetBBCTrig(bool trig){mTriggerFlag[14] = trig;}
        void SetBBCTacTrig(bool trig){mTriggerFlag[15] = trig;}
        void SetZDCTrig(bool trig){mTriggerFlag[16] = trig;}
        void SetZDCTacTrig(bool trig){mTriggerFlag[17] = trig;}
        void SetVPDTrig(bool trig){mTriggerFlag[18] = trig;}
        void SetZeroBiasTrig(bool trig){mTriggerFlag[19] = trig;}

        void SetRHICfRunNumber(unsigned int num);
        void SetRHICfEventNumber(unsigned int num);
        void SetRHICfTriggerNumber(unsigned int num);
        void SetRHICfRunType(int num);

        void SetTofMult(int num); // B-TOF multiplicity in DAQ level
        void SetBTofMult(int num); // B-TOF multiplicity in Hit level (StMuBTofHit)
        void SetRefMult(int num);
        void SetGRefMult(int num);

        void SetPrimaryTrkNum(int num);
        void SetGlobalTrkNum(int num);

        void SetPrimaryVertex(double vx, double vy, double vz);
        
        UInt_t GetRunNumber();
        UInt_t GetEventNumber();
        UInt_t GetEventTime();
        Int_t GetFillNumber();
        Int_t GetBunchNumber();
        Int_t GetSpinBit();
        Float_t GetBeamEnergy();

        Bool_t GetRHICfTrig(){return mTriggerFlag[0];}
        Bool_t GetRHICfDiffractiveTrig(){return mTriggerFlag[1];}
        Bool_t GetRHICfVPDMB30Trig(){return mTriggerFlag[2];}
        Bool_t GetRHICfTPCTrig(){return mTriggerFlag[3];}
        Bool_t GetRPSDTTrig(){return mTriggerFlag[4];}
        Bool_t GetRPETTrig(){return mTriggerFlag[5];}
        Bool_t GetRPCPTnoBBCLTrig(){return mTriggerFlag[6];}
        Bool_t GetRPCPT2Trig(){return mTriggerFlag[7];}
        Bool_t GetRPCPT2noBBCLTrig(){return mTriggerFlag[8];}
        Bool_t GetVPDMB30Trig(){return mTriggerFlag[9];}
        Bool_t GetVPDMB100Trig(){return mTriggerFlag[10];}
        Bool_t GetBHT3Trig(){return mTriggerFlag[11];}
        Bool_t GetTofHighMultTrig(){return mTriggerFlag[12];}
        Bool_t GetEPDTrig(){return mTriggerFlag[13];}
        Bool_t GetBBCTrig(){return mTriggerFlag[14];}
        Bool_t GetBBCTacTrig(){return mTriggerFlag[15];}
        Bool_t GetZDCTrig(){return mTriggerFlag[16];}
        Bool_t GetZDCTacTrig(){return mTriggerFlag[17];}
        Bool_t GetVPDTrig(){return mTriggerFlag[18];}
        Bool_t GetZeroBiasTrig(){return mTriggerFlag[19];}

        UInt_t GetRHICfRunNumber();
        UInt_t GetRHICfEventNumber();
        UInt_t GetRHICfTriggerNumber();
        Int_t GetRHICfRunType();
        Bool_t GetRHICfShowerTrig();
        Bool_t GetRHICfPi0Trig();
        Bool_t GetRHICfHighEMTrig();

        Int_t GetTofMult(); // B-TOF multiplicity in DAQ level
        Int_t GetBTofMult(); // B-TOF multiplicity in Hit level (StMuBTofHit)
        Int_t GetRefMult();
        Int_t GetGRefMult();

        Int_t GetPrimaryTrkNum();
        Int_t GetGlobalTrkNum();

        Double_t GetPrimaryVtxX();
        Double_t GetPrimaryVtxY();
        Double_t GetPrimaryVtxZ();
        
    private:
        // general event information
        UInt_t mRunNumber;
        UInt_t mEventNumber;
        UInt_t mEventTime;
        UShort_t mFillNumber;
        UShort_t mBunchNumber;
        Char_t mSpinBit;
        Float_t mBeamEnergy;

        /* 
        // Trigger Flag array information
        // mTriggerFlag[0] = RHICf trigger
        // mTriggerFlag[1] = RHICf diffractivetrigger
        // mTriggerFlag[2] = RHICf VPDMB-30 trigger
        // mTriggerFlag[3] = RHICf tpc trigger
        // mTriggerFlag[4] = RP_SDT trigger
        // mTriggerFlag[5] = RP_ET trigger
        // mTriggerFlag[6] = RP_CPTnoBBCL trigger
        // mTriggerFlag[7] = RP_CPT2 trigger
        // mTriggerFlag[8] = RP_CPT2noBBCL trigger
        // mTriggerFlag[9] = VPDMB-30 trigger
        // mTriggerFlag[10] = VPDMB-100 trigger
        // mTriggerFlag[11] = BHT3 trigger
        // mTriggerFlag[12] = TofHighMult trigger
        // mTriggerFlag[13] = EPD trigger
        // mTriggerFlag[14] = BBC trigger
        // mTriggerFlag[15] = BBC-tac trigger
        // mTriggerFlag[16] = ZDC trigger
        // mTriggerFlag[17] = ZDC-tac trigger
        // mTriggerFlag[18] = VPD trigger
        // mTriggerFlag[19] = ZEROBIAS trigger
        */
        Bool_t mTriggerFlag[20]; 

        // RHICf event information in RHICf standalone DAQ
        UInt_t mRHICfRunNumber;
        UInt_t mRHICfEventNumber;
        UInt_t mRHICfTriggerNumber;
        Char_t mRHICfRunType;

        // Multiplicity
        Short_t mTofMult;  // B-TOF multiplicity in DAQ level
        Short_t mBTofMult; // B-TOF multiplicity in Hit level (StMuBTofHit)
        Short_t mRefMult;
        Short_t mGRefMult;

        Short_t mPrimaryTrkNum;
        Short_t mGlobalTrkNum;

        Double_t mPrimaryVtx[3]; // [cm]

    ClassDef(StRHICfEvent,1)
};

#endif
