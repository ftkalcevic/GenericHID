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
    bool Terminate();
    bool Program( const QString &sEepromPath, const QString &sFirmwarePath );
    bool RunFirmware();
    QString LastError();

protected:
    virtual void UpdateStatus( ProgramState::ProgramState status ) = 0;
    virtual void CompletionStatus( int nPercentComplete ) = 0;
    static void completion_callback( void *user_data, int percent );

    class DFUProgrammer *m_programmer;
};

#endif // PROGRAMMER_H
