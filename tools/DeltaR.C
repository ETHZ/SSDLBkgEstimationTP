//Computes the delta R between two vectors. This is a copy of the funtion used in TLorentzVectors

#ifndef DELTAR
#define DELTAR

Double_t DeltaR(double Eta1, double Eta2, double Phi1, double Phi2) {

	Double_t deta = Eta1-Eta2;
	Double_t dphi = TVector2::Phi_mpi_pi(Phi1-Phi2);
	return TMath::Sqrt( deta*deta+dphi*dphi );

}

#endif
