#include "stdafx.h"
#include "timerconfigdlg.h"
#include "timercounter.h"
#include "deviceconfig.h"

TimerConfigDlg::TimerConfigDlg( int nBits, QStringList &sPrescales, QWidget *parent)
: QDialog(parent)
, m_nBits( nBits )
, m_sPrescales( sPrescales )
{
    ui.setupUi(this);

    m_nClockFrequency = 8000000;
    //m_nBits = 16;
    //m_sPrescales << "1" << "8" << "64" << "256" << "1024";
    //m_nBits = 8;
    //m_sPrescales << "1" << "8" << "32" << "64" << "128" << "256" << "1024";

    ui.cboWaveformMode->addItems( QStringList() << "Phase Correct" << "Fast PWM" );
    ui.cboPrescale->addItems( m_sPrescales );
    ui.lblClockFrequency->setText( QString("%1MHz").arg(((double)m_nClockFrequency)/1000000.0, 0, 'f', 1 ) );
    ui.lblBits->setText( QString::number(m_nBits) );

    ui.spinCounterTop->setRange( 1, (2<<m_nBits)-1 );

    ComputeTimer();

    ui.spinFrequency->setRange(0.001, m_nClockFrequency );
    ui.spinResolution->setRange( 1, (2<<m_nBits)-1 );

    ui.groupAdvanced->setVisible( false );

    ui.spinResolution->setValue(100);
    ui.spinFrequency->setValue(10000);
}

TimerConfigDlg::~TimerConfigDlg()
{
}

void TimerConfigDlg::ComputeTimer()
{
    int nPrescale = ui.cboPrescale->currentText().toInt();
    int nMode = ui.cboWaveformMode->currentIndex();
    int nTop = ui.spinCounterTop->value();

    double dClock = (double)m_nClockFrequency / (double)nPrescale;
    double dFreq = dClock / (double)nTop;
    if ( nMode == TC_MODE_PHASECORRECT )	// phase correct
	dFreq /= 2;

    ui.lblOutputFrequency->setText( TimerCounter::MakeFreq( dFreq ) );

    int nBits = 0;
    while ( nTop != 0 )
    {
	nBits++;
	nTop >>= 1;
    }
    ui.lblDataBits->setText( QString::number(nBits) );
}



void TimerConfigDlg::AutoComputeTimer()
{
    double dTargetFrequency = ui.spinFrequency->value();
    int nResolution = ui.spinResolution->value();

    // Brute force search through the combinations to find the closest.
    int nAutoPrescale=0;
    int nAutoMode=0;
    int nAutoTop=0;
    double dAutoFrequency=0;

    double dError = 1000.0 * (double)m_nClockFrequency;
    foreach(QString sPrescale, m_sPrescales )
    {
	int nPrescale = sPrescale.toInt();

	double dFreq = (double)m_nClockFrequency / (double)nPrescale;
	dFreq = dFreq / (double)nResolution;
	
	// fast pwm
	if ( abs(dFreq - dTargetFrequency) < dError )
	{
	    dError = abs(dFreq - dTargetFrequency);
	    nAutoPrescale = nPrescale;
	    nAutoMode = TC_MODE_FASTPWM;
	    nAutoTop = nResolution;
	    dAutoFrequency = dFreq;
	}

	// phase correct
	dFreq /= 2.0;
	if ( abs(dFreq - dTargetFrequency) < dError )
	{
	    dError = abs(dFreq - dTargetFrequency);
	    nAutoPrescale = nPrescale;
	    nAutoMode = TC_MODE_PHASECORRECT;
	    nAutoTop = nResolution;
	    dAutoFrequency = dFreq;
	}
    }

    ui.lblActualFrequency->setText( TimerCounter::MakeFreq( dAutoFrequency ) );

    ui.cboPrescale->setCurrentIndex( ui.cboPrescale->findText( QString::number(nAutoPrescale) ) );
    ui.cboWaveformMode->setCurrentIndex( nAutoMode );
    ui.spinCounterTop->setValue( nAutoTop );
}



void TimerConfigDlg::onShowAdvanced()
{
    ui.groupAdvanced->setVisible( ui.btnAdvanced->isChecked() );
}


void TimerConfigDlg::setValue( const QString &sTimerDetails )
{
    int nMode;
    int nPrescaler;
    int nTop;
    TimerCounter::ExtractDetails( sTimerDetails, nMode, nPrescaler, nTop );

    ui.cboPrescale->setCurrentIndex( ui.cboPrescale->findText( QString::number(nPrescaler) ) );
    ui.cboWaveformMode->setCurrentIndex( nMode );
    ui.spinCounterTop->setValue( nTop );

    double dFreq = (double)m_nClockFrequency / (double)nPrescaler / (double)nTop;
    if ( nMode == TC_MODE_PHASECORRECT )	// phase correct
	dFreq /= 2;
    ui.spinFrequency->setValue(dFreq);
    ui.spinResolution->setValue( nTop);
}

QString TimerConfigDlg::value() const
{
    QString s = QString("%1,%2,%3").arg(ui.cboWaveformMode->currentIndex()).arg(ui.cboPrescale->currentText()).arg(ui.spinCounterTop->value());
    return s;
}

