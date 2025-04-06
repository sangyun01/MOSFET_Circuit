import matplotlib.pyplot as plt
import pandas as pd

# CSV 파일 불러오기
data_vgs = pd.read_csv("Vgs_vs_Id.csv")
data_vds = pd.read_csv("Vds_vs_Id.csv")

# summary.txt에서 변수 값 읽기
summary_vars = []
try:
    with open("summary.txt", "r") as f:
        for line in f:
            if "=" in line:
                summary_vars.append(line.strip())
except FileNotFoundError:
    summary_vars.append("summary.txt not found")

# 요약 정보 텍스트 만들기
summary_text = " | ".join(summary_vars)

# 1. Vgs vs Id 그래프
plt.figure(figsize=(8, 5))
plt.plot(data_vgs["Vgs"], data_vgs["Id"], label="Id vs Vgs", linewidth=2)
plt.xlabel("Vgs (V)")
plt.ylabel("Id (A)")
plt.title("Vgs vs Id")
plt.grid(True)
plt.legend()
plt.figtext(0.5, 0.01, summary_text, wrap=True, ha="center", fontsize=9)
plt.tight_layout()
plt.savefig("Vgs_vs_Id.png")

# 2. Vds vs Id 그래프
plt.figure(figsize=(8, 5))
plt.plot(
    data_vds["Vds"], data_vds["Id"], label="Id vs Vds", color="orange", linewidth=2
)
plt.xlabel("Vds (V)")
plt.ylabel("Id (A)")
plt.title("Vds vs Id")
plt.grid(True)
plt.legend()
plt.figtext(0.5, 0.01, summary_text, wrap=True, ha="center", fontsize=9)
plt.tight_layout()
plt.savefig("Vds_vs_Id.png")

plt.show()
