
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

bool SaveToTextualFile(const google::protobuf::Message& pbm, const std::string& filename)
{
	bool rv = false;
	std::ofstream ofs(filename.data(), std::ios::out | std::ios::trunc);
	if(ofs.is_open())
	{
		std::string text;
		google::protobuf::TextFormat::Printer printer;
		printer.SetUseUtf8StringEscaping(true);
		rv = printer.PrintToString(pbm, &text);
		std::cout << text;
		ofs << text;

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
	if(argc < 4)
	{
		std::cout << "pbbin2txt [xxx.bytes] [xxx.proto] [Cfg_xxxx] [output.txt]" << std::endl;
		return 0;
	}

	std::string in_file_name{argv[1]};
	std::string pbname{argv[2]};
	std::string cfgname{argv[3]};
	std::string out_file_name{argv[4]};


	using namespace google::protobuf;
	using namespace google::protobuf::compiler;

	DiskSourceTree sourceTree;
	sourceTree.MapPath("", "./");
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
	
	LoadFromBinaryFile(in_file_name, *pData);
	SaveToTextualFile(*pData, out_file_name);


}
