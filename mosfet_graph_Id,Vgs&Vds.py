import matplotlib.pyplot as plt
import pandas as pd

# Load data from CSV files
data_vgs = pd.read_csv("Vgs_vs_Id.csv")
data_vds = pd.read_csv("Vds_vs_Id.csv")

# Load summary from text file
summary_vars = []
try:
    with open("summary.txt", "r") as f:
        for line in f:
            if "=" in line:
                summary_vars.append(line.strip())
except FileNotFoundError:
    summary_vars.append("summary.txt not found")

# Create bottom text string
summary_text = " | ".join(summary_vars)

# Plot 1: Vgs vs Id
plt.figure(figsize=(8, 5))
plt.plot(data_vgs["Vgs"], data_vgs["Id"], label="Id vs Vgs", linewidth=2)
plt.xlabel("Vgs (V)")
plt.ylabel("Id (A)")
plt.title("MOSFET Vgs vs Id")
plt.grid(True)
plt.legend()
plt.figtext(0.5, 0.01, summary_text, wrap=True, ha="center", fontsize=9)
plt.tight_layout()
plt.savefig("Vgs_vs_Id.png")

# Plot 2: Vds vs Id
plt.figure(figsize=(8, 5))
plt.plot(
    data_vds["Vds"], data_vds["Id"], label="Id vs Vds", color="orange", linewidth=2
)
plt.xlabel("Vds (V)")
plt.ylabel("Id (A)")
plt.title("MOSFET Vds vs Id")
plt.grid(True)
plt.legend()
plt.figtext(0.5, 0.01, summary_text, wrap=True, ha="center", fontsize=9)
plt.tight_layout()
plt.savefig("Vds_vs_Id.png")

plt.show()