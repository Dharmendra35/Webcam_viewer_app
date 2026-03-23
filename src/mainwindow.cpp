#include "mainwindow.h"
#include "cameramanager.h"
#include "videorecorder.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QTimer>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_isRunning(false)
    , m_isRecording(false)
{
    setWindowTitle("Webcam Viewer");
    setGeometry(100, 100, 1024, 768);
    
    m_cameraManager = std::make_unique<CameraManager>(this);
    
    setupUI();
    createConnections();
    
    // Ensure output directory exists
    VideoRecorder::ensureOutputDirectoryExists();
    
    updateUIState();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Warning Panel for no cameras
    m_warningLabel = new QLabel(this);
    m_warningLabel->setStyleSheet("background-color: #fff3cd; color: #856404; padding: 10px; border: 1px solid #ffc107; border-radius: 4px;");
    m_warningLabel->setVisible(false);
    mainLayout->addWidget(m_warningLabel);
    
    // Control Panel
    QGroupBox *controlGroup = new QGroupBox("Camera Controls", this);
    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    
    // Camera selection
    QLabel *cameraLabel = new QLabel("Select Camera:", this);
    m_cameraComboBox = new QComboBox(this);
    m_cameraComboBox->addItems(m_cameraManager->getAvailableCameras());
    controlLayout->addWidget(cameraLabel);
    controlLayout->addWidget(m_cameraComboBox);
    
    controlLayout->addSpacing(20);
    
    // Start/Stop button
    m_startStopButton = new QPushButton("Start", this);
    m_startStopButton->setMinimumWidth(100);
    controlLayout->addWidget(m_startStopButton);
    
    // Snapshot button
    m_snapshotButton = new QPushButton("Snapshot", this);
    m_snapshotButton->setMinimumWidth(100);
    controlLayout->addWidget(m_snapshotButton);
    
    // Record button
    m_recordButton = new QPushButton("Record", this);
    m_recordButton->setMinimumWidth(100);
    controlLayout->addWidget(m_recordButton);
    
    controlLayout->addStretch();
    
    // Recording indicator
    m_recordingIndicator = new QLabel("", this);
    m_recordingIndicator->setStyleSheet("color: red; font-weight: bold;");
    controlLayout->addWidget(m_recordingIndicator);
    
    mainLayout->addWidget(controlGroup);
    
    // Video display
    QGroupBox *videoGroup = new QGroupBox("Video Feed", this);
    QVBoxLayout *videoLayout = new QVBoxLayout(videoGroup);
    
    m_videoWidget = new QVideoWidget(this);
    m_videoWidget->setMinimumHeight(500);
    m_videoWidget->setStyleSheet("background-color: black;");
    videoLayout->addWidget(m_videoWidget);
    
    mainLayout->addWidget(videoGroup, 1);
    
    // Status bar
    m_statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(m_statusLabel);
    
    // Connect camera manager to video widget
    m_cameraManager->getCaptureSession()->setVideoOutput(m_videoWidget);
}

void MainWindow::createConnections() {
    // Camera selection
    connect(m_cameraComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCameraSelectionChanged);
    
    // Start/Stop button
    connect(m_startStopButton, &QPushButton::clicked,
            this, &MainWindow::onStartStopClicked);
    
    // Snapshot button
    connect(m_snapshotButton, &QPushButton::clicked,
            this, &MainWindow::onSnapshotClicked);
    
    // Record button
    connect(m_recordButton, &QPushButton::clicked,
            this, &MainWindow::onRecordClicked);
    
    // Camera manager signals
    connect(m_cameraManager.get(), &CameraManager::cameraListUpdated,
            this, &MainWindow::onCameraListUpdated);
    
    connect(m_cameraManager.get(), &CameraManager::cameraStarted,
            this, &MainWindow::onCameraStarted);
    
    connect(m_cameraManager.get(), &CameraManager::cameraStopped,
            this, &MainWindow::onCameraStopped);
    
    connect(m_cameraManager.get(), &CameraManager::cameraError,
            this, &MainWindow::onCameraError);
    
    connect(m_cameraManager.get(), &CameraManager::recordingStarted,
            this, &MainWindow::onRecordingStarted);
    
    connect(m_cameraManager.get(), &CameraManager::recordingStopped,
            this, &MainWindow::onRecordingStopped);
    
    connect(m_cameraManager.get(), &CameraManager::recordingError,
            this, &MainWindow::onRecordingError);
}

void MainWindow::onCameraSelectionChanged(int index) {
    if (m_isRunning) {
        m_cameraManager->stopCamera();
        m_isRunning = false;
    }
    
    if (m_cameraManager->selectCamera(index)) {
        showStatusMessage("Camera selected: " + m_cameraComboBox->currentText());
    } else {
        showErrorMessage("Failed to select camera: " + m_cameraManager->getLastError());
    }
    
    updateUIState();
}

