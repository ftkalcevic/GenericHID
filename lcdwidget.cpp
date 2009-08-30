#include "stdafx.h"
#include "lcdwidget.h"

const int X_BORDER = 2;
const int Y_BORDER = 2;
const int X_SPACING = 1;
const int Y_SPACING = 1;

QMap<int,LCDChar*> LCDWidget::m_chars;

LCDWidget::LCDWidget(QWidget *parent)
    : QWidget(parent)
    , m_nRows( 4 )  // some default
    , m_nCols( 20 ) // some default
    , m_bReadOnly( false )
{
    m_bkgBrush = QBrush(QColor(134, 154, 58));
    if ( m_chars.isEmpty() )
    {
        m_chars[32] = new LCDChar( 0,  0,  0,  0,  0); //
        m_chars[33] = new LCDChar( 0,  0, 79,  0,  0); // !
        m_chars[34] = new LCDChar( 0,  7,  0,  7,  0); // "
        m_chars[35] = new LCDChar( 20,127, 20,127, 20); // #
        m_chars[36] = new LCDChar( 36, 42,127, 42, 18); // $
        m_chars[37] = new LCDChar( 35, 19,  8,100, 98); // %
        m_chars[38] = new LCDChar( 54, 73, 85, 34, 80); // &
        m_chars[39] = new LCDChar( 0,  5,  3,  0,  0); // '
        m_chars[40] = new LCDChar( 0, 28, 34, 65,  0); // (
        m_chars[41] = new LCDChar( 0, 65, 34, 28,  0); // )
        m_chars[42] = new LCDChar( 20,  8, 62,  8, 20); // *
        m_chars[43] = new LCDChar( 8,  8, 62,  8,  8); // +
        m_chars[44] = new LCDChar( 0, 80, 48,  0,  0); // ,
        m_chars[45] = new LCDChar( 8,  8,  8,  8,  8); // -
        m_chars[46] = new LCDChar( 0, 96, 96,  0,  0); // .
        m_chars[47] = new LCDChar( 32, 16,  8,  4,  2); // /
        m_chars[48] = new LCDChar( 62, 81, 73, 69, 62); // 0
        m_chars[49] = new LCDChar( 0, 66,127, 64,  0); // 1
        m_chars[50] = new LCDChar( 66, 97, 81, 73, 70); // 2
        m_chars[51] = new LCDChar( 33, 65, 69, 75, 49); // 3
        m_chars[52] = new LCDChar( 24, 20, 18,127, 16); // 4
        m_chars[53] = new LCDChar( 39, 69, 69, 69, 57); // 5
        m_chars[54] = new LCDChar( 60, 74, 73, 73, 48); // 6
        m_chars[55] = new LCDChar( 1,113,  9,  5,  3); // 7
        m_chars[56] = new LCDChar( 54, 73, 73, 73, 54); // 8
        m_chars[57] = new LCDChar( 6, 73, 73, 41, 30); // 9
        m_chars[58] = new LCDChar( 0, 54, 54,  0,  0); // :
        m_chars[59] = new LCDChar( 0, 86, 54,  0,  0); // ;
        m_chars[60] = new LCDChar( 8, 20, 34, 65,  0); // <
        m_chars[61] = new LCDChar( 20, 20, 20, 20, 20); // =
        m_chars[62] = new LCDChar( 0, 65, 34, 20,  8); // >
        m_chars[63] = new LCDChar( 2,  1, 81,  9,  6); // ?
        m_chars[64] = new LCDChar( 50, 73,121, 65, 62); // @
        m_chars[65] = new LCDChar( 126, 17, 17, 17,126); // A
        m_chars[66] = new LCDChar( 127, 73, 73, 73, 54); // B
        m_chars[67] = new LCDChar( 62, 65, 65, 65, 34); // C
        m_chars[68] = new LCDChar( 127, 65, 65, 34, 28); // D
        m_chars[69] = new LCDChar( 127, 73, 73, 73, 65); // E
        m_chars[70] = new LCDChar( 127,  9,  9,  9,  1); // F
        m_chars[71] = new LCDChar( 62, 65, 73, 73,122); // G
        m_chars[72] = new LCDChar( 127,  8,  8,  8,127); // H
        m_chars[73] = new LCDChar( 0, 65,127, 65,  0); // I
        m_chars[74] = new LCDChar( 32, 64, 65, 63,  1); // J
        m_chars[75] = new LCDChar( 127,  8, 20, 34, 65); // K
        m_chars[76] = new LCDChar( 127, 64, 64, 64, 64); // L
        m_chars[77] = new LCDChar( 127,  2, 12,  2,127); // M
        m_chars[78] = new LCDChar( 127,  4,  8, 16,127); // N
        m_chars[79] = new LCDChar( 62, 65, 65, 65, 62); // O
        m_chars[80] = new LCDChar( 127,  9,  9,  9,  6); // P
        m_chars[81] = new LCDChar( 62, 65, 81, 33, 94); // Q
        m_chars[82] = new LCDChar( 127,  9, 25, 41, 70); // R
        m_chars[83] = new LCDChar( 70, 73, 73, 73, 49); // S
        m_chars[84] = new LCDChar( 1,  1,127,  1,  1); // T
        m_chars[85] = new LCDChar( 63, 64, 64, 64, 63); // U
        m_chars[86] = new LCDChar( 31, 32, 64, 32, 31); // V
        m_chars[87] = new LCDChar( 63, 64, 48, 64, 63); // W
        m_chars[88] = new LCDChar( 99, 20,  8, 20, 99); // X
        m_chars[89] = new LCDChar( 7,  8,112,  8,  7); // Y
        m_chars[90] = new LCDChar( 97, 81, 73, 69, 67); // Z
        m_chars[91] = new LCDChar( 0,127, 65, 65,  0); // [
        m_chars[92] = new LCDChar( 2,  4,  8, 16, 32); // backslash 
        m_chars[93] = new LCDChar( 0, 65, 65,127,  0); // ]
        m_chars[94] = new LCDChar( 4,  2,  1,  2,  4); // ^
        m_chars[95] = new LCDChar( 64, 64, 64, 64, 64); // _
        m_chars[96] = new LCDChar( 0,  1,  2,  4,  0); // `
        m_chars[97] = new LCDChar( 32, 84, 84, 84,120); // a
        m_chars[98] = new LCDChar( 127, 72, 68, 68, 56); // b
        m_chars[99] = new LCDChar( 56, 68, 68, 68, 32); // c
        m_chars[100] = new LCDChar( 56, 68, 68, 72,127); // d
        m_chars[101] = new LCDChar( 56, 84, 84, 84, 24); // e
        m_chars[102] = new LCDChar( 8,126,  9,  1,  2); // f
        m_chars[103] = new LCDChar( 12, 82, 82, 82, 62); // g
        m_chars[104] = new LCDChar( 127,  8,  4,  4,120); // h
        m_chars[105] = new LCDChar( 0, 68,125, 64,  0); // i
        m_chars[106] = new LCDChar( 32, 64, 68, 61,  0); // j
        m_chars[107] = new LCDChar( 127, 16, 40, 68,  0); // k
        m_chars[108] = new LCDChar( 0, 65,127, 64,  0); // l
        m_chars[109] = new LCDChar( 124,  4, 24,  4,120); // m
        m_chars[110] = new LCDChar( 124,  8,  4,  4,120); // n
        m_chars[111] = new LCDChar( 56, 68, 68, 68, 56); // o
        m_chars[112] = new LCDChar( 124, 20, 20, 20,  8); // p
        m_chars[113] = new LCDChar( 8, 20, 20, 24,124); // q
        m_chars[114] = new LCDChar( 124,  8,  4,  4,  8); // r
        m_chars[115] = new LCDChar( 72, 84, 84, 84, 32); // s
        m_chars[116] = new LCDChar( 4, 63, 68, 64, 32); // t
        m_chars[117] = new LCDChar( 60, 64, 64, 32,124); // u
        m_chars[118] = new LCDChar( 28, 32, 64, 32, 28); // v
        m_chars[119] = new LCDChar( 60, 64, 48, 64, 60); // w
        m_chars[120] = new LCDChar( 68, 40, 16, 40, 68); // x
        m_chars[121] = new LCDChar( 12, 80, 80, 80, 60); // y
        m_chars[122] = new LCDChar( 68,100, 84, 76, 68); // z
        m_chars[123] = new LCDChar( 0,  8, 54, 65,  0); // {
        m_chars[124] = new LCDChar( 0,  0,127,  0,  0); // |
        m_chars[125] = new LCDChar( 0, 65, 54,  8,  0); // }
        m_chars[126] = new LCDChar( 8,  8, 42, 28,  8); // ~
    }   
    setSize( m_nRows, m_nCols );
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHeightForWidth( true );
    setSizePolicy( policy );

    setFocusPolicy( Qt::StrongFocus );
    m_cursorPosRow = 0;
    m_cursorPosCol = 0;
}       
        
