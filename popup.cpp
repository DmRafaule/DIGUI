#include "DIPrimitives/primitives.hpp"
#include "popup.hpp"
#include "DIWindow/win.hpp"
#include "interfacer.hpp"
#include "DIUtils/utils.hpp"

namespace DI{

  extern WinData* eg_winData;

  std::pair<bool,int*> Popup::isActive = {false,nullptr};

  Popup::Popup(glm::vec2 pos, glm::vec2 size){
    // Remove previose one and setup new one
    if (isActive.first)
      gui_handler->Remove(gui_handler->Get(*isActive.second));
    isActive = {true,&id};

    position = pos;
    size = size;
    padding = 10;
    marging = 20;
    // Set up background Primitive
    background = new Reqtangle(POSITION_AND_COLORS);
    background->SetShader(InterfacerSingleton::GetInstance()->GetShader(POSITION_AND_COLORS));
    background->SetCamera(*InterfacerSingleton::GetInstance()->GetCamera());
    background->SetColor(glm::vec4(1));
    // Set up position and size on creation
    SetPosition(pos); 
    SetSize(size);
  }
  Popup::~Popup(){
    // Allow creation new popup
    isActive = {false,nullptr};
    // Remove background Primitive
    delete background; 
  }
  void Popup::DrawThisContainer(){
    RendererSingleton::GetInstance()->Draw(*background);    
  }

  void Popup::SetPosition(glm::vec2 pos){
    this->position = pos;
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x,pos.y,0));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));
  }
  void Popup::SetSize(glm::vec2 size){
    this->size = size;
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0));
    background->SetScale(glm::vec3(size.x/2 + padding,size.y/2 + padding,0.f));
  }   
  void Popup::SetPadding (int padding){
    this->padding = padding;
    // Update size
    SetSize(size);
  }
  void Popup::UpdatePosition(){
    InterfacerSingleton* gui = InterfacerSingleton::GetInstance();
    int top = position.y + size.y/2 - padding/2.f;
    for (int i = 0; i < children.size(); i++){
      // First margin will be margin*0 others margin*1
      int margin = this->marging * (1%(i+1));
      InterfaceUnit* child = gui->Get(children[i]); 
      top = top - child->GetSize().y/2 - margin;
      child->SetPosition(glm::vec2(position.x,top));
    }
  }
  void Popup::UpdateSize(){
    int w = 0, h = 0;
    InterfacerSingleton* gui = InterfacerSingleton::GetInstance();
    for (int i = 0; i < children.size(); i++){
      InterfaceUnit* child = gui->Get(children[i]); 
      h += child->GetSize().y;
      w = Utils::GetMax<int>(w,child->GetSize().x);
    }
    w += padding + marging;
    h += padding + marging;
    size = glm::vec2(w,h);

    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));
  }
}
