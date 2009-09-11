#include "stdafx.h"


const char *webText = \
"<html>\n" \
    "<head>\n" \
	"<title>%1</title>\n" \
	"<link rel=\"stylesheet\" type=\"text/css\" href=\"/Site.css\" />\n" \
	"<script type=\"text/javascript\" src=\"/menu.js\"></script>\n" \
    "</head>\n" \
    "<body>\n" \
	"<div id=\"mainContainer\">\n" \
	    "<div id=\"topBar\">\n" \
		"<h1 align=\"center\">%2</h1>\n" \
	    "</div>\n" \
	    "<div id=\"leftColumn\">\n" \
		"<!-- #include file=\"/menu.html\" -->\n" \
	    "</div>\n"\
	    "<div id=\"mainContent\">\n" \
		"%3\n" \
	    "</div>\n" \
	"</div>\n" \
	"<!-- #include file=\"/analytics.html\" -->\n" \
    "</body>\n" \
"</html>\n";


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
	fputs( "Can't find <body>", stderr );
	return;
    }
    QString sBody = ex1.cap(1);

    // Find the title
    QRegExp ex2("<title>(.*)</title>");
    pos = ex2.indexIn( sWeb, 0 );
    if ( pos == -1 )
    {
	fputs( "Can't find <title>", stderr );
	return;
    }
    QString sTitle = ex2.cap(1);

    // Remove the <h1> heading from body
    QRegExp ex3("<h1>(.*)</h1>");
    pos = ex3.indexIn( sBody, 0 );
    if ( pos == -1 )
    {
	fputs( "Can't find <h1></h1> in body", stderr );
	return;
    }
    QString sHeading = ex3.cap(1);
    sBody.replace( ex3, "" );

    QString sNewHtml = QString( webText).arg( sTitle ).arg( sHeading ).arg(sBody);

 
    QFileInfo fileInfo( sFile );
    QString sDestFile = sDestDir + "/" + fileInfo.fileName();

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
	    fputs( QString("Argument 1, destination direction, is not a valid directory ('%1')").arg(sDestination).toAscii().constData(), stderr );
	    return 0;
	}

	foreach( QString s, args )
	    ProcessHelp( sDestination, s );
    }
    else
	fputs( "Usage: makehelp destdir files ...", stderr );
    return 0;
}
