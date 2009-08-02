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
    const QString &LastError();

protected:
    virtual void UpdateStatus( ProgramState::ProgramState status ) = 0;
    class Flip *m_flip;
};

#endif // PROGRAMMER_H
