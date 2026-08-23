#!/bin/bash

# Usage:
#   ./build.sh        Build a .hex suitable for flashing to real hardware
#   ./build.sh -s     Build a .hex suitable for SimulIDE simulation

set -euo pipefail

SIM_MODE=false
while getopts "s" opt; do
  case $opt in
    s) SIM_MODE=true ;;
  esac
done

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$DIR/../.." && pwd)"

COMPILE_CMD="arduino-cli compile -e -b arduino:avr:mega \
  --libraries ~/Arduino/libraries --library \"$REPO_ROOT\" --clean \
  --build-property build.extra_flags=\"-DDEBUG\""

cd "$DIR"
if $SIM_MODE; then
  eval "$COMPILE_CMD ."
  HEX=$(ls build/arduino.avr.mega/*.hex | grep -v bootloader | head -1)
  SIM_HEX="${HEX%.hex}.sim.hex"
  python3 - "$HEX" "$SIM_HEX" << 'EOF'
import sys
def checksum(data_bytes):
    return (0x100 - sum(data_bytes) % 0x100) % 0x100
with open(sys.argv[1]) as f_in, open(sys.argv[2], 'w') as f_out:
    for line in f_in:
        line = line.strip()
        if line[7:9] == '02':
            segment = int(line[9:13], 16)
            upper16 = segment >> 12
            b = [0x02, 0x00, 0x00, 0x04, upper16 >> 8, upper16 & 0xFF]
            f_out.write(f':{b[0]:02X}{b[1]:02X}{b[2]:02X}{b[3]:02X}{b[4]:02X}{b[5]:02X}{checksum(b):02X}\n')
        else:
            f_out.write(line + '\n')
EOF
  echo "SimulIDE-compatible hex: $SIM_HEX"
else
  eval "$COMPILE_CMD ."
fi
