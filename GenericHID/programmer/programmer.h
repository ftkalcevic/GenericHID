#ifndef _PROGRAMMER_H_
#define _PROGRAMMER_H_

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

class Programmer
{
public:
    Programmer();
    ~Programmer();

    bool Init();
    bool Program( const QString &sEepromPath, const QString &sFirmwarePath );
    bool RunFirmware();
    QString LastError();

protected:
    virtual void UpdateStatus( ProgramState::ProgramState status ) = 0;
//    class Flip *m_flip;
    class DFUProgrammer *m_programmer;
};

#endif // PROGRAMMER_H
