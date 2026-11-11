
#!/usr/bin/env bash

set -euo pipefail

# Menentukan direktori proyek secara otomatis
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Direktori penyimpanan log
LOG_DIR="$PROJECT_DIR/test_logs"
LOG_FILE="$LOG_DIR/dummy_test.log"

mkdir -p "$LOG_DIR"

echo "================================"
echo "GVSoC Dummy IP Automated Test"
echo "================================"

cd "$PROJECT_DIR"

echo "[INFO] Starting GVSoC simulation..."

# Menjalankan simulasi dengan batas waktu
if timeout 30s gvsoc \
    --target-dir="$PROJECT_DIR" \
    --target=testbench \
    --work-dir="$PROJECT_DIR/build" \
    run > "$LOG_FILE" 2>&1
then
    echo "[INFO] Simulation finished normally."
else
    STATUS=$?
    cat "$LOG_FILE"
    echo "[FAIL] GVSoC exited with code: $STATUS"
    exit 1
fi

# Menampilkan hasil simulasi
cat "$LOG_FILE"

# Memeriksa pesan PASS aktual dari GVSoC
if grep -Fq "[PASS] Dummy IP Echo/Loopback OK!" "$LOG_FILE"; then
    echo "================================"
    echo "[SUCCESS] Dummy IP Test PASSED"
    echo "================================"
    exit 0
else
    echo "================================"
    echo "[FAIL] Dummy IP Test FAILED"
    echo "================================"
    exit 1
fi

