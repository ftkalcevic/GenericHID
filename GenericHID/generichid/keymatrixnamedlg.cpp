#include "stdafx.h"
#include "keymatrixnamedlg.h"

KeyMatrixNameDlg::KeyMatrixNameDlg(QWidget *parent)
: QDialog(parent)
, m_nRows(1)
, m_nColumns(1)
{
    ui.setupUi(this);
}

KeyMatrixNameDlg::~KeyMatrixNameDlg()
{

}

void KeyMatrixNameDlg::setRC( int nRows, int nColumns )
{
    m_nRows = nRows;
    m_nColumns = nColumns;

    // Build the table
    ui.tableWidget->clear();

    ui.tableWidget->setRowCount(m_nRows);
    ui.tableWidget->setColumnCount(m_nColumns);

    for ( int i = 0; i < m_nRows; i++ )
	ui.tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString("Row %1").arg(i+1)) );

    for ( int i = 0; i < m_nColumns+1; i++ )
	ui.tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("Col %1").arg(i+1)) );

    for ( int r = 0; r < m_nRows; r++ )
	for ( int c = 0; c < m_nColumns; c++ )
	    ui.tableWidget->setItem( r, c, new QTableWidgetItem("") );

    ui.tableWidget->horizontalHeader()->setResizeMode( QHeaderView::Interactive );
    ui.tableWidget->horizontalHeader()->resizeSections( QHeaderView::Stretch );
    update();
}

void KeyMatrixNameDlg::setValue( const QString &sNames )
{
    // [r,c]String...
    QRegExp rx("\\[(\\d+),(\\d+)\\]([^\\[]+)");
    int pos = 0;

    while ((pos = rx.indexIn(sNames, pos)) != -1) 
    {
	int nRow = rx.cap(1).toInt() - 1;
	int nCol = rx.cap(2).toInt() - 1;
	QString sName = rx.cap(3);

	if ( nRow >= 0 && nRow < m_nRows &&
	     nCol >= 0 && nCol < m_nColumns )
	{
	    ui.tableWidget->item(nRow,nCol)->setText( sName );
	}

	pos += rx.matchedLength();
    }
    ui.tableWidget->horizontalHeader()->resizeSections( QHeaderView::Stretch );
}

QString KeyMatrixNameDlg::value() const
{
    QString sNames;
    // [r,c]String...

    for ( int r = 0; r < m_nRows; r++ )
	for ( int c = 0; c < m_nColumns; c++ )
	{
	    QString s = ui.tableWidget->item(r,c)->text();
	    s = s.trimmed();
	    if ( ! s.isEmpty() )
		sNames += QString("[%1,%2]%3").arg(r+1).arg(c+1).arg(s);
	}
    return sNames;
}


void KeyMatrixNameDlg::resizeEvent( QResizeEvent * event )
{
    event->ignore();
    ui.tableWidget->horizontalHeader()->resizeSections( QHeaderView::Stretch );
}


void KeyMatrixNameDlg::showEvent( QShowEvent * event )
{
    event->ignore();
    ui.tableWidget->horizontalHeader()->resizeSections( QHeaderView::Stretch );
}
