#include "DIPrimitives/primitives.hpp"
#include "interfacer.hpp"
#include "checkbox.hpp"
#include "DIWindow/win.hpp"
#include "DIPhysics/collider.hpp"
#include "DIParser/text.hpp"



namespace DI{

  extern WinData* eg_winData;
  /********************InvisibleCheckbox*******************/
  InvisibleCheckbox::InvisibleCheckbox( glm::vec2 pos ){
    isSwitched  = true;
    // Set physical collider
    collider = new ColliderSquare();
    SetPosition(pos);
    SetSize(glm::vec2(30));
    SetCollider(collider);
  }
  InvisibleCheckbox::~InvisibleCheckbox(){
    PhysicsHandler::UnSet(collider);
    this->callbacks.clear();
  }
  void InvisibleCheckbox::Draw(){

  }
  void InvisibleCheckbox::PressedByMouse(){
    //switch value
    isSwitched = !isSwitched;
    // Check if button has some callback to call
    auto clb = callbacks.find(PRESSED);
    if ( clb != callbacks.end())
      clb->second();
  }
  void InvisibleCheckbox::SetPosition( glm::vec2 pos ){
    this->position = pos;
    SetCollider(collider);
  }
  void InvisibleCheckbox::SetSize( glm::vec2 size ){
    this->size = size;
    SetCollider(collider);
  }
  void InvisibleCheckbox::SetCollider( ColliderSquare* cldr){
    // Set needed properties for collider
    PhysicsHandler::SetColliderSquare(cldr, glm::vec2(position.x-size.x/2,position.y-size.y/2), size);
    this->collider = cldr;
  }
  
  void InvisibleCheckbox::SetCallback( GUIState state, const std::function<void()> callback){
    // Do not need for check if some callback already exist. It will rewrite mem for new one
    callbacks.emplace(state,callback);
  }  

  /********************PlaneCheckbox*******************/


  PlaneCheckbox::PlaneCheckbox( glm::vec2 pos ) : 
    InvisibleCheckbox(pos)
  {
    box = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(box);
    box->SetColor(glm::vec4(80/255.f, 91/255.f, 110/255.f,1));

    cross = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(cross);
    cross->SetColor(glm::vec4(eg_winData->bg.r,eg_winData->bg.g,eg_winData->bg.b,1));
    
    SetPosition(pos);
    SetSize(glm::vec2(15));
  }
  PlaneCheckbox::~PlaneCheckbox(){
    delete box;
    delete cross;
  }
  void PlaneCheckbox::Draw(){
    if (!isSwitched)
      RendererSingleton::GetInstance()->Draw(*cross);    
    RendererSingleton::GetInstance()->Draw(*box);    
  }
  void PlaneCheckbox::SetPrimitive(Primitive* in){
    in->SetShader(InterfacerSingleton::GetInstance()->GetShader(POSITION_AND_COLORS));
    in->SetCamera(*InterfacerSingleton::GetInstance()->GetCamera());
    in->SetPosition(glm::vec3(position.x,position.y,0));
    in->SetScale(glm::vec3(size.x/2,size.y/2,0));
  }
  void PlaneCheckbox::SetPosition(glm::vec2 pos){
    box->ResetModelMatrix();
    box->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    box->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    cross->ResetModelMatrix();
    cross->SetPosition(glm::vec3(pos.x,pos.y,0.f));
    cross->SetScale(glm::vec3(size.x/2.5,size.y/2.5,0.f));

    this->position = pos;
    SetCollider(collider);
  }
  void PlaneCheckbox::SetSize(glm::vec2 size){
    box->ResetModelMatrix();
    box->SetPosition(glm::vec3(position.x,position.y,0.f));
    box->SetScale(glm::vec3(size.x/2, size.y/2,0.f));

    cross->ResetModelMatrix();
    cross->SetPosition(glm::vec3(position.x,position.y,0.f));
    cross->SetScale(glm::vec3(size.x/2.5, size.y/2.5,0.f));

    this->size = size;
    SetCollider(collider);
  }
  /********************TextCheckbox*******************/

  TextCheckbox::TextCheckbox( const char* const lable, glm::vec2 pos ) : 
    PlaneCheckbox(pos)
  {
    text = new Text(lable);
    background = new Reqtangle(POSITION_AND_COLORS);
    SetPrimitive(background);
    background->SetColor(glm::vec4(0/255.f, 0/255.f, 110/255.f,1.0));
    padding = 5;

    SetPosition(pos);
    SetSize(size);
    SetText(lable);
    SetCollider(collider);
  }
  TextCheckbox::~TextCheckbox(){
    delete text;
    delete background;
  }
  void TextCheckbox::Draw(){
    if (!isSwitched)
      RendererSingleton::GetInstance()->Draw(*cross);    
    RendererSingleton::GetInstance()->Draw(*box);    
    RendererSingleton::GetInstance()->Draw(*background);    
    RendererSingleton::GetInstance()->Draw(*text);    
  }
  void TextCheckbox::SetPosition(glm::vec2 pos){
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(pos.x ,pos.y, 0.f));
    background->SetScale(glm::vec3(size.x + (text->GetSize().x)/2,text->GetSize().y/2,0.f));

    box->ResetModelMatrix();
    box->SetPosition(glm::vec3(pos.x - text->GetSize().x/2 - size.x/2 - padding,pos.y,0.f));
    box->SetScale(glm::vec3(size.x/2,size.y/2,0.f));

    cross->ResetModelMatrix();
    cross->SetPosition(glm::vec3(pos.x - text->GetSize().x/2 - size.x/2 - padding,pos.y,0.f));
    cross->SetScale(glm::vec3(size.x/2.5,size.y/2.5,0.f));


    text->SetPosition(glm::vec2(pos.x,pos.y));

    this->position = glm::vec2(pos.x - text->GetSize().x/2 - size.x/2 - padding,pos.y);
    SetCollider(collider);
  }
  void TextCheckbox::SetSize(glm::vec2 size){
    background->ResetModelMatrix();
    background->SetPosition(glm::vec3(position.x ,position.y, 0.f));
    background->SetScale(glm::vec3(size.x + (text->GetSize().x)/2,(size.y + text->GetSize().y)/2 ,0.f));

    box->ResetModelMatrix();
    box->SetPosition(glm::vec3(position.x - text->GetSize().x/2 - size.x/2 - padding,position.y,0.f));
    box->SetScale(glm::vec3(size.x/2, size.y/2,0.f));

    cross->ResetModelMatrix();
    cross->SetPosition(glm::vec3(position.x - text->GetSize().x/2 - size.x/2 - padding,position.y,0.f));
    cross->SetScale(glm::vec3(size.x/2.5, size.y/2.5,0.f));


    this->size = size;
    SetCollider(collider);
  }
  void TextCheckbox::SetText(const char* const text){
    this->text->ApplyFont(InterfacerSingleton::GetInstance()->GetFont());
    this->text->SetPosition(position);
    this->text->ApplyText(text);
    SetPosition(position);
  }
  void TextCheckbox::SetPadding(int padding){
    this->padding = padding;
    SetPosition(position);
  }
  glm::vec2 TextCheckbox::GetSize(){
    return glm::vec2(background->GetScale().x*2,background->GetScale().y*2); 
  }
  glm::vec2 TextCheckbox::GetPosition(){
    return glm::vec2(background->GetPosition().x,background->GetPosition().y);
  }
  // TODO: maybe expand collider on text also 

}


