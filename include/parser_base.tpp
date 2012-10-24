#ifndef _PARSER_BASE_TPP_
#define _PARSER_BASE_TPP_

//------------------------------------------------------------------------------
template <class T>
void parser_base<T>::parse(const std::string & p_name)
{

  std::ifstream l_tmp_input_file(p_name.c_str());
  if(l_tmp_input_file==NULL)
    {
      std::cout << "ERROR : Unable to open file \"" << p_name << "\"" << std::endl ;
      exit(-1);
    }
  l_tmp_input_file.close();
  igzstream l_input_file(p_name.c_str());
    this->parse_stream(l_input_file);
    l_input_file.close();
}

#endif //_PARSER_BASE_TPP_
//EOF
