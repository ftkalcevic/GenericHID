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

#ifndef LCDWIDGET_H
#define LCDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "lcdchar.h"
#include "datatypes.h"

class LCDWidget : public QWidget
{
    Q_OBJECT

public:
    LCDWidget(QWidget *parent = NULL);
    ~LCDWidget();
    void Write( int nRow, int nCol, const QString &s, bool bHighlight );
    void Erase( int nRow, int nCol, int nLen );
    void Clear();
    void setSize( int nRows, int nCols );
    void SetUserFont( byte index, const QVector<byte> &data );
    void setReadOnly( bool bReadOnly) { m_bReadOnly = bReadOnly; }
    void SetCursor( bool bEnable, bool bBlink );
    void LCDFunctionSet( int n );

signals:
    void write( int nRow, int nCol, const QString &s );
    void cursor( int nRow, int nCol, bool bEnable, bool bBlink );
    void clear();
    void functionSet(int n);

public slots:
    void onCursorTimer();

private:
    virtual void paintEvent( QPaintEvent * evnt );
    virtual void resizeEvent ( QResizeEvent * evnt );
    virtual int heightForWidth ( int w ) const;
    virtual QSize sizeHint() const;
    virtual void keyPressEvent( QKeyEvent * event );
    void MoveCursor();

    void InvalidateCells( int nRow, int nCol, int n );
    QRect CellQRect( int nRow, int nCol );

    QSize m_sizeTextArea;
    QImage m_imgLCD;

    QBrush m_bkgBrush;
    static QMap<int,LCDChar*> m_chars;
    int m_nRows;
    int m_nCols;
    int m_cursorPosRow;
    int m_cursorPosCol;
    bool m_bReadOnly;
    bool m_bCursorEnable;
    bool m_bCursorBlink;
    bool m_bBlinkOn;
    QTimer *m_cursorTimer;
};

#endif // LCDWIDGET_H
