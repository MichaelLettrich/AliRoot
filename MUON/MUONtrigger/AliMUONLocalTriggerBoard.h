#ifndef ALIMUONLOCALTRIGGERBOARD_H
#define ALIMUONLOCALTRIGGERBOARD_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

/// \ingroup trigger 
/// \class AliMUONLocalTriggerBoard
/// \brief Implementation of local trigger board objects
///
//  Author Rachid Guernane (LPCCFd)

#include "AliMUONTriggerBoard.h"
#include "AliMpLocalBoard.h"

class AliMUONTriggerLut;

class AliMUONLocalTriggerBoard : public AliMUONTriggerBoard 
{
   public: 
       
      AliMUONLocalTriggerBoard();
      AliMUONLocalTriggerBoard(AliMpLocalBoard* mpLocalBoard);

      virtual ~AliMUONLocalTriggerBoard();
      
                       /// Return true if LUT is set
      Bool_t           HasLUT() const { return (fLUT != 0); }

                       /// Set LUT     
      void             SetLUT(AliMUONTriggerLut* lut) { fLUT = lut; }
                       /// Set Coinc 44 (0/1 = coinc34/coinc44)
      void             SetCoinc44(Int_t coinc44=0) { fCoinc44 = coinc44; }
    
      virtual void     SetbitM(Int_t strip, Int_t cathode, Int_t chamber);

      virtual void     Pattern(const Option_t *option = "X Y") const; // default option displays X then Y bp

      virtual void     Reset();
      virtual void     ResetResponse();


                       /// Return i-th Switch value
      virtual UShort_t GetSwitch(Int_t i) const {return (UShort_t)fMpLocalBoard->GetSwitch(i);}

                       /// Return Transverse connector
      virtual Bool_t   GetTC() const {return fMpLocalBoard->GetTC();}

                       /// Return Board number
      virtual Int_t    GetNumber() const { return fMpLocalBoard->GetId(); }
    
      /// Is notified (copy boards are not)
      virtual Bool_t    IsNotified() const { return fMpLocalBoard->IsNotified(); }

                       /// Return Crate name
      virtual TString  GetCrate() const {return fMpLocalBoard->GetCrate();}

                       /// Return XY
      virtual void     GetXY(UShort_t XY[2][4]) const {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) XY[i][j] = fXY[i][j];}

                       /// Return XY
      virtual UShort_t GetXY(Int_t i, Int_t j) const {return fXY[i][j];} 

                       /// Set XY
      virtual void     SetXY(UShort_t XY[2][4]) {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) fXY[i][j] = XY[i][j];}

      virtual void     Conf() const;

      virtual void     Response();

      virtual void     Mask(Int_t index, UShort_t value);

      virtual void     TrigX(Int_t ch1q[16], Int_t ch2q[16], Int_t ch3q[32], Int_t ch4q[32]);
      
      virtual void     Sort2x5(Int_t dev1[6], Int_t dev2[6],
                               Int_t minDev[6], Int_t &dev1GTdev2);
      
      virtual void     TrigY(Int_t y1[16], Int_t y2[16], Int_t y3[16], Int_t y4[16],
                             Int_t y3u[16], Int_t y3d[16], Int_t y4u[16], Int_t y4d[16]);

                       /// Set XYU
      virtual void     SetXYU(UShort_t V[2][4]) {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) fXYU[i][j] = V[i][j];}

                       /// Return XYU
      virtual void     GetXYU(UShort_t V[2][4]) const {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) V[i][j] = fXYU[i][j];}

                       /// Set XYD
      virtual void     SetXYD(UShort_t V[2][4]) {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) fXYD[i][j] = V[i][j];}

                       /// Return XYD
      virtual void     GetXYD(UShort_t V[2][4]) const {for (Int_t i=0;i<2;i++) for (Int_t j=0;j<4;j++) V[i][j] = fXYD[i][j];}

      virtual void     Scan(Option_t *option = "") const;

      virtual Int_t    GetI() const;

      virtual void     LocalTrigger();

                       /// Return info if Board has triggered
      virtual Int_t    Triggered() const {return fOutput;}

                       /// Return MT1 X position of the valid road
      virtual Int_t    GetStripX11() const {return fStripX11;}

                       /// Return MT1 Y position of the valid road
      virtual Int_t    GetStripY11() const {return fStripY11;}

                       /// Return Deviation 
      virtual Int_t    GetDev() const {return fDev;}

                       /// Return Sign of Deviation 
      virtual Int_t    GetSdev() const {return fMinDev[4];}
      
                       /// Return Trigger in Y
      virtual Int_t    GetTrigY() const {return fTrigY;}

      virtual Bool_t   IsTrigY() const;

      virtual Bool_t   IsTrigX() const;

      AliMUONLocalTriggerBoard(const AliMUONLocalTriggerBoard& right);
      AliMUONLocalTriggerBoard&  operator = (const AliMUONLocalTriggerBoard& right);

      virtual Bool_t IsNull() const;
      
   protected:

      static const Int_t fgkCircuitId[234]; ///< old numbering (to be removed)

      virtual void     Resp(Option_t *option) const; ///< local trigger info before ("I") and after ("F") LUT

      virtual void     BP(const Option_t *option) const;   ///< display X/Y bp

   private:
      AliMpLocalBoard*  fMpLocalBoard; ///< pointer to the local board mapping 

      UShort_t fXY[2][4];         ///< Bit pattern
      UShort_t fXYU[2][4];        ///< Bit pattern UP
      UShort_t fXYD[2][4];        ///< Bit pattern DOWN
      UShort_t fMask[2][4];       ///< Mask

      Int_t    fStripX11;         ///< MT1 X position of the valid road 

      Int_t    fStripY11;         ///< MT1 Y position of the valid road

      Int_t    fDev;              ///< X deviation as in table 3-1 of Local Trigger Board PRR
      Int_t    fTrigY;            ///< Trigger in Y
      
      Int_t    fLutLpt[2];        ///< Low Pt cuts estimated from LUT
      Int_t    fLutHpt[2];        ///< High Pt cuts estimated from LUT

//    Outputs of the local logic
      Int_t    fOutput;           ///< Board has triggered
      Int_t    fMinDevStrip[5];   ///< X (from algo)
      Int_t    fMinDev[5];        ///< Dev (from algo)
      Int_t    fCoordY[5];        ///< Y (from algo)
      
      AliMUONTriggerLut *fLUT;    //!<! Pointer to trigger LUT, that we do not own.
      Int_t    fCoinc44;          ///< Coinc 44 (0/1 = coinc34/coinc44)
      
      ClassDef(AliMUONLocalTriggerBoard,2) //Implementation of local trigger board objects
};
#endif
