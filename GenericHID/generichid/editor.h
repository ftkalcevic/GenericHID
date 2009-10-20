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
    QCursor *m_curRotateOff;
    QCursor *m_curWire;
    QCursor *m_curWireOff;
    QCursor *m_curWireNot;
    QCursor *m_curPointer;
    class PinItem *m_pWiringStartPin;
    class WireItem *m_pCurrentWire;
};

#endif
