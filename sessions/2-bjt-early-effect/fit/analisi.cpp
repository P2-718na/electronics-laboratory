void analisi() {
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(0);

  TGraphErrors* graph100 = new TGraphErrors("100mA.csv", "%lg %lg %lg", ",");
  graph100->SetMarkerStyle(2);
  graph100->SetMarkerColor(12);
  graph100->SetMarkerSize(1);
  graph100->SetLineColor(12);
  for (int i = 0; i < graph100->GetN(); i++) {
    double e1 = graph100->GetErrorX(i) / 10;
    double e2 = graph100->GetPointX(i) * 0.03;
    double ex = sqrt(e1 * e1 + e2 * e2);
    graph100->SetPointError(i, ex, 0);
  }

  TGraphErrors* graph150 = new TGraphErrors("150mA.csv", "%lg %lg %lg", ",");
  graph150->SetMarkerStyle(2);
  graph150->SetMarkerColor(12);
  graph150->SetMarkerSize(1);
  graph150->SetLineColor(12);
  for (int i = 0; i < graph150->GetN(); i++) {
    double e1 = graph150->GetErrorX(i) / 10;
    double e2 = graph150->GetPointX(i) * 0.03;
    double ex = sqrt(e1 * e1 + e2 * e2);
    graph150->SetPointError(i, ex, 0);
  }

  TGraphErrors* graph200 = new TGraphErrors("200mA.csv", "%lg %lg %lg", ",");
  graph200->SetMarkerStyle(2);
  graph200->SetMarkerColor(12);
  graph200->SetMarkerSize(1);
  graph200->SetLineColor(12);
  for (int i = 0; i < graph200->GetN(); i++) {
    double e1 = graph200->GetErrorX(i) / 10;
    double e2 = graph200->GetPointX(i) * 0.03;
    double ex = sqrt(e1 * e1 + e2 * e2);
    graph200->SetPointError(i, ex, 0);
  }

  graph100->Fit("pol1", "Q", "", 1, 4);
  graph150->Fit("pol1", "Q", "", 1, 4);
  graph200->Fit("pol1", "Q", "", 1, 4);

  TF1* func100 = graph100->GetFunction("pol1");
  func100->SetRange(0, 4);
  func100->SetLineColor(kRed + 1);
  func100->SetLineWidth(1);

  TF1* func150 = graph150->GetFunction("pol1");
  func150->SetRange(0, 4);
  func150->SetLineColor(kBlue + 1);
  func150->SetLineWidth(1);

  TF1* func200 = graph200->GetFunction("pol1");
  func200->SetRange(0, 4);
  func200->SetLineColor(kGreen + 1);
  func200->SetLineWidth(1);

  TMultiGraph* graph_tot = new TMultiGraph();
  graph_tot->SetTitle(
      "Caratteristica Transistor BJT; Tensione #left|V_{ce}#right| (V); Corrente in Uscita #left|I_{c}#right| (mA)");
  graph_tot->Add(graph100);
  graph_tot->Add(graph150);
  graph_tot->Add(graph200);


  TCanvas* canva = new TCanvas("Transistor BJT");
  TLegend* legend = new TLegend(0.65, 0.13, 0.88, 0.28);
  legend->AddEntry(func100,"I_{b} = -100 mA","l");
  legend->AddEntry(func150,"I_{b} = -150 mA","l");
  legend->AddEntry(func200,"I_{b} = -200 mA","l");
  canva->SetGrid();
  graph_tot->Draw("APE");
  func100->Draw("SAME");
  func150->Draw("SAME");
  func200->Draw("SAME");
  legend->Draw("same");

  double Chi100 = func100->GetChisquare() / func100->GetNDF();
  double Chi150 = func150->GetChisquare() / func150->GetNDF();
  double Chi200 = func200->GetChisquare() / func200->GetNDF();

  double Slope100 = func100->GetParameter(0);
  double Slope150 = func150->GetParameter(0);
  double Slope200 = func200->GetParameter(0);
  double Intercetta100 = func100->GetParameter(1);
  double Intercetta150 = func150->GetParameter(1);
  double Intercetta200 = func200->GetParameter(1);

  double err0100 = func100->GetParError(0);
  double err0150 = func150->GetParError(0);
  double err0200 = func200->GetParError(0);
  double err1100 = func100->GetParError(1);
  double err1150 = func150->GetParError(1);
  double err1200 = func200->GetParError(1);

  printf(
      "\nIb -100 mA:\n\tChi Quadro: %f\n\tSlope: %e +/- %e\n\tIntercetta: %e "
      "+/- %e",
      Chi100, Slope100, err0100, Intercetta100, err1100);
  printf(
      "\nIb -150 mA:\n\tChi Quadro: %f\n\tSlope: %e +/- %e\n\tIntercetta: %e "
      "+/- %e",
      Chi150, Slope150, err0150, Intercetta150, err1150);
  printf(
      "\nIb -200 mA:\n\tChi Quadro: %f\n\tSlope: %e +/- %e\n\tIntercetta: %e "
      "+/- %e",
      Chi200, Slope200, err0200, Intercetta200, err1200);
  cout << endl;
}