void MainWindow::onStartStopClicked() {
    if (m_isRunning) {
        m_cameraManager->stopCamera();
        m_isRunning = false;
    } else {
        // Check if camera is selected
        if (m_cameraComboBox->count() == 0) {
            QMessageBox::warning(this, "No Camera Available", 
                "No camera detected on this system.\n\n"
                "Please:\n"
                "• Connect a webcam\n"
                "• Check camera permissions in Settings > Privacy & Security > Camera\n"
                "• Ensure the camera driver is installed\n"
                "• Restart the application");
            return;
        }
        
        if (m_cameraComboBox->currentIndex() < 0) {
            QMessageBox::warning(this, "No Camera Selected", 
                "Please select a camera from the dropdown menu before starting.");
            return;
        }
        
        if (m_cameraManager->startCamera()) {
            m_isRunning = true;
        } else {
            QMessageBox::critical(this, "Camera Error", 
                "Failed to start camera: " + m_cameraManager->getLastError() + 
                "\n\nThe camera may be in use by another application.");
        }
    }
    
    updateUIState();
}

void MainWindow::onSnapshotClicked() {
    if (!m_isRunning) {
        showErrorMessage("Camera is not running. Start the camera first.");
        return;
    }
    
    QString snapshotPath = VideoRecorder::generateSnapshotPath();
    if (m_cameraManager->captureSnapshot(snapshotPath)) {
        showStatusMessage("Snapshot saved: " + snapshotPath);
    } else {
        showErrorMessage("Failed to capture snapshot: " + m_cameraManager->getLastError());
    }
}

void MainWindow::onRecordClicked() {
    if (!m_isRunning) {
        showErrorMessage("Camera is not running. Start the camera first.");
        return;
    }
    
    if (m_isRecording) {
        m_cameraManager->stopRecording();
        m_isRecording = false;
    } else {
        QString videoPath = VideoRecorder::generateVideoPath();
        if (m_cameraManager->startRecording(videoPath)) {
            m_isRecording = true;
        } else {
            showErrorMessage("Failed to start recording: " + m_cameraManager->getLastError());
        }
    }
    
    updateUIState();
}

void MainWindow::onCameraListUpdated(const QStringList &cameras) {
    m_cameraComboBox->clear();
    m_cameraComboBox->addItems(cameras);
    
    if (cameras.isEmpty()) {
        m_warningLabel->setText("⚠ WARNING: No camera detected! Please connect a webcam and check camera permissions.");
        m_warningLabel->setVisible(true);
        m_startStopButton->setEnabled(false);
    } else {
        m_warningLabel->setVisible(false);
        m_startStopButton->setEnabled(true);
    }
}

void MainWindow::onCameraStarted() {
    m_isRunning = true;
    updateUIState();
}

void MainWindow::onCameraStopped() {
    m_isRunning = false;
    m_isRecording = false;
    updateUIState();
}

void MainWindow::onCameraError(const QString &error) {
    showErrorMessage("Camera Error: " + error);
    m_isRunning = false;
    updateUIState();
}

void MainWindow::onRecordingStarted() {
    m_isRecording = true;
    updateUIState();
}

void MainWindow::onRecordingStopped() {
    m_isRecording = false;
    updateUIState();
}

void MainWindow::onRecordingError(const QString &error) {
    showErrorMessage("Recording Error: " + error);
    m_isRecording = false;
    updateUIState();
}

void MainWindow::updateUIState() {
    // Update Start/Stop button
    m_startStopButton->setText(m_isRunning ? "Stop" : "Start");
    m_startStopButton->setStyleSheet(m_isRunning ? "background-color: #ff6b6b;" : "");
    
    // Update Snapshot button
    m_snapshotButton->setEnabled(m_isRunning);
    
    // Update Record button
    m_recordButton->setText(m_isRecording ? "Stop Recording" : "Record");
    m_recordButton->setEnabled(m_isRunning);
    m_recordButton->setStyleSheet(m_isRecording ? "background-color: #ff6b6b;" : "");
    
    // Update recording indicator
    m_recordingIndicator->setText(m_isRecording ? "● RECORDING" : "");
    
    // Update camera combo box
    m_cameraComboBox->setEnabled(!m_isRunning);
}

void MainWindow::showStatusMessage(const QString &message, int duration) {
    m_statusLabel->setText(message);
    
    // Auto-clear after specified duration
    QTimer::singleShot(duration, this, [this]() {
        m_statusLabel->setText("Ready");
    });
}

void MainWindow::showErrorMessage(const QString &message) {
    m_statusLabel->setText("Error: " + message);
    m_statusLabel->setStyleSheet("color: red;");
    
    QTimer::singleShot(5000, this, [this]() {
        m_statusLabel->setText("Ready");
        m_statusLabel->setStyleSheet("");
    });
}