LCDWidget::~LCDWidget()
{
}


void LCDWidget::setSize( int nRows, int nCols )
{
    m_nRows = nRows;
    m_nCols = nCols;

    m_sizeTextArea = QSize( 2*X_BORDER + m_nCols * (LCDChar::PIXELS_X+X_SPACING)-X_SPACING, 2*Y_BORDER + m_nRows * (LCDChar::PIXELS_Y + Y_SPACING) - Y_SPACING );
    m_sizeTextArea *= LCDChar::PIXEL_SIZE;
    m_imgLCD = QImage(m_sizeTextArea, QImage::Format_ARGB32 );
    Clear();
}

void LCDWidget::paintEvent( QPaintEvent *  )
{
    QImage img = m_imgLCD.scaled( size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    QPainter p2( this );
    p2.setRenderHint( QPainter::HighQualityAntialiasing, true );
    p2.setRenderHint( QPainter::SmoothPixmapTransform, true );
    p2.drawImage( QPoint(0,0), img );

    if ( !m_bReadOnly )
    {
	QRectF rcCursorCell( CellQRect(m_cursorPosRow, m_cursorPosCol) );
	QTransform transform;
	double scale = (double)img.width() / (double)m_imgLCD.width();
	transform.scale( scale, scale );
	rcCursorCell = transform.mapRect( rcCursorCell );
	p2.fillRect( rcCursorCell, Qt::black );
    }

    if ( hasFocus() )
    {
	p2.drawRect( img.rect().adjusted(0,0,-1,-1) );
    }
}

QRect LCDWidget::CellQRect( int nRow, int nCol )
{
    return QRect( QPoint( (X_BORDER + nCol * (LCDChar::PIXELS_X+X_SPACING) - X_SPACING) * LCDChar::PIXEL_SIZE, (Y_BORDER + nRow * (LCDChar::PIXELS_Y+Y_SPACING) - Y_SPACING) * LCDChar::PIXEL_SIZE ),
                   QSize( (LCDChar::PIXELS_X) * LCDChar::PIXEL_SIZE, (LCDChar::PIXELS_Y) * LCDChar::PIXEL_SIZE ) );
}



void LCDWidget::Write( int nRow, int nCol, const QString &str, bool bHighlight )
{
    if ( nRow < 0 || nRow >= m_nRows )
    {
        assert( false );
        return;
    }
    if ( nCol < 0 || nCol >= m_nCols )
    {
        assert( false );
        return;
    }
    QString s = str;
    if ( nCol + s.length() >= m_nCols )
        s = s.mid( 0, m_nCols - nCol );

    QPainter p( &m_imgLCD );

    for ( int c = 0; c < s.length(); c++ )
    {
        QChar data = s[c];
        if ( m_chars.contains(data.toAscii()) )
        {
            QRect rc = CellQRect( nRow, nCol + c );
            p.fillRect( rc, m_bkgBrush ); 
            m_chars[data.toAscii()]->Draw(p, rc, bHighlight);
        }
    }
    InvalidateCells( nRow, nCol, s.length() );
    emit write( nRow, nCol, s );
}

void LCDWidget::Erase( int nRow, int nCol, int nLen )
{
    QString s(nLen,' ');
    Write( nRow, nCol, s, false );
}

void LCDWidget::Clear()
{
    m_imgLCD.fill( m_bkgBrush.color().rgba() );
    m_cursorPosCol = 0;
    m_cursorPosRow = 0;
    update();
    emit clear();
}

void LCDWidget::InvalidateCells( int /*nRow*/, int /*nCol*/, int /*n*/ )
{
//  Map RC cell coordinates to screen coordinates
//    QRect rc = CellQRect( nRow, nCol );
//    rc.unite( CellQRect( nRow, nCol+n ) );
    update();
}

void LCDWidget::resizeEvent( QResizeEvent * )
{
    setGeometry( x(), y(), width(), heightForWidth(width()) );
    updateGeometry();
}

int LCDWidget::heightForWidth ( int w ) const
{
    double h = (double)w * (double)(m_sizeTextArea.height()) / (double)(m_sizeTextArea.width());
    return (int)h;
}

QSize LCDWidget::sizeHint() const
{
    int w = 10 * m_nCols;
    QSize s = QSize( w, heightForWidth(w) );
    return s;;
}

void LCDWidget::keyPressEvent( QKeyEvent * event )
{
    event->ignore();
    if ( m_bReadOnly )
	return;
    switch ( event->key() )
    {
	case Qt::Key_Home:
	    m_cursorPosCol = 0;
	    m_cursorPosRow = 0;
	    event->accept();
	    break;
	case Qt::Key_Backspace:
	    Write( m_cursorPosRow, m_cursorPosCol, " ", false );
	case Qt::Key_Left:
	    m_cursorPosCol--;
	    if ( m_cursorPosCol < 0 )
		m_cursorPosCol = m_nCols - 1;
	    event->accept();
	    break;
	case Qt::Key_Up:
	    m_cursorPosRow--;
	    if ( m_cursorPosRow < 0 )
		m_cursorPosRow = m_nRows - 1;
	    event->accept();
	    break;
	case Qt::Key_Delete:
	    Write( m_cursorPosRow, m_cursorPosCol, " ", false );
	case Qt::Key_Right:
	    m_cursorPosCol++;
	    if ( m_cursorPosCol >= m_nCols )
		m_cursorPosCol = 0;
	    event->accept();
	    break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
	    m_cursorPosCol = 0;
	case Qt::Key_Down:
	    m_cursorPosRow++;
	    if ( m_cursorPosRow >= m_nRows )
		m_cursorPosRow = 0;
	    event->accept();
	    break;
	default:
	    if ( event->text().length() == 1 && event->text()[0].isPrint() )
	    {
		Write( m_cursorPosRow, m_cursorPosCol, event->text(), false );
		m_cursorPosCol++;
		if ( m_cursorPosCol >= m_nCols )
		{
		    m_cursorPosCol = 0;
		    m_cursorPosRow++;
		    if ( m_cursorPosRow >= m_nRows )
			m_cursorPosRow = 0;
		}
		event->accept();
	    }
	    break;
    }
    if ( event->isAccepted() )
	update();
}


static void setBit( int r, int c, byte *data, bool bSet )
{
    if ( bSet )
	data[c] |= 1 << r;
    else
	data[c] &= ~(1 << r);
}

static bool getBit( int cols, int r, int c, const byte *data )
{
    int nBit = r * cols + (cols-c-1);
    int nByte = nBit / 8;
    nBit &= 0x07;
    return (data[nByte] & (1 << nBit)) != 0;
}

void LCDWidget::SetUserFont( byte index, const QVector<byte> &data )
{
    // for some reason the data in this set is in columns, whereas everything
    // else is in rows.  We need to convert.
    byte char_data[5];
    memset( char_data, 0, sizeof(char_data) );

    for ( int r = 0; r < LCDChar::PIXELS_Y; r++ )
	for ( int c = 0; c < LCDChar::PIXELS_X; c++ )
	    setBit( r, c, char_data, getBit( LCDChar::PIXELS_X, r, c, data.data() ) );

    if ( m_chars.contains(index) )
    {
	delete m_chars[index&0x7];
	delete m_chars[index&0x7|0x8];
    }
    m_chars[index] = new LCDChar( char_data[0], char_data[1], char_data[2], char_data[3], char_data[4] );
    m_chars[index|0x8] = new LCDChar( char_data[0], char_data[1], char_data[2], char_data[3], char_data[4] );
}
