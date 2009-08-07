#ifndef _EDITOR_H_
#define _EDITOR_H_


namespace EditMode
{
    enum EditMode
    {
	Pointer,
	Wiring,
	Mirror,
	Rotate
    };
}

class Editor
{
public:
    Editor(void);
    ~Editor(void);

    EditMode::EditMode m_eEditMode;
    QCursor *m_curMirror;
    QCursor *m_curMirrorOff;
    QCursor *m_curRotate;
    QCursor *m_curWire;
    QCursor *m_curPointer;
};

#endif
