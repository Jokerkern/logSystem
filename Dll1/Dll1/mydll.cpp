#include<stdio.h>
#include "mydll.h"
#include"pch.h"



logControl* g_logControl = NULL;
std::shared_ptr<logsystem_server> g_webSocketControl = NULL;
//char path[1024] = {};



void Init() {
	g_webSocketControl = std::make_shared<logsystem_server>();
	thread t([&] {g_webSocketControl->run(9002); });
	t.detach();
}

void Open(const char* name) {
	char path[1024] = {};
	strcat(path, name);
	strcat(path, ".txt");
	std::ofstream fout;
	fout.open("dir.txt", std::ios::out | std::ios::app);
	fout << path << std::endl;

	g_logControl = new logControl;
	std::cout << "Open.path=" << path << std::endl;
	g_logControl->open(name, path, g_webSocketControl);

}

void Write(LOGLEVEL type, const char* log) {
	g_logControl->write(type, log);
	//g_webSocketControl->sendData(log);
	//std::ifstream fs;
	//std::string lastline;
	//std::cout << "Write.path=" << path << std::endl;

	//fs.open(path);

	//if (fs.is_open())
	//{
	//	fs.seekg(-1, std::ios_base::end);
	//	if (fs.peek() == '\n')
	//	{
	//		fs.seekg(-1, std::ios_base::cur);
	//		int i = fs.tellg();
	//		for (i; i >= 0; i--)
	//		{
	//			if (fs.peek() == '\n')
	//			{
	//				//Found 
	//				fs.get();
	//				break;
	//			}
	//			fs.seekg(i, std::ios_base::beg);
	//		}
	//	}
	//	getline(fs, lastline);

	//}
	//Sleep(10000);
	//std::cout << "Senddata: " << lastline << std::endl;
	//Senddata(lastline);
	
}

void Close() {
	//memset(path, 0, 1024 * sizeof(char));
	g_logControl->close();
	delete g_logControl;
}

void Uninit() {
	//delete g_webSocketControl;
}

