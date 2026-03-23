# Webcam Viewer Application

A professional desktop application built with Qt 6 and C++ for capturing and displaying live video from system webcams.

## Screenshots

The application features a clean, intuitive interface with:

```
┌─────────────────────────────────────────────────────────┐
│ Webcam Viewer                                       [_][□][X]
├─────────────────────────────────────────────────────────┤
│ Camera Controls                                         │
│ Select Camera: [Integrated Camera ▼]  [Start] [Snapshot] [Record] │
│                                                         │
├─────────────────────────────────────────────────────────┤
│ Video Feed                                              │
│                                                         │
│                    [Live Video Stream]                  │
│                                                         │
│                                                         │
│                                                         │
├─────────────────────────────────────────────────────────┤
│ Ready                                                   │
└─────────────────────────────────────────────────────────┘
```

**Key UI Elements:**
- Camera selection dropdown with auto-detection
- Start/Stop button (red when recording)
- Snapshot button (captures current frame)
- Record button (toggles video recording)
- Live video display area (black background when idle)
- Status bar showing application state
- Warning banner when no camera is detected

## Features

- **Multi-Camera Support**: Detect and select from available camera devices
- **Live Video Feed**: Real-time video streaming with smooth playback
- **Snapshot Capture**: Save individual frames as PNG images
- **Video Recording**: Record video clips in MP4 format
- **Responsive UI**: Non-blocking operations with proper threading
- **Error Handling**: Comprehensive error handling for camera unavailability and conflicts
- **Professional Architecture**: Clean separation of concerns with manager pattern

## Project Structure

```
WebcamViewer/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── src/
│   ├── main.cpp               # Application entry point
│   ├── mainwindow.h/cpp       # Main UI window
│   ├── cameramanager.h/cpp    # Camera management logic
│   └── videorecorder.h/cpp    # Video/snapshot utilities
└── resources/
    └── resources.qrc          # Qt resource file
```

## Requirements

- Qt 6.2 or higher
- C++17 compatible compiler
- CMake 3.16 or higher
- Platform-specific multimedia libraries

### Platform-Specific Requirements

**Windows:**
- Visual Studio 2019+ or MinGW
- Windows Media Foundation (included with Windows)

**Linux:**
- GCC/Clang with C++17 support
- libqt6multimedia6
- libqt6multimediawidgets6

**macOS:**
- Xcode 12+
- AVFoundation framework (included with macOS)

## Building

### Prerequisites

1. Install Qt 6 (with Multimedia module)
2. Install CMake 3.16+

### Build Steps

**Windows (Visual Studio):**
```bash
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

**Linux/macOS:**
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Running

**Windows:**
```bash
build\Release\WebcamViewer.exe
```

**Linux/macOS:**
```bash
./build/WebcamViewer
```

## Usage

1. **Select Camera**: Choose a camera from the dropdown menu
2. **Start Stream**: Click "Start" to begin video capture
3. **Capture Snapshot**: Click "Snapshot" to save the current frame
4. **Record Video**: Click "Record" to start recording, click again to stop
5. **Stop Stream**: Click "Stop" to end video capture

### Output Files

- **Snapshots**: Saved to `Documents/WebcamViewer/snapshot_YYYY-MM-DD_HH-MM-SS-zzz.png`
- **Videos**: Saved to `Documents/WebcamViewer/video_YYYY-MM-DD_HH-MM-SS.mp4`

## Architecture

### CameraManager
Handles all camera operations:
- Camera device detection and selection
- Start/stop camera streaming
- Snapshot capture
- Video recording control
- Error handling and reporting

### MainWindow
Manages the user interface:
- Camera selection dropdown
- Control buttons (Start/Stop, Snapshot, Record)
- Video display widget
- Status messages and error reporting
- UI state management

### VideoRecorder
Utility class for file operations:
- Generate timestamped file paths
- Manage output directory
- Ensure directory existence

## Error Handling

The application handles the following scenarios:

- **No cameras available**: Displays error message and disables controls
- **Camera in use**: Gracefully handles camera unavailability
- **Recording failures**: Reports errors and maintains UI state
- **Invalid selections**: Validates user input before operations
- **File system errors**: Handles snapshot/video save failures

## Key Design Patterns

1. **Manager Pattern**: CameraManager encapsulates all camera logic
2. **Signal/Slot Pattern**: Qt's event-driven architecture for loose coupling
3. **RAII**: Smart pointers for automatic resource management
4. **State Management**: Proper UI state tracking and updates

## Performance Considerations

- Non-blocking camera operations using Qt's event loop
- Efficient frame capture using QVideoSink
- Minimal memory footprint with smart pointer management
- Responsive UI maintained during recording

## Troubleshooting

**Camera not detected:**
- Ensure camera is connected and not in use by another application
- Check system permissions for camera access
- Restart the application

**Recording fails:**
- Verify write permissions to Documents folder
- Ensure sufficient disk space
- Check that output directory was created successfully

**Video playback stutters:**
- Close other applications using the camera
- Reduce system load
- Check camera driver is up to date

## Future Enhancements

- Camera settings adjustment (brightness, contrast, etc.)
- Multiple simultaneous camera support
- Video format selection
- Frame rate adjustment
- Filters and effects
- Streaming to network

## License

This project is provided as-is for educational and commercial use.

## Support

For issues or questions, please refer to the Qt documentation:
- Qt Multimedia: https://doc.qt.io/qt-6/qtmultimedia-index.html
- Qt Widgets: https://doc.qt.io/qt-6/qtwidgets-index.html
