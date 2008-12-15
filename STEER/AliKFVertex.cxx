//----------------------------------------------------------------------------
// Implementation of the AliKFVertex class
// .
// @author  S.Gorbunov, I.Kisel
// @version 1.0
// @since   13.05.07
// 
// Class to reconstruct and store primary and secondary vertices
// The method is described in CBM-SOFT note 2007-003, 
// ``Reconstruction of decayed particles based on the Kalman filter'', 
// http://www.gsi.de/documents/DOC-2007-May-14-1.pdf
//
// This class is ALICE interface to general mathematics in AliKFParticleCore
// 
//  -= Copyright &copy ALICE HLT Group =-
//____________________________________________________________________________


#include "AliKFVertex.h"
#include "Riostream.h"

ClassImp(AliKFVertex)


AliKFVertex::AliKFVertex( const AliVVertex &vertex ): fIsConstrained(0)
{
  // Constructor from ALICE VVertex

  vertex.GetXYZ( fP );
  vertex.GetCovarianceMatrix( fC );  
  fChi2 = vertex.GetChi2();  
  fNDF = 2*vertex.GetNContributors() - 3;
  fQ = 0;
  fAtProductionVertex = 0;
  fIsLinearized = 0;
  fSFromDecay = 0;
}

/*
void     AliKFVertex::Print(Option_t* ) const
{  
  cout<<"AliKFVertex position:    "<<GetX()<<" "<<GetY()<<" "<<GetZ()<<endl;
  cout<<"AliKFVertex cov. matrix: "<<GetCovariance(0)<<endl;
  cout<<"                         "<<GetCovariance(1)<<" "<<GetCovariance(2)<<endl;
  cout<<"                         "<<GetCovariance(3)<<" "<<GetCovariance(4)<<" "<<GetCovariance(5)<<endl;
}
  */

void AliKFVertex::SetBeamConstraint( Double_t X, Double_t Y, Double_t Z, 
				      Double_t ErrX, Double_t ErrY, Double_t ErrZ )
{
  // Set beam constraint to the vertex
  fP[0] = X;
  fP[1] = Y;
  fP[2] = Z;
  fC[0] = ErrX*ErrX;
  fC[1] = 0;
  fC[2] = ErrY*ErrY;
  fC[3] = 0;
  fC[4] = 0;
  fC[5] = ErrZ*ErrZ;
  fIsConstrained = 1;
}

void AliKFVertex::SetBeamConstraintOff()
{
  fIsConstrained = 0;
}

void AliKFVertex::ConstructPrimaryVertex( const AliKFParticle *vDaughters[], 
					  int NDaughters, Bool_t vtxFlag[],
					  Double_t ChiCut  )
{
  //* Primary vertex finder with simple rejection of outliers
  if( NDaughters<2 ) return;
  Construct( vDaughters, NDaughters, 0, -1, fIsConstrained );
  for( int i=0; i<NDaughters; i++ ) vtxFlag[i] = 1;

  Int_t nRest = NDaughters;
  while( nRest>2 )
    {    
      Double_t worstChi = 0.;
      Int_t worstDaughter = 0;
      for( Int_t it=0; it<NDaughters; it++ ){
	if( !vtxFlag[it] ) continue;
	const AliKFParticle &p = *(vDaughters[it]);
	AliKFVertex tmp = *this - p;
	Double_t chi = p.GetDeviationFromVertex( tmp );      
	if( worstChi < chi ){
	  worstChi = chi;
	  worstDaughter = it;
	}
      }
      if( worstChi < ChiCut ) break;
      
      vtxFlag[worstDaughter] = 0;    
      *this -= *(vDaughters[worstDaughter]);
      nRest--;
    } 

  if( nRest<=2 && GetChi2()>ChiCut*ChiCut*GetNDF() ){
    for( int i=0; i<NDaughters; i++ ) vtxFlag[i] = 0;
    fNDF = -3;
    fChi2 = 0;
  }
}
