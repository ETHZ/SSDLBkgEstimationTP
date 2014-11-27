double InvMass(double pt1, double eta1, double phi1, double m1, double pt2, double eta2, double phi2, double m2){

	//double X1;
	//double X2;
	//double Y1;
	//double Y2;
	//double Z1;
	//double Z2;
	//double T1;
	//double T2;

	//X1 = pt1*TMath::Cos(phi1);
	//X2 = pt2*TMath::Cos(phi2);
	//Y1 = pt1*TMath::Sin(phi1);
	//Y2 = pt2*TMath::Sin(phi2);
	//Z1 = pt1*sinh(eta1);
	//Z2 = pt2*sinh(eta1);

	//T1 = TMath::sqrt(X1*X1+Y1*Y1+Z1*Z1 + m1*m1);
	//T2 = TMath::sqrt(X2*X2+Y2*Y2+Z2*Z2 + m2*m2);

	//return T1*T2 -(X1*X2 + Y1*Y2 + Z1*Z2);

	TLorentzVector vec1;
	TLorentzVector vec2;
	vec1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
	vec2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
	TLorentzVector Z = vec1 + vec2;
	return  Z.M();

}
