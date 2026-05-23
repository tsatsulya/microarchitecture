import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("hw3_results.csv")

for metric in ["ipc", "l2_miss_rate"]:
    pivot = df.pivot(index="trace", columns="policy", values=metric)

    ax = pivot.plot(kind="bar", figsize=(22, 7))
    ax.set_title(f"{metric} by trace")
    ax.set_ylabel(metric)
    ax.set_xlabel("Trace")
    ax.grid(axis="y")

    plt.xticks(rotation=80, ha="right")
    plt.tight_layout()
    plt.savefig(f"hw3_{metric}_by_trace.png", dpi=200)
    plt.close()

print("Saved:")
print("  hw3_ipc_by_trace.png")
print("  hw3_l2_miss_rate_by_trace.png")
