#ifndef StRHICfVPD_HH
#define StRHICfVPD_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfVPD : public TObject
{
    public: 
        StRHICfVPD();
        ~StRHICfVPD();

        void Clear(Option_t *option = "");

        void SetVPDADC(int dir, int pmt, int adc);
        void SetVPDTDC(int dir, int pmt, int tdc);
        void SetVPDHighThrADC(int dir, int pmt, int adc);
        void SetVPDHighThrTDC(int dir, int pmt, int tdc);
        void SetEarliestTDC(int dir, int tdc);
        void SetEarliestHighThrTDC(int dir, int tdc);
        void SetVPDTimeDiff(int num);

        Int_t GetVPDADC(int dir, int pmt);
        Int_t GetVPDTDC(int dir, int pmt);
        Int_t GetVPDHighThrADC(int dir, int pmt);
        Int_t GetVPDHighThrTDC(int dir, int pmt);
        Int_t GetEarliestTDC(int dir);
        Int_t GetEarliestHighThrTDC(int dir);
        Int_t GetVPDTimeDiff();

        Int_t GetVPDSumADC(int dir);

    private:
        Short_t mVpd[kBeamSideNum][kVPDPmtNum][2]; // [east, west][pmt idx][adc, tdc] 
        Short_t mVpdHighThr[kBeamSideNum][kVPDPmtNum][2]; // Hight Threshold [east, west][pmt idx][adc, tdc] 
        Short_t mVpdEarliestTDC[kBeamSideNum]; // [East, West]
        Short_t mVpdEarliestTDCHighthr[kBeamSideNum]; // [East, West]
        Short_t mVpdTimeDiff; // vpdTimeDifference


    ClassDef(StRHICfVPD,1)
};

#endif
