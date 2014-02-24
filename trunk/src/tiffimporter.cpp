/*
    YAGF - cuneiform and tesseract OCR graphical front-ends
    Copyright (C) 2009-2014 Andrei Borovsky <anb@symmetrica.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "tiffimporter.h"
#include "settings.h"
#include <QProcess>
#include <QDir>

TiffImporter::TiffImporter(const QString &fileName, QObject *parent) :
    QObject(parent), tiffName(fileName)
{
}

void TiffImporter::exec()
{
    QString wd = Settings::instance()->workingDir();
    QDir dir;
    dir.setPath(wd);
    QStringList sl, pf, nf;
    sl << "t*_out*.jpg";
    pf = dir.entryList(sl);
    QProcess proc;
    proc.setEnvironment(QProcess::systemEnvironment());
    proc.setWorkingDirectory(wd);
    QString cmd = QString("convert %1  -units PixelsPerInch -resize 4000x4000 -units PixelsPerInch -density 300  t%2_out.jpg").arg(tiffName).arg(Settings::instance()->uniqueSeed());
    proc.start(cmd);
    proc.waitForFinished();
    if (proc.exitStatus() != QProcess::NormalExit) {
        emit error();
        return;
    }
    nf = dir.entryList(sl);

    foreach(QString s, nf) {
        if (!pf.contains(s))
            files.append(wd+s);
    }
    if (files.count())
        emit finished(files);
    else
        emit error();
}

QStringList TiffImporter::extractedFiles()
{
    return files;
}
