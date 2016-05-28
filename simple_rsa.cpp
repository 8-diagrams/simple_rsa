#include <iostream>
#include <boost/program_options.hpp>
#include "rsa.h"

using namespace std;
using namespace simple_rsa;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  po::options_description general_desc("General options");
  general_desc.add_options()
    ("help,h", "produce a help message")
    ("version,v", "output the version")
    ;

  po::options_description argv_desc("Keygen options");
  argv_desc.add_options()
    ("bits,b", po::value<int>()->default_value(1024), "bits of the key, 1024|2048")
    ("key,k", po::value<string>()->default_value("simple_rsa_key"), "key file name")
    ;
  po::options_description crypt_desc("Encrypt/Decrypt options");
  crypt_desc.add_options()
    ("key,k", po::value<string>()->default_value("simple_rsa_key"), "key file name")
    ("input,i", po::value<string>(), "input file")
    ("output,o", po::value<string>(), "output file")
    ;
  po::options_description sign_desc("Sign/Verify options");
  sign_desc.add_options()
    ("key,k", po::value<string>()->default_value("simple_rsa_key"), "key file name")
    ;
  po::options_description verify_desc("Verify options");
  verify_desc.add_options()
    ("output,o", po::value<string>(), "output file")
    ;

  po::options_description all;
  all.add(general_desc).add(argv_desc).add(crypt_desc).add(sign_desc).add(verify_desc);

  po::options_description cmd_po("cmd options");
  cmd_po.add_options()
    ("help,h", "produce a help message")
    ("version,v", "output the version")
    ("cmd", po::value<string>(), "command to excute")
    ("argv", po::value<std::vector<std::string> >(), "arguments")
    ;

  po::positional_options_description pos;
  pos.add("cmd", 1).add("argv", -1);

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv).
      options(cmd_po).
      positional(pos).
      allow_unregistered().
      run();
  po::store(parsed, vm);

  if(vm.size() == 0 || vm.count("help")) {
  cout<<"usage: simple_rsa [--version] [--help] <command> [<args>]\n"<<
      "  command: keygen | encrypt | decrypt | sign | verify\n"<<
      all<<"\n"<<
      "Bug report: <exiledkingcc@gmail.com>"<<endl;
  exit(0);
  }
  if(vm.count("version")) {
  cout<<"simple_rsa version 0.1\n"<<
      "Copyright (C) 2016 cc<exiledkingcc@gmail.com>\n"<<
      "this is just a demo, do NOT use it at work!"<<endl;
  exit(0);
  }
  return 0;
}
