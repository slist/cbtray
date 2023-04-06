// Copyright 2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include "cbtray.h"

static const QString syslog_service_name = "cb-psc-sensor[";

cbtray::cbtray(QObject * parent)
    : QSystemTrayIcon{parent},
      syslog_filesize(0),
      syslog_line_number(0)
{
    setIcon(QIcon(":/images/cbtray.png"));

    cbAction = new QAction("VMware Carbon Black Cloud", this);
    connect(cbAction, SIGNAL(triggered()), this, SLOT(open_cb()));

    logAction = new QAction("", this);
    logAction->setEnabled(false);

    resetAction = new QAction("&Reset alerts", this);
    connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));

    if (QFile::exists("/var/log/syslog")) {
        syslog_filename = "/var/log/syslog";
    } else if (QFile::exists("/var/log/messages")) {
        syslog_filename = "/var/log/messages";
    } else {
        syslog_filename = "no /var/log/syslog and no /var/log/messages on this Linux!";
        setIcon(QIcon(":/images/cbtray_malware.png"));
    }

    syslogAction = new QAction("&Open " + syslog_filename, this);
    connect(syslogAction, SIGNAL(triggered()), this, SLOT(open_syslog()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu();
    trayIconMenu->addAction(cbAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(logAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(resetAction);

    if (syslog_filename != "/var/log/messages") {
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(syslogAction);
    }

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    setContextMenu(trayIconMenu);

    const QFileInfo info(syslog_filename);
    if (! info.isReadable()) {
        qDebug() << syslog_filename << " is not readable";
        logAction->setText(syslog_filename + " is not readable\n");
    }

    QTimer *timer = new QTimer(this);
    timer->setInterval(2000); // Every 2 seconds
    connect(timer, SIGNAL(timeout()), this, SLOT(parse_syslog()));
    timer->start();
}

void cbtray::parse_syslog()
{
    qDebug() << "Open syslog";
    QFile inputFile(syslog_filename);
    qint64 filesize = inputFile.size();

    if (filesize == syslog_filesize) {
        qDebug("No changes in syslog");
        return;
    } else if (filesize < syslog_filesize) {
        qDebug("syslog file is shorter. It has been rotated");
        syslog_line_number=0;
    }
    syslog_filesize = filesize;

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        qint64 line_number=0;

        while (!in.atEnd())
        {
            QString line_string = in.readLine();
            if (line_number++ >= syslog_line_number) { // parse only new lines
                syslog_line_number = line_number;
                if (line_string.contains(syslog_service_name)) {
                    qDebug() << line_string;

                    int i = line_string.indexOf(syslog_service_name);
                    line_string = line_string.mid(i+ syslog_service_name.length());

                    i = line_string.indexOf(" ");
                    line_string = line_string.mid(i+1);
                    qDebug() << line_string;

                    if (syslog_filename != "/var/log/messages") {
                        logAction->setText(logAction->text() + line_string + "\n\n");
                    }

                    showMessage("VMware Carbon Black Cloud alert:", line_string, QSystemTrayIcon::Warning);
                    setIcon(QIcon(":/images/cbtray_malware.png"));
                    return;
                }
            }
        }
        inputFile.close();
    }
}

void cbtray::open_cb()
{
    QDesktopServices::openUrl(QUrl("https://carbonblack.vmware.com"));
}

void cbtray::open_syslog()
{
    QDesktopServices::openUrl(QUrl("file://" + syslog_filename));
}

void cbtray::reset()
{
    setIcon(QIcon(":/images/cbtray.png")); // Restore normal color
    logAction->setText("");
}
