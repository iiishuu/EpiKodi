#!/bin/bash

# Script de test pour vérifier les capacités multimédia sous WSL

echo "=== Test des capacités multimédia sous WSL ==="

# Test 1: Vérification de l'affichage X11
echo "1. Test DISPLAY..."
if [ -z "$DISPLAY" ]; then
    echo "   ❌ Variable DISPLAY non définie"
    echo "   💡 Lancez: export DISPLAY=:0"
else
    echo "   ✅ DISPLAY=$DISPLAY"
fi

# Test 2: Test Qt
echo "2. Test Qt5..."
if command -v qmake-qt5 &> /dev/null; then
    echo "   ✅ Qt5 installé"
    qmake-qt5 --version
else
    echo "   ❌ Qt5 non trouvé"
fi

# Test 3: Test GStreamer
echo "3. Test GStreamer..."
if command -v gst-launch-1.0 &> /dev/null; then
    echo "   ✅ GStreamer installé"
    gst-inspect-1.0 --version
else
    echo "   ❌ GStreamer non trouvé"
fi

# Test 4: Test VLC
echo "4. Test VLC..."
if command -v vlc &> /dev/null; then
    echo "   ✅ VLC installé"
    vlc --version | head -n 1
else
    echo "   ❌ VLC non installé"
    echo "   💡 Installation: sudo apt install vlc"
fi

# Test 5: Test MPV
echo "5. Test MPV..."
if command -v mpv &> /dev/null; then
    echo "   ✅ MPV installé"
    mpv --version | head -n 1
else
    echo "   ❌ MPV non installé"
    echo "   💡 Installation: sudo apt install mpv"
fi

# Test 6: Test des codecs installés
echo "6. Test des codecs..."
echo "   Formats supportés par GStreamer:"
if command -v gst-inspect-1.0 &> /dev/null; then
    gst-inspect-1.0 | grep -E "(mp4|h264|aac|mp3)" | head -3
fi

echo ""
echo "=== Résumé ==="
echo "Si VLC ou MPV sont installés, vous pouvez les utiliser comme solution de secours."
echo "Pour tester: vlc assets/votre_video.mp4"