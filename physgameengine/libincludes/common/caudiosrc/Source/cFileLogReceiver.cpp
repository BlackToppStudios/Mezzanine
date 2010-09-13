// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#include "../Headers/cFileLogReceiver.h"

#ifdef CAUDIO_COMPILE_WITH_FILE_LOG_RECEIVER
#include <iostream>
#include <fstream>

namespace cAudio
{
	cFileLogReceiver::cFileLogReceiver()
	{
		firsttime = false;
	}

	cFileLogReceiver::~cFileLogReceiver()
	{
		//the logstream is managed by the logger
	}

	bool cFileLogReceiver::OnLogMessage(const char* sender, const char* message, LogLevel level, float time)
	{
		
		
		//std::ofstream outf;
		
		if(firsttime == false)
		{
			
			(*LogStream) << "Audio Logging Initialized"<<std::endl;
			/*if( !outf.is_open() )Audio
			{
			// Reset log file
			outf.setf( std::ios::fixed );
			outf.precision( 3 );
			outf.open( "cAudioEngineLog.html", std::ios::out );
			
				if( !outf ){
					return false;
				}

			
			}*/
			firsttime = true;
		}
		else
		{
			/*outf.open( "cAudioEngineLog.html", std::ios::out | std::ios::app );
			
			if( !outf ){
				return false;
			}

			outf<<"<tr>\n";
			outf<<"<td width=\"100\">";
			outf<<time;
			outf <<"</td>\n";
			outf<<"<td class=\"";
			*/
			(*LogStream) << time << " ";
			switch( level )
			{
			case ELL_DEBUG:
				(*LogStream)<<"debug ";
				break;

			case ELL_INFO:
				(*LogStream)<<"info ";
				break;

			case ELL_WARNING:
				(*LogStream)<<"warn ";
				break;

			case ELL_ERROR:
				(*LogStream)<<"err ";
				break;

			case ELL_CRITICAL:
				(*LogStream)<<"crit ";
				break;

			case ELL_COUNT:
				(*LogStream)<<"debug ";
				break;

			default:
				(*LogStream)<<"debug ";
			}
			(*LogStream)<<message<<std::endl;
			
		}
		return true;
	}

};

#endif




