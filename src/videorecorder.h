#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QString>
#include <QDateTime>
#include <QStandardPaths>

class VideoRecorder {
public:
    static QString generateSnapshotPath();
    static QString generateVideoPath();
    static QString getOutputDirectory();
    static bool ensureOutputDirectoryExists();
};

#endif // VIDEORECORDER_H
