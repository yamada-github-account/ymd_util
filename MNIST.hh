#ifndef YMD_MNIST_HH
#define YMD_MNIST_HH 1

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <vector>
#include <functional>
#include <algorithm>

#include "byte_swap.hh"

namespace ymd {
  template<typename Type> inline auto read_byte(std::ifstream& ifs,Type& arg){
    ifs.read((char*)&arg,sizeof(arg));

    if(std::endian::little == std::endian::native){
      switch(sizeof(arg)){
      case 4:
	arg = swap32(arg);
	break;
      case 2:
	arg = swap16(arg);
      }
    }

    return sizeof(arg);
  }

  template<> inline auto read_byte(std::ifstream& ifs,std::uint8_t& arg){
    ifs.read((char*)&arg,sizeof(arg));
    return sizeof(arg);
  }

  template<typename...Type> inline auto read_bytes(std::ifstream& ifs,Type&...args){
    return (... + read_byte(ifs,args));
  }

  inline auto read_MNIST(std::string filename,bool normalize = true){
    std::ifstream ifs(filename,std::ios::in | std::ios::binary);
    if(!ifs.is_open()){
      std::cerr << "Fail to Open " << filename << std::endl;
    }

    std::uint32_t magic_number,Nitem,Nrows,Ncolumns;

    ymd::read_bytes(ifs,magic_number,Nitem);
    if(!ifs.good()){
      std::cerr << "Fail to Read" << std::endl;
    }

    std::cout << magic_number << " (image:2051, label:2049)" << std::endl;
    std::cout << Nitem << std::endl;

    std::function<std::vector<double>()> f;
    std::function<double(double)> normalizer = (normalize) ?
      [](double d){ return(d - 128)/2.0; } :
      [](double d){ return d; };

    switch(magic_number){
    case 2051:
      ymd::read_bytes(ifs,Nrows,Ncolumns);
      std::cout << Nrows << " x " << Ncolumns << std::endl;
      f = [&](){
	    auto image = std::vector<double>{};
	    image.reserve(Nrows*Ncolumns);
	    std::uint8_t byte;
	    std::generate_n(std::back_inserter(image),Nrows*Ncolumns,
			    [&](){
			      ymd::read_byte(ifs,byte);
			      return normalizer(byte * 1.0);
			    });
	    return image;
	  };
      break;

    case 2049:
      f = [&](){
	    auto label = std::vector<double>(10,0.0);
	    std::uint8_t byte;
	    ymd::read_byte(ifs,byte);
	    label[byte] = 1.0;
	    return label;
	  };
      break;
    }

    auto data = std::vector<std::vector<double>>{};
    data.reserve(Nitem);
    std::generate_n(std::back_inserter(data),Nitem,f);

    return data;
  }

} // namespace ymd
#endif // YMD_MNIST_HH
