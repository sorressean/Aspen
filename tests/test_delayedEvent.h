#include <Frisk.h>
#include "../src/delayedEvent.h"

namespace FriskTests
{
    DEF_TEST(DelayedEvent_DelayedEvent_Constructor_test);
    DEF_TEST(DelayedEvent_DelayedEvent_Constructor_test)
    {
      BEGIN_TEST(self);
      self.setOption(FRISK_OPTION_PENDING, true);
      /* implement your tests for DelayedEvent::DelayedEvent::Constructor! */
    }

    DEF_TEST(DelayedEvent_DelayedEvent_Constructor2_test);
    DEF_TEST(DelayedEvent_DelayedEvent_Constructor2_test)
    {
      BEGIN_TEST(self);
      self.setOption(FRISK_OPTION_PENDING, true);
      /* implement your tests for DelayedEvent::DelayedEvent::Constructor2! */
    }

    DEF_TEST(DelayedEvent_SetDelay_test);
    DEF_TEST(DelayedEvent_SetDelay_test)
    {
      BEGIN_TEST(self);
      self.setOption(FRISK_OPTION_PENDING, true);
      /* implement your tests for DelayedEvent::SetDelay! */
    }

    DEF_TEST(DelayedEvent_Invoke_test);
    DEF_TEST(DelayedEvent_Invoke_test)
    {
      BEGIN_TEST(self);
      self.setOption(FRISK_OPTION_PENDING, true);
      /* implement your tests for DelayedEvent::Invoke! */
    }

}