#include "lable.hpp"
#include "interfacer.hpp"
#include "DIPhysics/collider.hpp"
#include "DIParser/text.hpp"
#include "DIDebuger/log.hpp"


namespace DI{

  Lable::Lable(glm::vec2 pos, const char* str ){
    position = pos;
    padding = 25;
    collider = new ColliderSquare();
    SetCollider(collider);
    string = str;
    lable = new Text(string);
    
    background = new Reqtangle(POSITION_AND_COLORS);
    background->SetShader(InterfacerSingleton::GetInstance()->GetShader(POSITION_AND_COLORS));
    background->SetCamera(*InterfacerSingleton::GetInstance()->GetCamera());
    background->SetColor(glm::vec4(80/255.f, 91/255.f, 110/255.f, 1.0f));
    SetPrimitive(background);
    SetLable(string);
  }
  Lable::~Lable(){
    PhysicsHandler::UnSet(collider);
    delete background;
    delete lable;
  }
  void Lable::Draw(){
    RendererSingleton::GetInstance()->Draw(*background);
    RendererSingleton::GetInstance()->Draw(*lable);
  }
  void Lable::SetPosition( glm::vec2 pos ){
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    background->SetScale(glm::vec3(size.x/2,size.y/2,0.f));
    lable->SetPosition(pos);
    position = pos;
  }
  void Lable::SetSize( glm::vec2 size ){
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0.f));
    background->SetScale(glm::vec3(size.x/2, size.y/2,0.f));
    size = size;
  }
  void Lable::SetPrimitive( Primitive* prmt){
    background = prmt;
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x,position.y,0.f));
    background->SetScale(glm::vec3(size.x/2, size.y/2,0.f));
  }
  void Lable::SetLable( const char* str){
    lable->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    lable->SetPosition(glm::vec3(position.x,position.y,0));
    lable->ApplyText(str);
    size = lable->GetSize();
    SetPosition(position);
    SetSize(size);
  }
  void Lable::SetLable( Text* new_lable){
    // Free previous allocated memory
    if (new_lable->GetFont() != nullptr){
      delete lable;
      lable = new_lable;
    }
    DI_LOG_DEBUG("Fail to assign new Text object, no font are attached");
    lable->SetPosition(position);
    size = lable->GetSize();
    size.x += padding;
    size.y += padding;
    SetPosition(position);
    SetSize(size);
  }    

}
