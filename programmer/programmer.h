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

#ifndef _PROGRAMMER_H_
#define _PROGRAMMER_H_

#include "log.h"

namespace ProgramState
{
    enum ProgramState
    {
	ErasingDevice,
        ProgrammingEEPROM,
        VerifyingEEPROM,
        ProgrammingFlash,
        VerifyingFlash,
        Done
    };
};

namespace ProgrammerType
{
    enum ProgrammerType
    {
        DFU,
        HalfK
    };
};



class DeviceProgrammer;

class Programmer
{
public:
    Programmer();
    ~Programmer();

    bool Init(ProgrammerType::ProgrammerType type);
    bool Terminate();
    bool Program( const QString &sEepromPath, const QString &sFirmwarePath );
    bool RunFirmware();
    QString LastError();

protected:
    virtual void UpdateStatus( ProgramState::ProgramState status ) = 0;
    virtual void CompletionStatus( int nPercentComplete ) = 0;
    static void completion_callback( void *user_data, int percent );

    Logger m_Logger;
    DeviceProgrammer *m_programmer;
};

#endif // PROGRAMMER_H
