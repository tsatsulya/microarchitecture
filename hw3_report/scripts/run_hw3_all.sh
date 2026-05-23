#!/usr/bin/env bash
set -e

WARMUP=10000000
SIM=50000000

TRACE_DIR=traces
OUT_DIR=results_hw3

mkdir -p "$OUT_DIR"

REPLS="lru pseudo_lru srrip lru_lip lru_bip"

TRACES="
600.perlbench_s-1273B.champsimtrace.xz
602.gcc_s-1850B.champsimtrace.xz
603.bwaves_s-2931B.champsimtrace.xz
605.mcf_s-1536B.champsimtrace.xz
607.cactuBSSN_s-2421B.champsimtrace.xz
619.lbm_s-2676B.champsimtrace.xz
620.omnetpp_s-141B.champsimtrace.xz
621.wrf_s-575B.champsimtrace.xz
623.xalancbmk_s-165B.champsimtrace.xz
625.x264_s-12B.champsimtrace.xz
627.cam4_s-490B.champsimtrace.xz
628.pop2_s-17B.champsimtrace.xz
631.deepsjeng_s-928B.champsimtrace.xz
638.imagick_s-10316B.champsimtrace.xz
641.leela_s-149B.champsimtrace.xz
644.nab_s-12459B.champsimtrace.xz
648.exchange2_s-387B.champsimtrace.xz
649.fotonik3d_s-1176B.champsimtrace.xz
654.roms_s-293B.champsimtrace.xz
657.xz_s-4994B.champsimtrace.xz
"

for repl in $REPLS; do
  for trace in $TRACES; do
    out="$OUT_DIR/${repl}_${trace}.out"

    if [ -f "$out" ]; then
      echo "Skip existing $out"
      continue
    fi

    echo "Running replacement=$repl trace=$trace"

    ./bin/champsim_hw3_$repl \
      --warmup-instructions "$WARMUP" \
      --simulation-instructions "$SIM" \
      "$TRACE_DIR/$trace" \
      > "$out"

    grep "CPU 0 cumulative IPC\|cpu0->cpu0_L2C TOTAL" "$out"
  done
done
