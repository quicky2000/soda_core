#ifndef _PARSER_BASE_TPP_
#define _PARSER_BASE_TPP_

//------------------------------------------------------------------------------
template <class T>
void parser_base<T>::parse(const std::string & p_name)
{

  std::ifstream l_tmp_input_file(p_name.c_str());
  if(l_tmp_input_file==NULL)
    {
      std::stringstream l_stream ;
      l_stream << "ERROR : Unable to open file \"" << p_name << "\"";
      throw new quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
    }
  l_tmp_input_file.close();
  igzstream l_input_file(p_name.c_str());
  this->parse_stream(l_input_file);
  l_input_file.close();
}

#endif //_PARSER_BASE_TPP_
//EOF
