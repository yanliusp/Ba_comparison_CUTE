//@ liveTime in seconds
//@ zipName = 2 for R15
TH1D* GetHistogram(const string fileName, const string histName, double liveTime, const string zipName, double scaleFactor){

  TH1D *h = new TH1D(histName.c_str(), histName.c_str(), 200, 0, 600);

  double TriggerType, TriggerDetectorNum, TriggerMask;
  double PTOFamps, PTOFchisq;
  double EventTime;
  liveTime = liveTime/3600./24.; // convert to days
  TFile *f1 = new TFile(fileName.c_str(), "READ");
  TTree *t1 = (TTree*)f1->Get(("rqDir/zip" + zipName).c_str());
  TTree *t2 = (TTree*)f1->Get("rqDir/eventTree");
  t1->SetBranchAddress("PTOFamps", &PTOFamps);
  t1->SetBranchAddress("PTOFchisq", &PTOFchisq);
  t2->SetBranchAddress("TriggerType", &TriggerType);
  t2->SetBranchAddress("TriggerDetectorNum", &TriggerDetectorNum);
  t2->SetBranchAddress("TriggerMask", &TriggerMask);
  t2->SetBranchAddress("EventTime", &EventTime);


  //int a = 0;
  for (int i=0; i<t1->GetEntries(); i++) {
    t1->GetEntry(i);
    t2->GetEntry(i);
    if (TriggerType!=1.) continue; // only pass L1 primitives
    if (TriggerDetectorNum!=std::stod(zipName)) continue; // only pass T5Z2, this is 
    if (TriggerMask!=1.) continue; // only pass L1 primitives
    if (PTOFchisq>36000) continue;
    h->Fill(PTOFamps*scaleFactor);
    //if (a == 0) {a=1; cout << setprecision(12) << "first timestamp" << EventTime << endl;}
    //if (i > t1->GetEntries() - 20) cout << setprecision(12)<< "last few timestamps" << EventTime << endl;
  }

  h->Scale(1./liveTime/600./0.6); //0.6 kg is the mass

  return h;
}

void compare() {
  //r15 Ba
  TH1D* h_r15 = GetHistogram("Prod2T_23200903_171922.root", "h_r15", 20036., "2", 356./1.00163e-06); //21456
  //r15 Bg
  TH1D* h_rb15 = GetHistogram("Prod2T_23200907_000409.root", "h_rb15", 73693., "2", 356./1.00163e-06); //74715
  //r18 Ba
  TH1D* h_r18 = GetHistogram("Prod2T_23201115_134045.root", "h_r18", 40106., "2", 356./1.25638e-06); //41123
  //r18 Bg
  TH1D* h_rb18 = GetHistogram("Prod2T_23201116_130131.root", "h_rb18", 48300., "2", 356./1.25638e-06); //48487
  //r19 Ba
  TH1D* h_r19 = GetHistogram("ProdPD2_23210107_131117.root", "h_r19", 16368., "1", 356./1.81332e-06); //17390
  //r19 Bg
  TH1D* h_rb19 = GetHistogram("ProdPD2_23210107_232410.root", "h_rb19", 84872., "1", 356./1.81332e-06); //85280
  //r20 Ba
  TH1D* h_r20 = GetHistogram("ProdOnline_23210315_224559.root", "h_r20", 7749., "1", 356./8.61292e-07); //8811
  //r20 Bg
  TH1D* h_rb20 = GetHistogram("ProdOnline_23210314_221354.root", "h_rb20", 45432., "1", 356./8.61292e-07); //45832

  // plotting
  h_r15->SetLineColor(kRed);
  h_r18->SetLineColor(kBlue);
  h_r19->SetLineColor(kViolet);
  h_r20->SetLineColor(kBlack);
  h_rb15->SetLineColor(kRed+2);
  h_rb18->SetLineColor(kCyan);
  h_rb19->SetLineColor(kViolet+2);
  h_rb20->SetLineColor(kGray+3);

  h_r15->SetLineWidth(2);
  h_r18->SetLineWidth(2);
  h_r19->SetLineWidth(2);
  h_r20->SetLineWidth(2);
  h_rb15->SetLineWidth(2);
  h_rb18->SetLineWidth(2);
  h_rb19->SetLineWidth(2);
  h_rb20->SetLineWidth(2);

  h_r19->SetStats(0);
  h_r19->GetXaxis()->SetTitle("keV");
  h_r19->GetYaxis()->SetTitle("dru: event/kg/keV/day");
 
  h_r19->GetYaxis()->SetRangeUser(0, 50);
  //h_r19->GetYaxis()->SetRangeUser(1e-3, 1000);
  h_r19->SetTitle("High Rate Ba @ CUTE");
  h_r19->Draw("hist c");
  h_r18->Draw("hist c same");
  h_r15->Draw("hist c same");
  h_r20->Draw("hist c same");
//  h_rb19->Draw("hist c same");
//  h_rb18->Draw("hist c same");
//  h_rb15->Draw("hist c same");
//  h_rb20->Draw("hist c same");
//  gPad->SetLogy();

  auto legend = new TLegend(0.7,0.7,0.9,0.9);
  //legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
  legend->AddEntry(h_r15,"R15, Ba, 112 cm","l");
  legend->AddEntry(h_r18,"R18, Ba, 112 cm","l");
  legend->AddEntry(h_r19,"R19, Ba, 112 cm","l");
  legend->AddEntry(h_r20,"R20, Ba, 112 cm","l");
//  legend->AddEntry(h_rb15,"R15, Bg","l");
//  legend->AddEntry(h_rb18,"R18, Bg","l");
//  legend->AddEntry(h_rb19,"R19, Bg","l");
//  legend->AddEntry(h_rb20,"R20, Bg","l");
  legend->Draw();

}
