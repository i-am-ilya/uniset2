#include <string>
#include "Debug.h"
#include "ObjectsActivator.h"
#include "SharedMemory.h"
#include "Extensions.h"
#include "TestProc.h"
// --------------------------------------------------------------------------
using namespace std;
using namespace UniSetTypes;
using namespace UniSetExtensions;
// --------------------------------------------------------------------------
int main(int argc, const char **argv)
{
	if( argc>1 && ( strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0 ) )
	{
		cout << "--confile	- Использовать указанный конф. файл. По умолчанию configure.xml" << endl;
		SharedMemory::help_print(argc, argv);
		return 0;
	}

	try
	{
		string confile = UniSetTypes::getArgParam( "--confile", argc, argv, "configure.xml" );
		conf = new Configuration(argc, argv, confile);

		conf->initDebug(dlog,"dlog");
		string logfilename = conf->getArgParam("--logfile", "smemory.log");
		string logname( conf->getLogDir() + logfilename );
		ulog.logFile( logname );
		dlog.logFile( logname );

		SharedMemory* shm = SharedMemory::init_smemory(argc, argv);
		if( !shm )
			return 1;

		TestProc tp(conf->getObjectID("TestProc1"));
		tp.init_dlog(dlog);

		ObjectsActivator act;

		act.addObject(static_cast<class UniSetObject*>(shm));
		act.addObject(static_cast<class UniSetObject*>(&tp));

		SystemMessage sm(SystemMessage::StartUp);
		act.broadcast( sm.transport_msg() );
		act.run(false);

		return 0;
	}
	catch( SystemError& err )
	{
		ulog.crit() << "(smemory): " << err << endl;
	}
	catch( Exception& ex )
	{
		ulog.crit() << "(smemory): " << ex << endl;
	}
	catch( std::exception& e )
	{
		ulog.crit() << "(smemory): " << e.what() << endl;
	}
	catch(...)
	{
		ulog.crit() << "(smemory): catch(...)" << endl;
	}

	return 1;
}