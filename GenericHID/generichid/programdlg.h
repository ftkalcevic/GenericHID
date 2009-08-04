#ifndef PROGRAMDLG_H
#define PROGRAMDLG_H

#include <QDialog>
#include "ui_programdlg.h"

class ProgramDlg : public QDialog
{
	Q_OBJECT

public:
	ProgramDlg(QWidget *parent = 0);
	~ProgramDlg();

private:
	Ui::ProgramDlgClass ui;
};

#endif // PROGRAMDLG_H
