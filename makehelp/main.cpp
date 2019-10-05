// generichid, DIY HID device 
// Copyright (C) 2009, Frank Tkalcevic, www.franksworkshop.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"


const char *webText = \
                          "%1\n";


static void ProcessHelp( const QString &sDestDir, const QString &sFile )
{
    QString sError;

    QFile file(sFile);
    file.open(QIODevice::ReadOnly);
    QTextStream text( &file );

    QString sWeb = text.readAll();
    file.close();

    // Find the body
    QRegExp ex1("<body>(.*)</body>");
    int pos = ex1.indexIn( sWeb, 0 );
    if ( pos == -1 )
    {
        qCritical() << "Can't find <body> in file " << sFile;
        return;
    }
    QString sBody = ex1.cap(1);

    // Find the title
    QRegExp ex2("<title>(.*)</title>");
    pos = ex2.indexIn( sWeb, 0 );
    if ( pos == -1 )
    {
        qCritical() << "Can't find <title> in file " << sFile;
        return;
    }
    QString sTitle = ex2.cap(1);

    // Remove the <h1> heading from body
    QRegExp ex3("<h1>(.*)</h1>");
    pos = ex3.indexIn( sBody, 0 );
    if ( pos == -1 )
    {
        qCritical() << "Can't find <h1></h1> in body in file " << sFile;
        return;
    }
    QString sHeading = ex3.cap(1);
    sBody.replace( ex3, "" );

    //QRegExp rxLinks("\\<a.*href=\"([^/:#]*\\.htm)\"\\>");
    QRegExp rxLinks( "(^.*\\<a.*href=\"[^/:#]*)(\\.htm)(\"\\>.*)" );

    // Process body
    QStringList lines = sBody.split("\n");
    sBody = "";
    foreach( QString line, lines)
    {
        // Remove leading whitespace
        line = line.trimmed();

        // Convert local links to links to md documents
        int pos = rxLinks.indexIn(line,0);
        if ( pos != -1 )
        {
            line = rxLinks.cap(1) + rxLinks.cap(3);
        }
        sBody.append(line);
        sBody.append("\n");
    }
    QString sNewHtml = QString( webText).arg(sBody);


    QFileInfo fileInfo( sFile );
    QString sDestFile = sDestDir + "/" + fileInfo.baseName() + ".md";

    QFile fileOut( sDestFile );
    fileOut.open(QIODevice::WriteOnly);
    QTextStream textOut(&fileOut);
    textOut << sNewHtml;
    fileOut.close();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList args = a.arguments();
    if ( args.count() > 2 )
    {
        args.takeFirst();

        QString sDestination = args.takeFirst();
        QFileInfo destInfo( sDestination );
        if ( !destInfo.isDir() )
        {
            qCritical() << "Argument 1, destination direction, is not a valid directory ('" << sDestination << "')";
            return 0;
        }

        foreach( QString s, args )
            ProcessHelp( sDestination, s );
    }
    else
        qCritical() << "Usage: makehelp destdir files ...";
    return 0;
}
