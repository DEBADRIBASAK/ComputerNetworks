/* -*- C++ -*-  */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

CORBA::Short
Test_i::get_server_num (void)
{
  return 1;
}

void
Test_i::terminate (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server received terminate and going to exit\n"));
  exit (0);
}
