/***********************************************************************

 Copyright (c) 2011 IDMIL

 ***********************************************************************/

#ifndef OFXLIBMAPPER_H
#define OFXLIBMAPPER_H

#include <mapper/mapper.h>
#include <lo/lo.h> // liblo

typedef mapper_device ofxLibmapperDevice;
typedef mapper_signal ofxLibmapperSignal;

static ofxLibmapperDevice ofxLibmapperCreateDevice( const char *name_prefix,
	int initial_port, mapper_admin admin )
{
	return mdev_new( name_prefix, initial_port, admin );
}

static void ofxLibmapperFreeDevice( ofxLibmapperDevice device )
{
	mdev_free( device );
}

static ofxLibmapperSignal ofxLibmapperAddOutput( ofxLibmapperDevice md,
	const char *name, int length, char type, const char *unit, void *minimum,
	void *maximum )
{
	return mdev_add_output( md, name, length, type, unit, minimum, maximum );
}

static ofxLibmapperSignal ofxLibmapperAddInput( ofxLibmapperDevice md,
	const char *name, int length, char type, const char *unit, void *minimum,
	void *maximum, mapper_signal_handler *handler, void *user_data)
{
	return mdev_add_input( md, name, length, type, unit, minimum, maximum, handler,
		user_data );
}

static int ofxLibmapperIsDeviceReady ( ofxLibmapperDevice device )
{
	return mdev_ready( device );
}

static int ofxLibmapperPoolDevice( ofxLibmapperDevice device, int block_ms )
{
	return mdev_poll( device, block_ms );
}

static int ofxLibmapperSetSignalName( ofxLibmapperSignal sig, char *name, int len )
{
	return msig_full_name( sig, name, len );
}

static const char * ofxLibmapperGetDeviceName( ofxLibmapperDevice device )
{
	return mdev_name( device );
}

static void ofxLibmapperUpdateFloat( mapper_signal sig, float value )
{
	msig_update_float( sig, value );
}

static void ofxLibmapperUpdateInt( mapper_signal sig, int value )
{
	msig_update_int( sig, value );
}

#endif // OFXLIBMAPPER_H
