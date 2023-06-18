#include "interfacer.hpp"
#include "button.hpp"
#include "DIWindow/win.hpp"
#include "DIPhysics/collider.hpp"
#include "DIRenderer/renderer.hpp"
#include "DIParser/text.hpp"



namespace DI{

  extern WinData* eg_winData;
  /*****************************InvisibleButton********************************/

  InvisibleButton::InvisibleButton(glm::vec2 pos, glm::vec2 size){
    collider = new ColliderSquare();
    SetPosition(pos);
    SetSize(size);
    SetCollider(collider);
  }
  InvisibleButton::~InvisibleButton(){
    PhysicsHandler::UnSet(collider);
    callbacks.clear();
  }
  void InvisibleButton::Draw(){

  }
  void InvisibleButton::SetPosition(glm::vec2 pos){
    position = pos;
    SetCollider(collider);
  }
  void InvisibleButton::SetSize(glm::vec2 size){
    this->size = size;
    SetCollider(collider);
  }
  void InvisibleButton::SetCollider( ColliderSquare* collider){
    // Set needed properties for collider
    PhysicsHandler::SetColliderSquare(collider, glm::vec2(position.x-size.x/2,position.y-size.y/2), size);
    this->collider = collider;
  }
  void InvisibleButton::SetCallback( GUIState state, const std::function<void()> callback){
    // Do not need for check if some callback already exist. It will rewrite mem for new one
    callbacks.emplace(state,callback);
  }  
  void InvisibleButton::PressedByMouse(){
    // Check if button has some callback to call
    auto clb = callbacks.find(PRESSED);
    if ( clb != callbacks.end())
      clb->second();
  }
  void InvisibleButton::ReleasedByMouse(){
    auto clb = callbacks.find(RELEASED);
    if ( clb != callbacks.end())
      clb->second();
  }
  void InvisibleButton::HoveredByMouse(){
    auto clb = callbacks.find(HOVERED);
    if ( clb != callbacks.end())
      clb->second();
  }


  /*****************************PlaneButton********************************/

  PlaneButton::PlaneButton(glm::vec2 pos, glm::vec2 size) :
    InvisibleButton(pos,size)
  {
    primitive = new Reqtangle(POSITION_AND_COLORS);

    SetPosition(pos);
    SetSize(size);
    SetPrimitive(primitive);
  }
  PlaneButton::~PlaneButton(){
    delete primitive;
  }
  void PlaneButton::Draw(){
    RendererSingleton::GetInstance()->Draw(*primitive);    
  }
  void PlaneButton::SetPrimitive( Primitive* prmt){
    InterfacerSingleton* guiInstance = InterfacerSingleton::GetInstance();
    prmt->SetShader(guiInstance->GetShader(POSITION_AND_COLORS));
    prmt->SetCamera(*guiInstance->GetCamera());
    prmt->SetColor(glm::vec4(80/255.f, 91/255.f, 110/255.f, 1.0f));
    SetPosition(position);
    this->primitive = prmt;
  }  
  void PlaneButton::SetPosition( glm::vec2 pos ){
    primitive->ResetModelMatrix();
    primitive->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    primitive->SetScale(glm::vec3(size.x/2,size.y/2,0.f));
    this->position = pos;
    SetCollider(collider);
  }
  void PlaneButton::SetSize( glm::vec2 size ){
    primitive->ResetModelMatrix();
    primitive->SetPosition(glm::vec3(position.x,position.y,0.f));
    primitive->SetScale(glm::vec3(size.x/2, size.y/2,0.f));
    this->size = size;
    SetCollider(collider);
  }

  /*****************************TextButton********************************/

  TextButton::TextButton(const char* lable, glm::vec2 pos, glm::vec2 size) :
    PlaneButton(pos,size)
  {
    text = new Text(lable);
    padding = 25;

    SetPosition(pos);
    SetSize(size);
    SetText(lable);
  }
  TextButton::~TextButton(){
    delete this->text;
  }
  void TextButton::Draw(){
    RendererSingleton::GetInstance()->Draw(*primitive);    
    RendererSingleton::GetInstance()->Draw(*text);    
  }
  void TextButton::SetText( const char* str){
    text->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    text->SetPosition(position);
    text->ApplyText(str);
    size = text->GetSize();
    size.x += padding; 
    size.y += padding; 
    SetPosition(position);
    SetSize(size);
  }  
  void TextButton::SetPosition( glm::vec2 pos ){
    primitive->ResetModelMatrix();
    primitive->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    primitive->SetScale(glm::vec3(size.x/2,size.y/2,0.f));
    text->SetPosition(pos);
    this->position = pos;
    SetCollider(collider);
  }
  void TextButton::SetSize( glm::vec2 size ){
    primitive->ResetModelMatrix();
    primitive->SetPosition(glm::vec3(position.x,position.y,0.f));
    primitive->SetScale(glm::vec3(size.x/2, size.y/2,0.f));
    this->size = size;
    SetCollider(collider);
  }


}
