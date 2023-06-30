// Copyright 2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#ifndef CBTRAY_H
#define CBTRAY_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

class cbtray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit cbtray(QObject *parent = nullptr);

private:
    QAction *cbAction;
    QAction *logAction;
    QAction *resetAction;
    QAction *syslogAction;
    QAction *quitAction;
    QMenu *trayIconMenu;
    QString syslog_filename;
    qint64 syslog_filesize;
    qint64 syslog_line_number; // Last syslog line number parsed

public slots:
    void parse_syslog();
    void open_cb();
    void open_syslog();
    void reset();
};

#endif // CBTRAY_H
