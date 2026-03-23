#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVideoWidget>
#include <memory>

class CameraManager;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCameraSelectionChanged(int index);
    void onStartStopClicked();
    void onSnapshotClicked();
    void onRecordClicked();
    void onCameraListUpdated(const QStringList &cameras);
    void onCameraStarted();
    void onCameraStopped();
    void onCameraError(const QString &error);
    void onRecordingStarted();
    void onRecordingStopped();
    void onRecordingError(const QString &error);

private:
    void setupUI();
    void createConnections();
    void updateUIState();
    void showStatusMessage(const QString &message, int duration = 3000);
    void showErrorMessage(const QString &message);

    // UI Components
    QLabel *m_warningLabel;
    QComboBox *m_cameraComboBox;
    QPushButton *m_startStopButton;
    QPushButton *m_snapshotButton;
    QPushButton *m_recordButton;
    QVideoWidget *m_videoWidget;
    QLabel *m_statusLabel;
    QLabel *m_recordingIndicator;

    // Manager
    std::unique_ptr<CameraManager> m_cameraManager;

    // State
    bool m_isRunning;
    bool m_isRecording;
};

#endif // MAINWINDOW_H
