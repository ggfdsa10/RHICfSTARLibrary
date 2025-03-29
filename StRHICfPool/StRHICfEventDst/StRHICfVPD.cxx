#include "StRHICfVPD.h"

ClassImp(StRHICfVPD)

StRHICfVPD::StRHICfVPD()
{
    Clear();
}

StRHICfVPD::~StRHICfVPD()
{
}

void StRHICfVPD::Clear(Option_t *option)
{
   fill_n(&mVpd[0][0][0], kBeamSideNum*kVPDPmtNum*2, -1);
   fill_n(&mVpdHighThr[0][0][0], kBeamSideNum*kVPDPmtNum*2, -1);
   fill_n(&mVpdEarliestTDC[0], kBeamSideNum, -1);
   fill_n(&mVpdEarliestTDCHighthr[0], kBeamSideNum, -1);
    mVpdTimeDiff = -1;
}

void StRHICfVPD::SetVPDADC(int dir, int pmt, int adc){mVpd[dir][pmt][0] = adc;}
void StRHICfVPD::SetVPDTDC(int dir, int pmt, int tdc){mVpd[dir][pmt][1] = tdc;}
void StRHICfVPD::SetVPDHighThrADC(int dir, int pmt, int adc){mVpdHighThr[dir][pmt][0] = adc;}
void StRHICfVPD::SetVPDHighThrTDC(int dir, int pmt, int tdc){mVpdHighThr[dir][pmt][1] = tdc;}
void StRHICfVPD::SetEarliestTDC(int dir, int tdc){mVpdEarliestTDC[dir] = tdc;}
void StRHICfVPD::SetEarliestHighThrTDC(int dir, int tdc){mVpdEarliestTDCHighthr[dir] = tdc;}
void StRHICfVPD::SetVPDTimeDiff(int num){mVpdTimeDiff = num;}

Int_t StRHICfVPD::GetVPDADC(int dir, int pmt){return mVpd[dir][pmt][0];}
Int_t StRHICfVPD::GetVPDTDC(int dir, int pmt){return mVpd[dir][pmt][1];}
Int_t StRHICfVPD::GetVPDHighThrADC(int dir, int pmt){return mVpdHighThr[dir][pmt][0];}
Int_t StRHICfVPD::GetVPDHighThrTDC(int dir, int pmt){return mVpdHighThr[dir][pmt][1];}
Int_t StRHICfVPD::GetEarliestTDC(int dir){return mVpdEarliestTDC[dir];}
Int_t StRHICfVPD::GetEarliestHighThrTDC(int dir){return mVpdEarliestTDCHighthr[dir];}
Int_t StRHICfVPD::GetVPDTimeDiff(){return mVpdTimeDiff;}

Int_t StRHICfVPD::GetVPDSumADC(int dir)
{
    int sumADC = 0;
    for(int i=0; i<kVPDPmtNum; i++){
        sumADC += mVpd[dir][i][0];
    }
    return sumADC;
}