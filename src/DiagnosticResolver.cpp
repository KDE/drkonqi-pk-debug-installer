// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
// SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>

#include "DiagnosticResolver.h"

#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryFile>

#include <PackageKit/Daemon>

#include "Debug.h"
#include "DebugPackage.h"

// NB: none of the strings are i18n'd because we need to understand them in
//   bug reports. They are here for our benefit, not the user's.

DiagnosticResolver::DiagnosticResolver(std::shared_ptr<File> file, QObject *parent)
    : QObject(parent)
    , m_file(std::move(file))
{
}

void DiagnosticResolver::resolve()
{
    auto proc = new QProcess(this);
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->setInputChannelMode(QProcess::ManagedInputChannel);

    connect(proc, &QProcess::started, this, [this, proc]() {
        QFile script(QStringLiteral(":/Diagnostic.sh"));
        if (!script.open(QFile::ReadOnly)) {
            m_data += QStringLiteral("Failed to open diagnostic script asset.");
            proc->closeWriteChannel();
            proc->kill();
            return;
        }
        proc->write(script.readAll());
        proc->closeWriteChannel();
    });

    connect(proc, &QProcess::readyReadStandardOutput, this, [this, proc]() {
        m_data += QString::fromUtf8(proc->readAll());
    });

    connect(proc, &QProcess::finished, this, [this, proc]() {
        proc->deleteLater();

        // Read remaining data in case there is any.
        m_data += QString::fromUtf8(proc->readAll());
        Q_EMIT finished();
    });

    proc->start(QStringLiteral("/bin/sh"), {QStringLiteral("-s"), m_file->path(), QFileInfo(m_file->path()).canonicalFilePath()});
}

QString DiagnosticResolver::data() const
{
    return m_data;
}
