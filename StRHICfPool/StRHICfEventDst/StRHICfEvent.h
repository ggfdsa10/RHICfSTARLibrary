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
        void SetFillNumber(int num);
        void SetBunchNumber(int num);
        void SetSpinBit(int num);
        void SetBeamEnergy(float energy);

        void SetRHICfTrig(bool trig);
        void SetDiffractiveTrig(bool trig);
        void SetVPDMB30Trig(bool trig);
        void SetTPCTrig(bool trig);

        void SetRHICfRunNumber(unsigned int num);
        void SetRHICfEventNumber(unsigned int num);
        void SetRHICfTriggerNumber(unsigned int num);
        void SetRHICfRunType(int num);
        void SetRHICfL20(int tower, double val);
        void SetRHICfL90(int tower, double val);

        void SetTofMult(int num); // B-TOF multiplicity in DAQ level
        void SetBTofMult(int num); // B-TOF multiplicity in Hit level (StMuBTofHit)
        void SetRefMult(int num);
        void SetGRefMult(int num);

        void SetPrimaryTrkNum(int num);
        void SetGlobalTrkNum(int num);

        void SetPrimaryVertex(double vx, double vy, double vz);
        
        UInt_t GetRunNumber();
        UInt_t GetEventNumber();
        Int_t GetFillNumber();
        Int_t GetBunchNumber();
        Int_t GetSpinBit();
        Float_t GetBeamEnergy();

        Bool_t GetRHICfTrig();
        Bool_t GetDiffractiveTrig();
        Bool_t GetVPDMB30Trig();
        Bool_t GetTPCTrig();

        UInt_t GetRHICfRunNumber();
        UInt_t GetRHICfEventNumber();
        UInt_t GetRHICfTriggerNumber();
        Int_t GetRHICfRunType();
        Bool_t GetRHICfShowerTrig();
        Bool_t GetRHICfPi0Trig();
        Bool_t GetRHICfHighEMTrig();
        Double_t GetRHICfL20(int tower);
        Double_t GetRHICfL90(int tower);

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
        UShort_t mFillNumber;
        UShort_t mBunchNumber;
        Char_t mSpinBit;
        Float_t mBeamEnergy;

        // Trigger information in STAR DAQ
        Bool_t mRHICfTrig;
		Bool_t mDiffractiveTrig;
		Bool_t mVPDMB30Trig;
		Bool_t mTPCTrig;

        // RHICf event information in RHICf standalone DAQ
        UInt_t mRHICfRunNumber;
        UInt_t mRHICfEventNumber;
        UInt_t mRHICfTriggerNumber;
        Char_t mRHICfRunType;

        Double_t mRHICfL20[2];
        Double_t mRHICfL90[2];

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
