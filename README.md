# pbbin2txt
convert protobuf binray data to txt format(human readably)


# useage
* cmd  [--input=xxx.bytes] [--pbdir=xxxxx] [--pb=xxx.proto] [--output=output.txt]
* ./pbbin2txt --input=resource/Cfg_BornPos.bytes --pbdir=../src/proto/ --pb=config/Cfg_BornPos.proto --output=Cfg_BornPos.txt
* convert txt file to binray
* cmd  [--input=xxx.txt] [--pbdir=xxxxx] [--pb=xxx.proto] [--output=output.bytes] [--t2b]
* ./pbbin2txt --input=Cfg_BornPos.txt --pbdir=../src/proto/ --pb=config/Cfg_BornPos.proto --output=Cfg_BornPos.bytes --t2b
