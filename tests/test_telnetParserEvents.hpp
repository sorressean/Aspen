#include <Frisk.h>
#include "../src/telnetParserEvents.hpp"

namespace FriskTests
{
    DEF_TEST(OptionEventArgs_OptionEventArgs_Constructor_test);
    DEF_TEST(OptionEventArgs_OptionEventArgs_Constructor_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for OptionEventArgs::OptionEventArgs::Constructor! */
    }

    DEF_TEST(OptionEventArgs_GetOption_test);
    DEF_TEST(OptionEventArgs_GetOption_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for OptionEventArgs::GetOption! */
    }

    DEF_TEST(OptionEventArgs_GetCommand_test);
    DEF_TEST(OptionEventArgs_GetCommand_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for OptionEventArgs::GetCommand! */
    }

    DEF_TEST(NegotiationEventArgs_NegotiationEventArgs_Constructor_test);
    DEF_TEST(NegotiationEventArgs_NegotiationEventArgs_Constructor_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for NegotiationEventArgs::NegotiationEventArgs::Constructor! */
    }

    DEF_TEST(NegotiationEventArgs_GetOption_test);
    DEF_TEST(NegotiationEventArgs_GetOption_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for NegotiationEventArgs::GetOption! */
    }

    DEF_TEST(NegotiationEventArgs_GetData_test);
    DEF_TEST(NegotiationEventArgs_GetData_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for NegotiationEventArgs::GetData! */
    }

    DEF_TEST(NegotiationEventArgs_GetSize_test);
    DEF_TEST(NegotiationEventArgs_GetSize_test)
    {
      BEGIN_TEST(self);
      TEST_OPTION(self, FRISK_OPTION_PENDING, true);
      /* implement your tests for NegotiationEventArgs::GetSize! */
    }

}