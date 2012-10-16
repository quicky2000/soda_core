#ifndef _PARSER_BASE_TPP_
#define _PARSER_BASE_TPP_

//------------------------------------------------------------------------------
template <class T>
void parser_base<T>::add_analyzer(T & p_analyzer)
{
  m_analyzers.insert(&p_analyzer);
}

//------------------------------------------------------------------------------
template <class T>
void parser_base<T>::parse(const std::string & p_name)
{
  igzstream l_input_file(p_name.c_str());
    if(l_input_file==NULL)
      {
	std::cout << "ERROR : Unable to open file \"" << p_name << "\"" ;
	exit(-1);
      }
    this->parse_stream(l_input_file);
    l_input_file.close();
}

//------------------------------------------------------------------------------
template <class T>
void parser_base<T>::perform_analyze(const analyze_operation<T> & func)
{
  for(typename std::set<T*>::iterator l_iter = m_analyzers.begin();l_iter != m_analyzers.end() ; ++l_iter)
    {
      func(*l_iter);
    }
}

#endif //_PARSER_BASE_TPP_
//EOF
