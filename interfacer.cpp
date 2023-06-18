#include "interfacer.hpp"
#include "interface_unit.hpp"

#include "DIDebuger/log.hpp"
#include "DIPrimitives/primitives.hpp"
#include "DIPhysics/collider.hpp"
#include "DIWindow/win.hpp"
#include "DIEvents/event.hpp"
#include "DIWindow/viewport.hpp"
#include "DIParser/font.hpp"



/********************************* InterfacerSingleton class *********************************/
namespace DI{

  extern WinData* eg_winData;
  extern EventData* eg_eventData;
  InterfacerSingleton* InterfacerSingleton::m_instance = 0;

  void updateMouse(){
    for (const auto& viewport : ViewportHandler::GetAllViewports()){
      ColliderSquare cldr;
      PhysicsHandler::SetColliderPoint(InterfacerSingleton::GetInstance()->GetMouseCollider(), glm::vec2(eg_eventData->mousePos.x,eg_eventData->mousePos.y));
      PhysicsHandler::SetColliderSquare(&cldr, glm::vec2(viewport->x,viewport->y), glm::vec2(viewport->w,viewport->h));
      // Check if mouse on viewport square
      if (PhysicsHandler::IsCollide_Point_Square(InterfacerSingleton::GetInstance()->GetMouseCollider(), &cldr)){
        // Set relitive to viewport mouse position
        InterfacerSingleton::GetInstance()->GetMousePosition().x = InterfacerSingleton::GetInstance()->GetMouseCollider()->pos.x - cldr.pos.x; 
        InterfacerSingleton::GetInstance()->GetMousePosition().y = InterfacerSingleton::GetInstance()->GetMouseCollider()->pos.y - cldr.pos.y; 
        PhysicsHandler::SetColliderPoint(InterfacerSingleton::GetInstance()->GetMouseCollider(),InterfacerSingleton::GetInstance()->GetMousePosition());
        // Break from loop because we want to save updated collider
        break;
      }
    }
  }

  void mousePressed(){
    updateMouse();
    for (auto& unit : InterfacerSingleton::GetInstance()->GetElements()){
      if (PhysicsHandler::IsCollide_Point_Square(InterfacerSingleton::GetInstance()->GetMouseCollider(), unit->GetCollider())){
        unit->PressedByMouse();
        break;
      }
      else{
        unit->PressedOutSide();
      }
    }
  }

  void mouseMoved(){
    updateMouse();
    for (auto& unit : InterfacerSingleton::GetInstance()->GetElements()){
      if (PhysicsHandler::IsCollide_Point_Square(InterfacerSingleton::GetInstance()->GetMouseCollider(), unit->GetCollider())){
        unit->HoveredByMouse();
      }
    } 
  }

  void mouseReleased(){
    updateMouse();
    for (auto& unit : InterfacerSingleton::GetInstance()->GetElements()){
      unit->ReleasedByMouse();
    } 
  }

  void keyPressed(){
    for (auto& unit : InterfacerSingleton::GetInstance()->GetElements()){
      unit->PressedByKey();
    }
  }

  


  void InterfacerSingleton::Init(){
    DI_LOG_TRACE("InterfacerSingleton say: Init GUI.");
    m_instance = new InterfacerSingleton();
    // Set events for UI
    DI::EventHandler::Set(DI::EventType::MOUSE_PRESSED,std::bind(mousePressed));
    DI::EventHandler::Set(DI::EventType::MOUSE_RELEASED,std::bind(mouseReleased));
    DI::EventHandler::Set(DI::EventType::MOUSE_MOVED,std::bind(mouseMoved));
    DI::EventHandler::Set(DI::EventType::KEY_PRESSED,std::bind(keyPressed));
    // Set shaders
    m_instance->flat_shader = new Shader();
    ShaderHandler::Set(*m_instance->flat_shader,"assets/shaders/pPosCol.vert","assets/shaders/pPosCol.frag");
    m_instance->texture_shader = new Shader();
    ShaderHandler::Set(*m_instance->texture_shader,"assets/shaders/pPosColTex.vert","assets/shaders/pPosColTex.frag");
    // Set default camera for GUI
    m_instance->camera = new OrthographicCamera();
    CameraHandler::Set(*m_instance->camera);
    // Set default font
    m_instance->font = new Font();
    FontHandler::Set(m_instance->font,"assets/fonts/Ubuntu-R.ttf",18);
    FontHandler::SetProjection(m_instance->font,m_instance->camera->GetProjection());
    // Get instance of RendererSingleton
    m_instance->rendererSingleton = RendererSingleton::GetInstance();
    // init mouse point collider
    m_instance->mouse_collider = new ColliderPoint();
    PhysicsHandler::SetColliderPoint(m_instance->mouse_collider, m_instance->mouse_position);

  }
  void InterfacerSingleton::Kill(){
    delete m_instance->flat_shader;
    delete m_instance->texture_shader;
    PhysicsHandler::UnSet(m_instance->mouse_collider);
    CameraHandler::UnSet(*m_instance->camera);
    FontHandler::UnSet(m_instance->font);
    m_instance->Clear();
    delete m_instance;

    DI_LOG_TRACE("InterfaceUnit say: Kill GUI.");
  }

  InterfaceUnit* InterfacerSingleton::Get(const int id) { 
    int size = units.size();
    for (int i = 0; i < size; i++){
      // Get unit by it's unique id
      if (units[i]->id == id){
        return units[i]; 
      }
    }
    return units[0];
  } 
  void InterfacerSingleton::Add(InterfaceUnit* unit, ViewportsSupportedID viewport_id){
    unit->viewport_id = viewport_id;
    unit->id = unit->next_id;
    // For new elements increase counter
    // TODO: Thinking about how to reuse passed numbers
    unit->next_id++;
    // Direct added to pool so it is not in container
    // Either InterfaceUnit added directly or InterfaceContainer by it self
    unit->isInContainer = false;
    // Add to pool
    eg_winData->viewports[viewport_id]->units.emplace_back(unit);
    m_instance->units.emplace_back(unit);
  }

  void InterfacerSingleton::Remove(InterfaceUnit* unit){
    int size = units.size();
    for (int i = 0; i < size; i++){
      // If ids are identical then remove
      if (units[i]->id == unit->id){
        // update pool
        units.erase(units.begin() + i);
        // free memory
        delete unit;
        // quit loop
        break;
      }
    }
  }

  void InterfacerSingleton::Clear(){
    for (int i = 0; i < units.size(); i++)
      if (!units[i]->isInContainer){
        // free memory
        delete units[i];
        // set counter to start(because std::vector will be update and have new size)
        // and also next unit will be have 0 id in pool
        i = 0;
        // update pool
        units.erase(units.begin() + i);
      }
    units.clear();
  }

  // Return a instance of InterfacerSingleton
  InterfacerSingleton* InterfacerSingleton::GetInstance(){ 
    if (m_instance == 0){
      m_instance = new InterfacerSingleton();
    }
    return m_instance; 
  }
  
  Shader* InterfacerSingleton::GetShader(int type){
    switch(type){
      case POSITION_AND_COLORS:{
        return flat_shader;
      } 
      case POSITION_AND_COLORS_AND_TEXTURES:{
        return texture_shader;
      } 

    }
    return flat_shader;
  }

}
