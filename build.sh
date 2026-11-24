#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"

# Automatically detect current script directory
CURRENT_DIR="$(cd "$(dirname "$0")" && pwd)"

NDK_BUILD="/data/user/0/aidepro.top/no_backup/ndksupport-1710240003/android-ndk-aide/build/ndk-build"


TARGET_DIR="$CURRENT_DIR/libs/arm64-v8a"

echo -e "${GREEN}[*] Starting NDK build...${RESET}"
if "$NDK_BUILD"; then
    echo -e "${GREEN}[✓] NDK build completed successfully.${RESET}"
else
    echo -e "${RED}[✗] NDK build failed.${RESET}"
    exit 1
fi


FILE_TO_MOVE=$(find "$TARGET_DIR" -maxdepth 1 -type f \( -name "*.so" -o -name "*.sh" -o ! -name "*.*" \) | head -n 1)

if [ -z "$FILE_TO_MOVE" ]; then
    echo -e "${RED}[✗] No matching file found (.so, .sh, or no extension).${RESET}"
    exit 1
fi

echo -e "${GREEN}[*] Identified file: ${FILE_TO_MOVE}${RESET}"

DEST_DIR="/data/local/tmp/Inject"

echo -e "${GREEN}[*] Moving file...${RESET}"
if cp -f "$FILE_TO_MOVE" "$DEST_DIR/libAxiomRuntime.so"; then
    echo -e "${GREEN}[✓] File moved successfully.${RESET}"
else
    echo -e "${RED}[✗] Failed to move file.${RESET}"
    exit 1
fi

FILENAME="$(basename "$FILE_TO_MOVE")"

cd "$DEST_DIR" || {
    echo -e "${RED}[✗] Failed to change directory to $DEST_DIR.${RESET}"
    exit 1
}

cd "$CURRENT_DIR" || {
    echo -e "${RED}[✗] Failed to change directory to $DEST_DIR.${RESET}"
    exit 1
}

chmod 777 "$DEST_DIR/libAxiomRuntime.so"

./Inject.sh