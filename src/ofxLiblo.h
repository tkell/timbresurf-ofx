/***********************************************************************

 Copyright (c) 2011 IDMIL

 ***********************************************************************/

#ifndef OFXLIBLO_H
#define OFXLIBLO_H

#include <mapper/mapper.h>
#include <stdarg.h>

typedef lo_address ofxLibloAddress;

// Need this logic to properly implement a wrapper around lo_send_message
static int ofxLibloSendMessage( ofxLibloAddress t, const char *path, const char *types, ... )
{
	int rc=0;
	va_list ap;
	va_start( ap, types );
	lo_message m = lo_message_new();
	if ( m ) {
		lo_message_add_varargs( m, types, ap );
		rc = lo_send_message( t, path, m );
		lo_message_free( m );
	}
	return rc;
}

static ofxLibloAddress ofxLibloURLToAddress( const char *url )
{
	return lo_address_new_from_url( url );
}

static void ofxLibloFreeAddress( ofxLibloAddress t )
{
	lo_address_free( t );
}

static void ofxLibloSetTimeToLive( ofxLibloAddress t, int ttl )
{
	lo_address_set_ttl( t, ttl );
}

#endif // OFXLIBLO_H
