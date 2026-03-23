#include "videorecorder.h"
#include <QDir>

QString VideoRecorder::generateSnapshotPath() {
    QString dir = getOutputDirectory();
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz");
    return dir + "/snapshot_" + timestamp + ".png";
}

QString VideoRecorder::generateVideoPath() {
    QString dir = getOutputDirectory();
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    return dir + "/video_" + timestamp + ".mp4";
}

QString VideoRecorder::getOutputDirectory() {
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WebcamViewer";
}

bool VideoRecorder::ensureOutputDirectoryExists() {
    QDir dir(getOutputDirectory());
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}
