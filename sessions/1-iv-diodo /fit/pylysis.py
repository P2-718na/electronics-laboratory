import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.optimize import curve_fit

Silicio = pd.read_csv("Silicio2.csv", sep=',', names=["mV","A","fondo_scala"])
Germanio = pd.read_csv("Germanio2.csv", sep=',', names=["V","A","fondo_scala"])
Calibrazione = pd.read_csv("calibrazione.csv", sep=',')

def pol1(x, A, B):
    y = A*x + B
    return y
def expo1(x, A, B):
    y = A*(np.exp(x/B) - 1)
    return y
def Variance(y, fy):
    var = 0
    for i in range(y.size):
        var += (y[i] - fy[i])**2
    return var

# Calibrazione ------------------------------------------------------
x_cal = pd.Series(Calibrazione["V_{osc}"])
y_cal = pd.Series(Calibrazione["V_{mult}"])
cal_par, cal_var = curve_fit(pol1, x_cal, y_cal, [1, 0])
cal_fit = pol1(x_cal, cal_par[0], cal_par[1])
print("Calibrazione")
print("\tPendenza: ", cal_par[0])
print("\tIntercetta: ", cal_par[1])

plt.plot(x_cal, y_cal, 'o')
plt.plot(x_cal, cal_fit)
plt.grid()
plt.title("Retta di Calibrazione")
plt.xlabel("Tensione Oscilloscopio(V)")
plt.ylabel("Tensione Multimetro (V)")
#--------------------------------------------------------------------
# Germanio2 ---------------------------------------------------------
x_ger = pd.Series(Germanio["V"])
y_ger = pd.Series(Germanio["A"])
x_fs = pd.Series(Germanio["fondo_scala"])
x_err = []
for i in range(x_fs.size):
    x_err.append(np.sqrt(x_fs[i]**2/100 + (0.03*x_ger[i])**2))

ger_par, ger_cov = curve_fit(expo1, x_ger[:11], y_ger[:11], [1e-6, 50e-3])
y_fit = expo1(x_ger, ger_par[0], ger_par[1])
Varianza = Variance(y_ger, y_fit)
print("Germanio")
print("\tCorrente di Saturazione: ", ger_par[0])
print("\tEta: ", ger_par[1]/26e-3)
print("\tVarianza: ", Varianza)


plt.figure()
plt.errorbar(x_ger,y_ger,yerr=None,xerr=x_err,fmt='o',markersize=3)
plt.plot(x_ger, y_fit)
plt.yscale('log')
plt.grid()
plt.title("Diodo al Germanio")
plt.xlabel("Tensione (V)")
plt.ylabel("Corrente (A)")
#--------------------------------------------------------------------
# Silicio2 ----------------------------------------------------------
x_sil = pd.Series(Silicio["mV"])
y_sil = pd.Series(Silicio["A"])
x_fss = pd.Series(Silicio["fondo_scala"])
x_errs = []
x_range = []
y_range = []
for i in range(x_sil.size):
    x_errs.append(np.sqrt(x_fss[i]**2/100 + (0.03*x_sil[i])**2))
    if (x_sil[i] < 610):
        x_range.append(x_sil[i])
        y_range.append(y_sil[i])

sil_par, sil_cov = curve_fit(expo1, x_range, y_range, [1e-9, 50])
y_fits = expo1(x_sil, sil_par[0], sil_par[1])
Varianzas = Variance(y_sil, y_fits)
print("Silicio")
print("\tCorrente di Saturazione: ", sil_par[0])
print("\tEta: ", sil_par[1]/26)
print("\tVarianza: ", Varianzas)

plt.figure()
plt.errorbar(x_sil,y_sil,yerr=None,xerr=x_errs,fmt='o',markersize=3)
plt.plot(x_sil, y_fits)
plt.yscale('log')
plt.grid()
plt.title("Diodo al Silicio")
plt.xlabel("Tensione (mV)")
plt.ylabel("Corrente (A)")
#--------------------------------------------------------------------
plt.show()
#print(Silicio.to_string())
#print(Germanio.to_string())
