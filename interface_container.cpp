#include "interface_container.hpp"
#include "interfacer.hpp"
#include "DIPhysics/collider.hpp"
#include "DIDebuger/debuger.hpp"
#include "DIDebuger/log.hpp"


namespace DI{

  InterfaceContainer::InterfaceContainer(){
    DI_LOG_DEBUG("Create a GUI container");
    // Get instance of InterfacerSingleton
    gui_handler = InterfacerSingleton::GetInstance();
    // Set up collider
    collider = new ColliderSquare();
    PhysicsHandler::SetColliderSquare(collider,position,size);
  }
  InterfaceContainer::~InterfaceContainer(){
    DI_LOG_DEBUG("Remove a GUI container");
    // Remove collider
    PhysicsHandler::UnSet(collider);
    // Remove all children
    Clear();
  }
  void InterfaceContainer::Draw(){
    DrawThisContainer();
  }
  // Set positon for InterfaceContainer and all children
  void InterfaceContainer::SetPosition( glm::vec2 pos ){
    this->position = pos;
    for (int i = 0; i < children.size(); i++){
      InterfaceUnit* unit = gui_handler->Get(i);
      unit->SetPosition(pos + unit->GetPosition()); 
    }
  }
  // Set size for InterfaceContainer and all children
  void InterfaceContainer::SetSize( glm::vec2 size){
    this->size = size;
    for (int i = 0; i < children.size(); i++){
      InterfaceUnit* unit = gui_handler->Get(i);
      unit->SetSize(size + unit->GetSize()); 
    }
  }
  // Get Actual InterfaceUnit from InterfacerSingleton
  InterfaceUnit* InterfaceContainer::Get(const int id){
    int size = gui_handler->GetElements().size(); 
    if (id < size && id >= 0)
      return gui_handler->Get(id);
    return this;
  }
  // Adding unit to container
  void InterfaceContainer::Add(InterfaceUnit* unit, ViewportsSupportedID viewport_id){
    // First update position of new added InterfaceUnit
    unit->SetPosition(this->position + unit->GetPosition());
    // Actual add InterfaceUnit to pool of InterfaceUnits
    gui_handler->Add(unit,viewport_id);
    // Set flag that this InterfaceUnit was added through container and not direct through InterfacerSingleton
    unit->isInContainer = true;
    // Save id of added InterfaceUnit
    children.emplace_back(unit->id);
  }
  // Removing unit from container
  void InterfaceContainer::Remove(InterfaceUnit* unit){
    int size = children.size();
    for (int i = 0; i < size; i++){
      if (children[i] == unit->id ){
        gui_handler->Remove(unit);
        break;
      }
    }
  }
  // Clear pool and list of InterfaceUnit's ids
  void InterfaceContainer::Clear(){
    int size = children.size();
    for (int i = 0; i < size; i++){
      gui_handler->Remove(gui_handler->Get(children[i]));
    }
    children.clear();
  }   
}
