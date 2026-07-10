#!/bin/bash
export DISPLAY=:99
cd /workspace/v5/build

pkill -f FLStudioEditor 2>/dev/null
sleep 1

./FLStudioEditor &
APP_PID=$!
sleep 3

# Try to activate window and send shortcut
WIN_ID=$(xdotool search --name "FL Studio Editor" 2>/dev/null | head -1)
if [ -n "$WIN_ID" ]; then
    xdotool windowactivate $WIN_ID 2>/dev/null
    sleep 0.5
    xdotool key --window $WIN_ID ctrl+2 2>/dev/null
    sleep 1
fi

import -window root /workspace/v5/screenshot_step.png

kill $APP_PID 2>/dev/null
echo "Step sequencer screenshot saved"