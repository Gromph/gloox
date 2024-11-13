/*
  Copyright (c) 2005-2023 by Jakob Schröter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/



#include "tlsopensslclient.h"

#ifdef HAVE_OPENSSL

namespace gloox
{

  OpenSSLClient::OpenSSLClient( TLSHandler* th, const std::string& server )
    : OpenSSLBase( th, server )
  {
  }

  OpenSSLClient::~OpenSSLClient()
  {
  }

  bool OpenSSLClient::setType()
  {
    m_ctx = SSL_CTX_new( SSLv23_client_method() );
    if( !m_ctx )
      return false;

    SSL_CTX_set_options( m_ctx, SSL_OP_NO_SSLv3 );

    return true;
  }

  bool OpenSSLClient::hasChannelBinding() const
  {
    return true;
  }

  const std::string OpenSSLClient::channelBinding() const
  {

    if( SSL_version( m_ssl ) == TLS1_3_VERSION )
    {
      unsigned char buf[32];
      const char* const label = "EXPORTER-Channel-Binding";
      SSL_export_keying_material( m_ssl, buf, 32, label, strlen( label ), { 0 }, 1, 0 );
      return std::string( reinterpret_cast<char* const>( buf ), 32 );
    }
    else
    {
      unsigned char* buf[128];
      long res = SSL_get_finished( m_ssl, buf, 128 );
      return std::string( reinterpret_cast<char*>( buf ), res );
    }
  }

  const std::string OpenSSLClient::channelBindingType() const
  {
    if( SSL_version( m_ssl ) == TLS1_3_VERSION ) {
      return "tls-exporter";
    }
    else
    {
      return "tls-unique";
    }
  }

  int OpenSSLClient::handshakeFunction()
  {
    return SSL_connect( m_ssl );
  }

}

#endif // HAVE_OPENSSL
