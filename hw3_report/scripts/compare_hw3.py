import pandas as pd

summary = pd.read_csv("hw3_summary.csv")

base = summary[summary["policy"] == "lru"].iloc[0]

summary["ipc_gmean_vs_lru_%"] = (summary["ipc_gmean"] / base["ipc_gmean"] - 1.0) * 100.0
summary["l2_miss_rate_gmean_reduction_vs_lru_%"] = (
    1.0 - summary["l2_miss_rate_gmean"] / base["l2_miss_rate_gmean"]
) * 100.0

summary = summary.sort_values("policy")
summary.to_csv("hw3_summary_computed.csv", index=False)

print(summary.to_string(index=False))
print()
print("Saved hw3_summary_computed.csv")
