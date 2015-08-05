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
    virtual void CompletionStatus( int nPercentComplete )
    {
	fprintf( stderr, "\b\b\b\b%3d%%", nPercentComplete );
	if ( nPercentComplete == 100 )
	    fprintf( stderr, "\n" );
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

    if ( prog.Init(ProgrammerType::DFU) )
	if ( prog.Program( sEeprom, sFirmware ) )
	    if ( prog.RunFirmware() )
		return 0;
    return 1;
}
