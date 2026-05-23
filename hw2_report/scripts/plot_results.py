import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("hw2_results.csv")

for metric in ["ipc", "mpki"]:
    pivot = df.pivot(index="trace", columns="predictor", values=metric)

    ax = pivot.plot(kind="bar", figsize=(20, 7))
    ax.set_title(f"{metric.upper()} by trace")
    ax.set_ylabel(metric.upper())
    ax.set_xlabel("Trace")
    ax.grid(axis="y")

    plt.xticks(rotation=80, ha="right")
    plt.tight_layout()
    plt.savefig(f"{metric}_by_trace.png", dpi=200)
    plt.close()

print("Saved ipc_by_trace.png and mpki_by_trace.png")
