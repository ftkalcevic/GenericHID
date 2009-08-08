#include "stdafx.h"
#include "editor.h"

Editor::Editor(void)
{
    m_eEditMode = EditMode::Pointer;
    m_curMirror = NULL;
    m_curMirrorOff = NULL;
    m_curRotate = NULL;
    m_curRotateOff = NULL;
    m_curWire = NULL;
    m_curWireOff = NULL;
    m_curWireNot = NULL;
    m_curPointer = NULL;
    m_pWiringStartPin = NULL;
    m_pCurrentWire = NULL;
}

Editor::~Editor(void)
{
}

