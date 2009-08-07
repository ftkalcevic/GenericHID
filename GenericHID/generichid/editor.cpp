#include "stdafx.h"
#include "editor.h"

Editor::Editor(void)
{
    m_eEditMode = EditMode::Pointer;
    m_curMirror = NULL;
    m_curMirrorOff = NULL;
    m_curRotate = NULL;
    m_curWire = NULL;
    m_curPointer = NULL;
}

Editor::~Editor(void)
{
}

