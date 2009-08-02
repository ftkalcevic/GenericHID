#include "stdafx.h"
#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QString>
#include "programmer.h"


class MyProgrammer: public Programmer
{
    virtual void UpdateStatus( ProgramState::ProgramState status )
    {
	switch ( status )
	{
	    case ProgramState::ErasingDevice:	    fprintf(stderr, "Erasing Device\n" ); break;
	    case ProgramState::ProgrammingEEPROM:   fprintf(stderr, "Programming EEPROM\n" ); break;
	    case ProgramState::VerifyingEEPROM:	    fprintf(stderr, "Verifying EEPROM\n" ); break;
	    case ProgramState::ProgrammingFlash:    fprintf(stderr, "Programming Flash\n" ); break;
	    case ProgramState::VerifyingFlash:	    fprintf(stderr, "Verifying Flash\n" ); break;
	    case ProgramState::Done:		    fprintf(stderr, "Done\n" ); break;
	    default:	    			    fprintf(stderr, "Unknown Status %d\n", (int)status ); break;
	}
    }
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName( argv[0] );

    QStringList sArgs = a.arguments();
    if ( sArgs.count() != 3 )
    {
	fprintf(stderr, "Usage: CLProgram firmware.hex eeprom.hex\n" );
	return 1;
    }

    QString sFirmware = sArgs[1];
    QString sEeprom = sArgs[2];

    MyProgrammer prog;

    if ( prog.Init() )
	if ( prog.Program( sEeprom, sFirmware ) )
	    if ( prog.RunFirmware() )
		return 0;
    return 1;
}
