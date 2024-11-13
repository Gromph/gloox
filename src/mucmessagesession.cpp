/*
  Copyright (c) 2006-2023 by Jakob Schröter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "mucmessagesession.h"
#include "clientbase.h"
#include "message.h"
#include "messagehandler.h"

namespace gloox
{

  MUCMessageSession::MUCMessageSession( ClientBase* parent, const JID& jid )
    : MessageSession( parent, jid, false, Message::Groupchat | Message::Chat
                                          | Message::Normal | Message::Error,
                      false )
  {
  }

  MUCMessageSession::~MUCMessageSession()
  {
  }

  void MUCMessageSession::handleMessage( Message& msg )
  {
    if( m_messageHandler )
      m_messageHandler->handleMessage( msg );
  }

  void MUCMessageSession::send( const std::string& message )
  {
    send( message, EmptyString );
  }

  void MUCMessageSession::send( const std::string& message, const std::string& subject, const StanzaExtensionList& sel )
  {
    if( !m_hadMessages )
    {
      m_thread = "gloox" + m_parent->getID();
      m_hadMessages = true;
    }

    Message m( Message::Groupchat, m_target.bare(), message, subject, m_thread );
    m.setID( m_parent->getID() );
    decorate( m );

    if( sel.size() )
    {
      StanzaExtensionList::const_iterator it = sel.begin();
      for( ; it != sel.end(); ++it )
        m.addExtension( (*it));
    }

    MessageSession::send( m );
  }

  void MUCMessageSession::setSubject( const std::string& subject )
  {
    Message m( Message::Groupchat, m_target.bareJID(), EmptyString, subject );
    m_parent->send( m );
  }

}
