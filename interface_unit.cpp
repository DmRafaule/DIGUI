#include "interface_unit.hpp"
#include "interfacer.hpp"
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"

namespace DI{

  int InterfaceUnit::next_id = 0;

  InterfaceUnit::InterfaceUnit(){
    DI_LOG_DEBUG("Create a GUI unit"); 
  }
  InterfaceUnit::~InterfaceUnit(){
    DI_LOG_DEBUG("Remove a GUI unit"); 
  }

}
