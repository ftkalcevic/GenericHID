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

#include "stdafx.h"
#include "mru.h"

MRU::MRU(void)
: m_pSeparator(NULL)
, m_pActionGroup(NULL)
{
}

MRU::~MRU(void)
{
}

void MRU::append( const QString &s )
{
    // Check for duplicate strings.  Remove if found, then insert again at top of the list
    m_MRU.removeAll( s );
    m_MRU.push_front( s );
    while ( m_MRU.count() > MAX_MRU )
	m_MRU.pop_back();

    // Rebuild the menu
    Update();
}

void MRU::onActionTriggered(QAction*pAction)
{
    assert( pAction != NULL );
    if ( pAction != NULL )
    {
	QString sFile = pAction->data().toString();
	emit MRUSelected(sFile);
    }
}

void MRU::Update()
{
    if ( m_MRU.count() == 0 )
    {
	m_pSeparator->setVisible( false );
	foreach( QAction *pAction, m_Actions )
	    pAction->setVisible( false );
    }
    else
    {
	m_pSeparator->setVisible( true );
	for ( int i = 0; i < m_Actions.count(); i++ )
	    if ( i < m_MRU.count() )
	    {
		m_Actions[i]->setText( QString("&%1 %2").arg(i+1).arg(m_MRU[i]) );
		m_Actions[i]->setData( m_MRU[i] );
		m_Actions[i]->setVisible( true );
	    }
	    else
		m_Actions[i]->setVisible( false );
    }
}


void MRU::setMenu( QMenu *pMenu )
{ 
    // Add MRU actions to the menu  Add them after the last separator in the menu
    QAction *pLastSeparator = NULL;
    QList<QAction *> actions = pMenu->actions();
    for ( int i = actions.count() - 1; i >= 0; i-- )
	if ( actions[i]->isSeparator() )
	{
	    pLastSeparator = actions[i];
	    break;
	}

    // Insert MAX_MRU actions and a separator before the separator.
    if ( pLastSeparator != NULL )
    {
	m_pSeparator = new QAction(NULL);
	m_pSeparator->setSeparator(true);
	m_pSeparator->setVisible( false );
	pMenu->insertAction( pLastSeparator, m_pSeparator );

	for ( int i = 0; i < MAX_MRU; i++ )
	{
	    QAction *pAction = new QAction(NULL);
	    pAction->setVisible( false );
	    pMenu->insertAction( pLastSeparator, pAction );
	    m_Actions.push_back( pAction );
	}
    }
    else if ( actions.count() > 0 )
    {
	// else - no spacer - append to end of menu
	m_pSeparator = pMenu->addSeparator();
	m_pSeparator->setVisible(false);
	for ( int i = 0; i < MAX_MRU; i++ )
	{
	    QAction *pAction = pMenu->addAction("");
	    pAction->setVisible( false );
	    m_Actions.push_back( pAction );
	}
    }
    else
    {
	// else - no menu, just actions
	m_pSeparator = NULL;
	for ( int i = 0; i < MAX_MRU; i++ )
	{
	    QAction *pAction = pMenu->addAction("");
	    pAction->setVisible( false );
	    m_Actions.push_back( pAction );
	}
    }

    m_pActionGroup = new QActionGroup(NULL);
    foreach ( QAction *pAction, m_Actions )
	m_pActionGroup->addAction( pAction );

    connect( m_pActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionTriggered(QAction*)) );
}


void MRU::setEnabled( bool bEnabled )
{ 
    foreach( QAction *pAction, m_Actions )
	pAction->setEnabled( bEnabled );
}

