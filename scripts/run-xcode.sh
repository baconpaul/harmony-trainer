#!/bin/sh
cmake . -Bbuild -GXcode || exit 2
xcodebuild build -configuration Release -project build/HarmonyTrainer.xcodeproj || exit 3
./build/HarmonyTrainer_artefacts/Release/HarmonyTrainer.app/Contents/MacOS/HarmonyTrainer 
