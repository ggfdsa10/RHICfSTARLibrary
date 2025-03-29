#ifndef StRHICfZDC_HH
#define StRHICfZDC_HH

#include "TObject.h"
#include "StRHICfParameters.h"

using namespace std;

class StRHICfZDC : public TObject
{
    public: 
        StRHICfZDC();
        ~StRHICfZDC();

        void Clear(Option_t *option = "");

        void SetZDCPmtADC(int dir, int pmt, int adc);
        void SetZDCPmtTDC(int dir, int pmt, int tdc);
        void SetZDCPmtAttenuatedSumADC(int dir, int num);
        void SetZDCPmtUnAttenuatedSumADC(int dir, int num);
        void SetZDCPmtSumTDC(int dir, int num);

        void SetZDCSmdADC(int dir, int axis, int smd, int adc);
        void SetZDCVtxZ(float val);

        Int_t GetZDCPmtADC(int dir, int pmt);
        Int_t GetZDCPmtTDC(int dir, int pmt);
        Int_t GetZDCPmtSumADC(int dir); // sum of ADC for pmt GetZDCPmtSumADC
        Int_t GetZDCPmtAttenuatedSumADC(int dir); // daq level sum of ADC
        Int_t GetZDCPmtUnAttenuatedSumADC(int dir); // daq level sum of ADC
        Int_t GetZDCPmtSumTDC(int dir); // daq level sum of ADC

        Int_t GetZDCSmdADC(int dir, int axis, int smd);
        Float_t GetZDCVtxZ();

    private:
        Short_t mZdcPmt[kBeamSideNum][kZDCPmtNum][2]; // [east, west][pmt][adc, tdc]
        Short_t mZdcPmtSumTDC[kBeamSideNum]; // [east, west] Note: daq level sumTDC
        Short_t mZdcPmtSumADC[kBeamSideNum][2]; // [east, west][Attenuated sumADC, Unattenuated sumADC] Note: daq level sumADC
        Short_t mZdcSmdADC[kBeamSideNum][2][kSMDNum]; // [east, west][vertical, horizontal][smd]
        Float_t mZdcVertexZ;

    ClassDef(StRHICfZDC,1)
};

#endif
