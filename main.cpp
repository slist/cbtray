// Copyright 2023 VMware, Inc.
// SPDX-License-Identifier: MIT

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>

#include "cbtray.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(cbtray);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    cbtray tray;
    tray.show();

    return app.exec();
}

