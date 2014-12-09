//Side function that returns the invariant mass

double InvMass(double pt1, double eta1, double phi1, double m1, double pt2, double eta2, double phi2, double m2){

	TLorentzVector vec1;
	TLorentzVector vec2;
	vec1.SetPtEtaPhiM(pt1, eta1, phi1, m1);
	vec2.SetPtEtaPhiM(pt2, eta2, phi2, m2);
	TLorentzVector Z = vec1 + vec2;
	return  Z.M();

}
