// hidcomp/hidconfig, HID device interface for emc
// Copyright (C) 2008, Frank Tkalcevic, www.franksworkshop.com

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

#ifndef __XMLUTILITY_H__
#define __XMLUTILITY_H__

#include <QDomElement>

class XMLUtility
{
public:
    XMLUtility(void);
    ~XMLUtility(void);

    static void setAttribute( QDomElement &pNode, const char *sName, const QString sValue );
    static void setAttribute( QDomElement &pNode, const char *sName, int nValue );
    static void setAttribute( QDomElement &pNode, const char *sName, double nValue );
    static void setAttribute( QDomElement &pNode, const char *sName, bool bValue );

    static bool getAttributeString( QDomElement &pNode, const char *sName, QString &s );
    static QString getAttribute( QDomElement &pNode, const char *sName, const QString &sDefault );
    static QString getAttribute( QDomElement &pNode, const char *sName, const char *sDefault );
    static int getAttribute( QDomElement &pNode, const char *sName, int nDefault );
    static double getAttribute( QDomElement &pNode, const char *sName, double nDefault );
    static bool getAttribute( QDomElement &pNode, const char *sName, bool bDefault );

    static QDomElement firstChildElement(QDomElement &pNode, const char *sTag);
    static QDomNodeList elementsByTagName(QDomElement &pNode, const char *sTag);
    static bool hasAttribute(QDomElement &pNode, const char *sTag);
};


#endif
