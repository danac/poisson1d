#include "muParserTest.h"

int main(int argc, char* argv[])
{
    mu::console() << _T( "-----------------------------------------------------------\n");
    mu::console() << _T( "Running test suite:\n\n");

    // Skip the self test if the value type is set to an integer type.
    if (mu::TypeInfo<mu::value_type>::IsInteger())
    {
        mu::console() << _T( "  Test skipped: integer data type are not compatible with the unit test!\n\n");
    }
    else
    {
        mu::Test::ParserTester pt;
        pt.Run();
    }

    return 0;
}
