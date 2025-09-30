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

QString CleanName( const QString &s )
{
    QString sRet = s.trimmed();
    sRet.replace( "[", "" );
    sRet.replace( "]", "" );
    return sRet;
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
	ui.tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString("Row %1").arg(i)) );

    for ( int i = 0; i < m_nColumns+1; i++ )
	ui.tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("Col %1").arg(i)) );

    for ( int r = 0; r < m_nRows; r++ )
	for ( int c = 0; c < m_nColumns; c++ )
	    ui.tableWidget->setItem( r, c, new QTableWidgetItem("") );

    ui.tableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::Interactive );
    ui.tableWidget->horizontalHeader()->resizeSections( QHeaderView::Stretch );
    update();
}

void KeyMatrixNameDlg::setValue( const QString &sNames )
{
    // [r,c]String...
    QRegularExpression rx("\\[(\\d+),(\\d+)\\]([^\\[]+)");
    QRegularExpressionMatch match = rx.match(sNames);

    for ( int i = 0; match.hasCaptured(i); i++ )
    {
	int nRow = match.capturedView(1).toInt();
	int nCol = match.capturedView(2).toInt();
	QString sName = match.capturedView(3).toString();

	if ( nRow >= 0 && nRow < m_nRows &&
	     nCol >= 0 && nCol < m_nColumns )
	{
	    ui.tableWidget->item(nRow,nCol)->setText( CleanName(sName) );
	}

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
	    s = CleanName(s);
	    if ( ! s.isEmpty() )
		sNames += QString("[%1,%2]%3").arg(r).arg(c).arg(s);
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
