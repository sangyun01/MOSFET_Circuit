import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv("Id_vs_Vgs.csv")

plt.plot(data["Vgs"], data["Id_real"], label="Real", linewidth=2)
plt.plot(data["Vgs"], data["Id_ideal"], label="Ideal", linestyle="--", linewidth=2)

plt.xlabel("Vgs (V)")
plt.ylabel("Id (A)")
plt.title("MOSFET Id vs Vgs (from C++ Simulation)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()