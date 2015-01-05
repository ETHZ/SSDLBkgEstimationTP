The script in this folder compare the Tag and Probe with the MC Ratio using the file "CompareMCvsTandP.C". It works as follows:

1) Start with a call of CompareMCvsTandP.C
2) CompareMCvsTandP.C calls then MC_Ratio.C which computes the efficiency using the MC ratio and stores them.
3) CompareMCvsTandP.C calls then TandP.C that compute the efficiency using the Tag and Probe methode. It follows the following steps:
	a) Check if the invariant mass distribution for the asked efficiency exists. If not, it creates and store the invariant mass
	   histogram by calling DrawInvMassBkg3.C
	b) Once the invariant mass distribution exists, it fits it using FitInvMassBkg_v3.C. The efficiency is computed from 
	   the fit in FitInvMassBkg_v3 and returned. The Fit of each Invariant mass is stored.

4)CompareMCvsTandP.C retreive the two efficiency curves and plot them on the same canvas.
	  



