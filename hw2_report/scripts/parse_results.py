import re
import glob
import math
import pandas as pd

rows = []

for path in sorted(glob.glob("results_full/*.out")):
    name = path.split("/")[-1]

    pred = name.split("_")[0]
    trace = name[len(pred) + 1:].replace(".out", "")

    text = open(path, errors="ignore").read()

    ipc_match = re.search(r"CPU 0 cumulative IPC:\s*([0-9.]+)", text)
    br_match = re.search(
        r"Branch Prediction Accuracy:\s*([0-9.]+)%\s*MPKI:\s*([0-9.]+)",
        text
    )

    if not ipc_match or not br_match:
        print("WARNING: could not parse", path)
        continue

    rows.append({
        "predictor": pred,
        "trace": trace.replace(".champsimtrace.xz", ""),
        "ipc": float(ipc_match.group(1)),
        "branch_accuracy": float(br_match.group(1)),
        "mpki": float(br_match.group(2)),
    })

df = pd.DataFrame(rows)

def gmean(series):
    values = [x for x in series if x > 0]
    return math.exp(sum(math.log(x) for x in values) / len(values))

summary = (
    df.groupby("predictor")
      .agg(
          ipc_gmean=("ipc", gmean),
          mpki_gmean=("mpki", gmean),
          accuracy_mean=("branch_accuracy", "mean"),
      )
      .reset_index()
      .sort_values("predictor")
)

df.to_csv("hw2_results.csv", index=False)
summary.to_csv("hw2_summary.csv", index=False)

print("\nPer-trace results:")
print(df.to_string(index=False))

print("\nSummary:")
print(summary.to_string(index=False))
