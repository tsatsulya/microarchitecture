import re
import glob
import math
import pandas as pd

rows = []

for path in sorted(glob.glob("results_hw3/*.out")):
    name = path.split("/")[-1].replace(".out", "")

    # replacement names contain underscores, so parse by known suffix.
    trace_start = None
    for marker in [
        "_600.", "_602.", "_603.", "_605.", "_607.", "_619.", "_620.",
        "_621.", "_623.", "_625.", "_627.", "_628.", "_631.", "_638.",
        "_641.", "_644.", "_648.", "_649.", "_654.", "_657."
    ]:
        idx = name.find(marker)
        if idx != -1:
            trace_start = idx + 1
            break

    if trace_start is None:
        print("WARNING: cannot parse filename", path)
        continue

    policy = name[:trace_start - 1]
    trace = name[trace_start:]

    text = open(path, errors="ignore").read()

    ipc_match = re.search(r"CPU 0 cumulative IPC:\s*([0-9.]+)", text)

    l2_match = re.search(
        r"cpu0->cpu0_L2C TOTAL\s+ACCESS:\s*([0-9]+)\s+HIT:\s*([0-9]+)\s+MISS:\s*([0-9]+)",
        text
    )

    if not ipc_match or not l2_match:
      print("WARNING: could not parse", path)
      continue

    access = int(l2_match.group(1))
    hit = int(l2_match.group(2))
    miss = int(l2_match.group(3))

    miss_rate = miss / access if access > 0 else 0.0

    rows.append({
        "policy": policy,
        "trace": trace.replace(".champsimtrace.xz", ""),
        "ipc": float(ipc_match.group(1)),
        "l2_access": access,
        "l2_hit": hit,
        "l2_miss": miss,
        "l2_miss_rate": miss_rate,
    })

df = pd.DataFrame(rows)

def gmean(series):
    values = [float(x) for x in series if float(x) > 0]
    return math.exp(sum(math.log(x) for x in values) / len(values))

summary = (
    df.groupby("policy")
      .agg(
          ipc_gmean=("ipc", gmean),
          l2_miss_rate_gmean=("l2_miss_rate", gmean),
          ipc_mean=("ipc", "mean"),
          l2_miss_rate_mean=("l2_miss_rate", "mean"),
      )
      .reset_index()
      .sort_values("policy")
)

df.to_csv("hw3_results.csv", index=False)
summary.to_csv("hw3_summary.csv", index=False)

print("Saved hw3_results.csv and hw3_summary.csv")
print()
print(summary.to_string(index=False))
