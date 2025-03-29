#include "StRHICfZDC.h"

ClassImp(StRHICfZDC)

StRHICfZDC::StRHICfZDC()
{
    Clear();
}

StRHICfZDC::~StRHICfZDC()
{
}

void StRHICfZDC::Clear(Option_t *option)
{
   fill_n(&mZdcPmt[0][0][0], kBeamSideNum*kZDCPmtNum*2, -1);
   fill_n(&mZdcSmdADC[0][0][0], kBeamSideNum*kSMDNum*2, -1);
   fill_n(&mZdcPmtSumTDC[0], kBeamSideNum, -1);
   fill_n(&mZdcPmtSumADC[0][0], kBeamSideNum*2, -1);
    mZdcVertexZ = -999.;
}

void StRHICfZDC::SetZDCPmtADC(int dir, int pmt, int adc){mZdcPmt[dir][pmt][0] = adc;}
void StRHICfZDC::SetZDCPmtTDC(int dir, int pmt, int tdc){mZdcPmt[dir][pmt][1] = tdc;}
void StRHICfZDC::SetZDCPmtAttenuatedSumADC(int dir, int num){mZdcPmtSumADC[dir][0] = num;}
void StRHICfZDC::SetZDCPmtUnAttenuatedSumADC(int dir, int num){mZdcPmtSumADC[dir][1] = num;}
void StRHICfZDC::SetZDCPmtSumTDC(int dir, int num){mZdcPmtSumTDC[dir] = num;}

void StRHICfZDC::SetZDCSmdADC(int dir, int axis, int smd, int adc){mZdcSmdADC[dir][axis][smd] = adc;}
void StRHICfZDC::SetZDCVtxZ(float val){mZdcVertexZ = val;}

Int_t StRHICfZDC::GetZDCPmtADC(int dir, int pmt){return mZdcPmt[dir][pmt][0];}
Int_t StRHICfZDC::GetZDCPmtTDC(int dir, int pmt){return mZdcPmt[dir][pmt][1];}

Int_t StRHICfZDC::GetZDCPmtSumADC(int dir)
{
    int sumADC = 0;
    for(int i=0; i<kZDCPmtNum; i++){
        sumADC += mZdcPmt[dir][i][0];
    }
    return sumADC;
}

Int_t StRHICfZDC::GetZDCPmtAttenuatedSumADC(int dir){return mZdcPmtSumADC[dir][0];}
Int_t StRHICfZDC::GetZDCPmtUnAttenuatedSumADC(int dir){return mZdcPmtSumADC[dir][1];}
Int_t StRHICfZDC::GetZDCPmtSumTDC(int dir){return mZdcPmtSumTDC[dir];}

Int_t StRHICfZDC::GetZDCSmdADC(int dir, int axis, int smd){return mZdcSmdADC[dir][axis][smd];}
Float_t StRHICfZDC::GetZDCVtxZ(){return mZdcVertexZ;}
