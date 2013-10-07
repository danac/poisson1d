const char* greet()
{
   return "hello, world";
}

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(test_boost_python)
{
    using namespace boost::python;
    def("greet", greet);
}
