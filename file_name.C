/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : myfile.c

* Purpose :

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

TString file_name(TString file, int leptonId, double par_low, double par_upp, TString select , TString effcut, double cut_den , double cut , TString par_x , TString option )
{

	/////////////////////////////////
	//Name of the input/output file//
	/////////////////////////////////

	//particle string
	TString pname;
	if(abs(leptonId) == 11){pname = "e";}
	if(abs(leptonId) == 13){pname = "mu";}

	//selection string
	TString _sel;

	if(select == "tight"){_sel = "tight";}
	else if(select == "loose"){_sel = "loose";}
	else if(select == ""){_sel = "unsel";}
	else{cout<<"ERROR: wrong selection !";return 1;}

	//cut string
	TString _effcut;
	if(effcut == "tight"){_effcut = "tight";}
	else if(effcut == ""){_effcut = "";}
	else if(effcut == "loose"){_effcut = "loose";}
	else if(effcut == "reliso3"){_effcut = Form("reliso3_%0.3lf",cut);}
	else if(effcut == "reliso4"){_effcut = Form("reliso4_%0.3lf",cut);}
	else if(effcut == "chiso3"){_effcut = Form("chiso3_%0.3lf",cut);}
	else if(effcut == "chiso4"){_effcut = Form("chiso4_%0.3lf",cut);}
	else if(effcut == "dxy"){_effcut = Form("dxy_%0.3lf",cut);}
	else if(effcut == "dz"){_effcut = Form("dz_%0.3lf",cut);}
	else{cout<<"ERROR: wrong numerator name !";return 1;}

	TString _fname = file;

	if(option.Contains("matching")){_fname += "_Matched";}
	if(option.Contains("nentries")){_fname += "_nentries";}
	if(option.Contains("ll")){_fname += "_nentries";}

	TString _ptrange;
	_ptrange = Form("Pt%0.f_Pt%0.f",par_low,par_upp);
	_fname += "_"+_ptrange;
	_fname += "_"+_effcut+"_for_"+_sel+"_"+pname+"_"+par_x; 

	return _fname;

}
