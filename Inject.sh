#!/system/bin/sh

GAME_PKG="com.GlobalSoFunny.Sausage"
GAME_ACT="com.funny.FunActivity"
TARGET_PKG="com.GlobalSoFunny.Sausage"
INJECTOR="/data/local/tmp/Inject/AndKittyInjector"
LIB="/data/local/tmp/Inject/libAxiomRuntime.so"
chmod 777 $LIB
echo "[*] Checking SELinux status..."

SELINUX=$(getenforce)

if [ "$SELINUX" = "Enforcing" ]; then
echo "[] SELinux is Enforcing, disabling..."
su -c setenforce 0
else
echo "[] SELinux already Permissive"
fi

echo "[*] Stopping target package..."
am force-stop $TARGET_PKG
pkill -f $TARGET_PKG

sleep 3

echo "[*] Checking if game process already exists..."

PID=$(pidof $GAME_PKG)

if [ ! -z "$PID" ]; then
echo "[] Game running with PID $PID — killing..."
su -c kill -9 $PID
sleep 2
else
echo "[] Game not running"
fi

echo "[*] Starting game..."
su -c am start -n $GAME_PKG/$GAME_ACT

echo "[] Waiting for game process..."
while true
do
PID=$(pidof $GAME_PKG)
if [ ! -z "$PID" ]; then
echo "[] Game started with PID $PID"
break
fi
sleep 1
done

echo "[*] Waiting for libil2cpp.so to load..."

while true
do
    PID=$(pidof $GAME_PKG)

    if [ ! -z "$PID" ]; then
        if grep -q "libil2cpp.so" /proc/$PID/maps 2>/dev/null; then
            echo "[*] libil2cpp.so detected in memory"
            break
        fi
    fi

    sleep 1
done

sleep 2

echo "[*] Injecting library..."
$INJECTOR --package $GAME_PKG --libs $LIB

echo "[*] Starting logcat..."
su -c logcat -s Kezxeon
