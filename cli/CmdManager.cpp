#include "CmdManager.h"

#include "command/command.h"

#include <stdio.h>

CmdManager::CmdManager() : msim(*this)
{
	mgl = new_GetLine(1024, 1024);
	#define CMD(str, cls) registerCmd(str, cls);
	#include "cmd.def"
	#undef CMD
}

CmdManager::~CmdManager()
{
	del_GetLine(mgl);
}

static std::string getargv(int i, int argc, char** argv)
{ return (i<argc) ? argv[i] : ""; }

void CmdManager::run(int argc, char** argv)
{
	bool isint = argc <= 1;
	
	for(int i = 1; i<argc; ++i)
	{ 
		std::string a = getargv(i, argc, argv);
		
		if(a == "-i")
			isint = true;
		else if(a == "-e")
			execfile(getargv(++i, argc, argv));
		else if(a == "-p")
			execute(getargv(++i, argc, argv));
	}
	
	interact();
}

void CmdManager::execute(const std::string& str)
{
	Nepeta ne;
	ne.compile(str);
	
	if(ne.getError().size()>0){
		std::cout << ne.fullErrorStr();
		return;
	}
	
	for(Nepeta::Iterator i = ne.getRoot().begin(); ne.getRoot().hasNext(i); ne.getRoot().next(i))
	{
		Nepeta::Node &node = ne.getRoot().getIterNode(i);
		CommandMap::iterator ci = mCommands.find(node.getId());
		if(ci != mCommands.end())
			ci->second->run(node);
	}
}

void CmdManager::execfile(const std::string& str)
{
	FILE *fp = fopen(str.c_str(), "r"); 
	if(fp) {
		fseek(fp, 0, SEEK_END);
		size_t size = ftell(fp);
		rewind(fp);
		std::string buf;
		buf.resize(size);
		fread(&buf[0], 1, size, fp);
		fclose(fp);
		execute(buf);
	}
	
}

void CmdManager::interact()
{
	bool isend = false;
	while (!isend)
	{
		std::string cmd = gl_get_line(mgl, "> ", 0, 0);
		
		if(cmd == "exit")
			isend = true;
		else execute(cmd);
	}
}
