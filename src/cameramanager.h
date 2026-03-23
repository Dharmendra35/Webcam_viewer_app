#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QMediaCaptureSession>
#include <QCamera>
#include <QMediaRecorder>
#include <QVideoSink>
#include <memory>

class CameraManager : public QObject {
    Q_OBJECT

public:
    explicit CameraManager(QObject *parent = nullptr);
    ~CameraManager();

    // Camera operations
    QStringList getAvailableCameras() const;
    bool selectCamera(int index);
    bool startCamera();
    bool stopCamera();
    bool isRunning() const;
    
    // Snapshot and recording
    bool captureSnapshot(const QString &filePath);
    bool startRecording(const QString &filePath);
    bool stopRecording();
    bool isRecording() const;
    
    // Getters
    QMediaCaptureSession *getCaptureSession() const;
    QCamera *getCurrentCamera() const;
    QString getLastError() const;

signals:
    void cameraListUpdated(const QStringList &cameras);
    void cameraStarted();
    void cameraStopped();
    void cameraError(const QString &error);
    void recordingStarted();
    void recordingStopped();
    void recordingError(const QString &error);

private slots:
    void onCameraError(QCamera::Error error, const QString &errorString);
    void onRecorderError(QMediaRecorder::Error error, const QString &errorString);

private:
    void initializeCameras();
    
    std::unique_ptr<QMediaCaptureSession> m_captureSession;
    std::unique_ptr<QCamera> m_camera;
    std::unique_ptr<QMediaRecorder> m_recorder;
    std::unique_ptr<QVideoSink> m_videoSink;
    
    QStringList m_availableCameras;
    QString m_lastError;
    int m_selectedCameraIndex;
};

#endif // CAMERAMANAGER_H
