
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_set>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>
#include "get_opt.h"
#include "StringAlgo.h"


bool SaveToTXTFile(const google::protobuf::Message& pbm, const std::string& filename)
{
	bool rv = false;
	std::ofstream ofs(filename.data(), std::ios::out | std::ios::trunc);
	if(ofs.is_open())
	{
		std::string text;
		google::protobuf::TextFormat::Printer printer;
		printer.SetUseUtf8StringEscaping(true);
		rv = printer.PrintToString(pbm, &text);
		std::cout << text << std::endl;
		ofs << text;

		ofs.close();
	}
	return rv;
}

bool LoadFromTXTFile(const std::string& filename, google::protobuf::Message& pbm)
{
	bool rv = false;

	std::ifstream ifs(filename.data());
	if(ifs.is_open())
	{
		google::protobuf::io::IstreamInputStream raw_input(&ifs); 

		rv = google::protobuf::TextFormat::Parse(&raw_input, &pbm);
		ifs.close();

		if(rv != true)
			std::cerr << "ParseFromStream failed, filename is "<< filename.c_str() << std::endl;
	}
	else
	{
		std::cerr << "Open file failed, filename is "<< filename.c_str() << std::endl;
	}
	return rv;
}

bool SaveToBinaryFile(const google::protobuf::Message& pbm, const std::string& filename)
{
	bool rv = false;
	std::ofstream ofs(filename.data(), std::ios::out | std::ios::binary);
	if (ofs.is_open())
	{
		pbm.SerializeToOstream(&ofs);
		ofs.close();
	}
	return rv;
}

bool LoadFromBinaryFile(const std::string& filename, google::protobuf::Message& pbm)
{
	bool rv = false;

	std::ifstream ifs(filename.data());
	if(ifs.is_open())
	{
		rv = pbm.ParseFromIstream(&ifs);
		ifs.close();

		if(rv != true)
			std::cerr << "ParseFromStream failed, filename is "<< filename.c_str() << std::endl;
	}
	else
	{
		std::cerr << "Open file failed, filename is "<< filename.c_str() << std::endl;
	}
	return rv;
}


int main(int argc, char** argv)
{
	get_opt opt(argc, (const char**)argv);
	if(opt.has("--input") == false ||
	   opt.has("--pbdir") == false ||
	   opt.has("--pb") == false ||
	   opt.has("--help") == true)
	{
		std::cout << "pbbin2txt [--input=xxx.bytes] [--pbdir=xxxxx] [--pb=xxx.proto] [--output=output.txt]" << std::endl;
		return 0;
	}

	std::string in_file_name = opt["--input"];
	std::string pbdirname = opt["--pbdir"];
	std::string pbname = opt["--pb"];
	std::string cfgname = GetFileNameWithoutExt(GetFileNameFromFullPath(pbname));
	if(opt.has("--cfg"))
		cfgname = opt["--cfg"];

	using namespace google::protobuf;
	using namespace google::protobuf::compiler;

	DiskSourceTree sourceTree;
	sourceTree.MapPath("", pbdirname);
	Importer importer(&sourceTree, nullptr);
	const FileDescriptor* fdes = importer.Import(pbname);
	if(fdes == nullptr)
	{
		std::cerr << "importer fail";
		return -1;
	}

	const Descriptor* desc = importer.pool()->FindMessageTypeByName(cfgname);
	if(desc == nullptr)
	{
		std::cerr << "find cfgname fail";
		return -1;
	}

	std::cout << desc->DebugString() << std::endl;
	std::cout << "prass any key to start convert" << std::endl;
	getchar();

	DynamicMessageFactory factory;
	const Message* message = factory.GetPrototype(desc);
	Message* pData = message->New();
	
	if(opt.has("--t2b"))
	{
		LoadFromTXTFile(in_file_name, *pData);
		if(opt.has("--output"))
		{
			SaveToBinaryFile(*pData, opt["--output"]);
		}
		else
		{
		
			std::cout << pData->Utf8DebugString()  << std::endl;;
		}
	}
	else
	{
		LoadFromBinaryFile(in_file_name, *pData);
		if(opt.has("--output"))
		{
			SaveToTXTFile(*pData, opt["--output"]);
		}
		else
		{
		
			std::cout << pData->Utf8DebugString()  << std::endl;;
		}
	}

}
