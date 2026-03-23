#include "cameramanager.h"
#include <QMediaDevices>
#include <QVideoFrame>
#include <QDateTime>
#include <QStandardPaths>
#include <QUrl>
#include <QDebug>

CameraManager::CameraManager(QObject *parent)
    : QObject(parent)
    , m_captureSession(std::make_unique<QMediaCaptureSession>())
    , m_selectedCameraIndex(-1)
{
    initializeCameras();
}

CameraManager::~CameraManager() {
    if (m_camera) {
        m_camera->stop();
    }
}

void CameraManager::initializeCameras() {
    const auto cameras = QMediaDevices::videoInputs();
    
    qDebug() << "Camera Detection Debug:";
    qDebug() << "Total cameras found:" << cameras.size();
    
    if (cameras.isEmpty()) {
        m_lastError = "No cameras detected on this system. Please check camera permissions and connections.";
        qDebug() << "ERROR:" << m_lastError;
        emit cameraError(m_lastError);
    }
    
    for (int i = 0; i < cameras.size(); ++i) {
        const auto &cameraDevice = cameras[i];
        QString cameraInfo = QString("Camera %1: %2").arg(i).arg(cameraDevice.description());
        qDebug() << cameraInfo;
        m_availableCameras.append(cameraDevice.description());
    }
    
    emit cameraListUpdated(m_availableCameras);
}

QStringList CameraManager::getAvailableCameras() const {
    return m_availableCameras;
}

bool CameraManager::selectCamera(int index) {
    const auto cameras = QMediaDevices::videoInputs();
    
    if (cameras.isEmpty()) {
        m_lastError = "No cameras available on this system";
        emit cameraError(m_lastError);
        return false;
    }
    
    if (index < 0 || index >= cameras.size()) {
        m_lastError = "Invalid camera index";
        emit cameraError(m_lastError);
        return false;
    }
    
    if (m_camera && m_camera->isActive()) {
        m_camera->stop();
    }
    
    m_camera = std::make_unique<QCamera>(cameras[index]);
    m_selectedCameraIndex = index;
    
    connect(m_camera.get(), &QCamera::errorOccurred, this, &CameraManager::onCameraError);
    
    m_captureSession->setCamera(m_camera.get());
    
    // Setup video sink for frame capture
    m_videoSink = std::make_unique<QVideoSink>();
    m_captureSession->setVideoSink(m_videoSink.get());
    
    // Setup recorder
    m_recorder = std::make_unique<QMediaRecorder>();
    m_captureSession->setRecorder(m_recorder.get());
    
    connect(m_recorder.get(), &QMediaRecorder::errorOccurred,
            this, &CameraManager::onRecorderError);
    
    return true;
}

bool CameraManager::startCamera() {
    if (!m_camera) {
        m_lastError = "No camera selected";
        emit cameraError(m_lastError);
        return false;
    }
    
    m_camera->start();
    emit cameraStarted();
    return true;
}

bool CameraManager::stopCamera() {
    if (m_camera) {
        m_camera->stop();
        emit cameraStopped();
        return true;
    }
    return false;
}

bool CameraManager::isRunning() const {
    return m_camera && m_camera->isActive();
}

bool CameraManager::captureSnapshot(const QString &filePath) {
    if (!m_videoSink) {
        m_lastError = "Video sink not available";
        emit cameraError(m_lastError);
        return false;
    }
    
    QVideoFrame frame = m_videoSink->videoFrame();
    if (!frame.isValid()) {
        m_lastError = "Failed to capture frame";
        emit cameraError(m_lastError);
        return false;
    }
    
    QImage image = frame.toImage();
    if (image.isNull()) {
        m_lastError = "Failed to convert frame to image";
        emit cameraError(m_lastError);
        return false;
    }
    
    if (!image.save(filePath)) {
        m_lastError = "Failed to save snapshot to: " + filePath;
        emit cameraError(m_lastError);
        return false;
    }
    
    return true;
}

bool CameraManager::startRecording(const QString &filePath) {
    if (!m_recorder) {
        m_lastError = "Recorder not initialized";
        emit recordingError(m_lastError);
        return false;
    }
    
    m_recorder->setOutputLocation(QUrl::fromLocalFile(filePath));
    m_recorder->record();
    
    if (m_recorder->recorderState() != QMediaRecorder::RecordingState) {
        m_lastError = "Failed to start recording";
        emit recordingError(m_lastError);
        return false;
    }
    
    emit recordingStarted();
    return true;
}

bool CameraManager::stopRecording() {
    if (!m_recorder) {
        return false;
    }
    
    m_recorder->stop();
    emit recordingStopped();
    return true;
}

bool CameraManager::isRecording() const {
    return m_recorder && m_recorder->recorderState() == QMediaRecorder::RecordingState;
}

QMediaCaptureSession *CameraManager::getCaptureSession() const {
    return m_captureSession.get();
}

QCamera *CameraManager::getCurrentCamera() const {
    return m_camera.get();
}

QString CameraManager::getLastError() const {
    return m_lastError;
}

void CameraManager::onCameraError(QCamera::Error error, const QString &errorString) {
    m_lastError = errorString;
    emit cameraError(m_lastError);
}

void CameraManager::onRecorderError(QMediaRecorder::Error error, const QString &errorString) {
    Q_UNUSED(error);
    m_lastError = errorString;
    emit recordingError(m_lastError);
}
