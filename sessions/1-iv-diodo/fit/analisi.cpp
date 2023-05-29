Double_t expo1(Double_t* x, Double_t* par) {
  Float_t xx = x[0];
  return par[0] * (exp(xx / par[1]) - 1);
}

void analisi() {
  gROOT->SetStyle("Modern");
  gStyle->SetOptFit(0);

  TGraph* calibrazione = new TGraph("calibrazione.csv", "%lg %lg", ",");
  calibrazione->SetTitle(
      "Retta di Calibrazione;Tensione Oscilloscopio (V);Tensione Multimetro "
      "(V)");
  calibrazione->SetMarkerStyle(2);
  calibrazione->SetMarkerColor(12);
  calibrazione->SetMarkerSize(1);
  calibrazione->SetLineColor(12);

  calibrazione->Fit("pol1", "Q");
  TF1* retta = calibrazione->GetFunction("pol1");
  retta->SetLineColor(kRed + 1);
  retta->SetLineWidth(1);

  TGraphErrors* germanio =
      new TGraphErrors("Germanio2.csv", "%lg %lg %lg", ",");
  germanio->SetTitle("Diodo al Germanio; Tensione (mV); Corrente (mA)");
  germanio->SetMarkerStyle(2);
  germanio->SetMarkerColor(12);
  germanio->SetMarkerSize(1);
  germanio->SetLineColor(12);

  for (int i = 0; i < germanio->GetN(); i++) {
    double ex =
        (germanio->GetErrorX(i) / 10) * (germanio->GetErrorX(i) / 10) +
        (0.03 * germanio->GetPointX(i)) * (0.03 * germanio->GetPointX(i));
    germanio->SetPointError(i, sqrt(ex), 0);
  }

  TF1* ger_func = new TF1("ger_func", "expo1", 0, 120, 2);
  ger_func->SetParameters(1e-3, 50e+3);
  ger_func->SetParNames("I_{0}", "#etaV_{T}");

  germanio->Fit("ger_func", "RQ");
  TF1* fit_func = germanio->GetFunction("ger_func");
  fit_func->SetRange(0, 600);
  fit_func->SetLineColor(kRed + 1);
  fit_func->SetLineWidth(1);

  TGraphErrors* silicio =
      new TGraphErrors("Silicio2.csv", "%lg %lg %lg", ",");
  silicio->SetTitle("Diodo al Silicio; Tensione (mV); Corrente (mA)");
  silicio->SetMarkerStyle(2);
  silicio->SetMarkerColor(12);
  silicio->SetMarkerSize(1);
  silicio->SetLineColor(12);
  //silicio->SetMaximum(30e-3);

  for (int i = 0; i < silicio->GetN(); i++) {
    double ex = (silicio->GetErrorX(i) / 10) * (silicio->GetErrorX(i) / 10) +
                (0.03 * silicio->GetPointX(i)) * (0.03 * silicio->GetPointX(i));
    silicio->SetPointError(i, sqrt(ex), 0);
  }

  TF1* sil_func = new TF1("sil_func", "[0]*(exp(x/[1]) - 1)", 0, 610);
  sil_func->SetParameters(1e-6, 60e+3);
  sil_func->SetParNames("I_{0}", "#etaV_{T}");

  silicio->Fit("sil_func", "RQ");
  TF1* sil_fit = silicio->GetFunction("sil_func");
  sil_fit->SetRange(0, 820);
  sil_fit->SetLineColor(kRed + 1);
  sil_fit->SetLineWidth(1);

  TCanvas* canva1 = new TCanvas("Germanio");
  canva1->SetGrid();
  canva1->SetLogy();
  germanio->Draw("APE");
  fit_func->Draw("same");
  TCanvas* canva2 = new TCanvas("Silicio");
  canva2->SetGrid();
  canva2->SetLogy();
  silicio->Draw("APE");
  sil_fit->Draw("same");
  TCanvas* canva3 = new TCanvas("Calibrazione");
  canva3->SetGrid();
  calibrazione->Draw("APE");
  retta->Draw("same");

  double Chi_Sil = sil_fit->GetChisquare() / sil_fit->GetNDF();
  double Chi_Ger = fit_func->GetChisquare() / fit_func->GetNDF();
  double sil_0 = sil_fit->GetParameter(0);
  double sil_err_0 = sil_fit->GetParError(0);
  double ger_0 = fit_func->GetParameter(0);
  double ger_err_0 = fit_func->GetParError(0);
  double sil_1 = sil_fit->GetParameter(1);
  double sil_err_1 = sil_fit->GetParError(1);
  double ger_1 = fit_func->GetParameter(1);
  double ger_err_1 = fit_func->GetParError(1);

  double Chi_Cal = retta->GetChisquare() / retta->GetNDF();
  double intercetta = retta->GetParameter(0);
  double pendenza = retta->GetParameter(1);

  printf(
      "Germanio:\n\tChi Quadro = %f\n\tCorrente di Saturazione = %e +/- "
      "%e A\n\tEta = %f +/- %f\n",
      Chi_Ger, ger_0, ger_err_0, ger_1 / 26, ger_err_1 / 26);
  printf(
      "Silicio:\n\tChi Quadro = %f\n\tCorrente di Saturazione = %e +/- %e "
      "A\n\tEta = %f +/- %f\n",
      Chi_Sil, sil_0, sil_err_0, sil_1 / 26, sil_err_1 / 26);
  printf(
      "Calibrazione:\n\tChi Quadro = %f\n\tIntercetta = %e "
      "V\n\tPendenza = %f\n",
      Chi_Cal, intercetta, pendenza);
}
