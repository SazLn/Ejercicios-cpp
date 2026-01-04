//------------------------------------//
// Sistema de Suscripciones a Canales //
//------------------------------------//


#include <iostream>
#include <vector>
#include <memory>


//Prototipo de estructura. Para evitar conflictos de declaraciones.
struct Suscriptor;

//Estructura para los canales
struct Canal {
  std::string nombre;
  int total_videos;
  std::vector<std::weak_ptr<Suscriptor>> suscriptores;
};

//Estructura para los suscriptores
struct Suscriptor {
  std::string nombre;
  std::vector<std::weak_ptr<Canal>> canales;
};

using CanalPtr = std::shared_ptr<Canal>;
using SuscriptorPtr = std::shared_ptr<Suscriptor>;

//--Prototipos de funciones--//
void suscribir(SuscriptorPtr& suscriptor, CanalPtr& canal);
void mostrar_suscriptores(const CanalPtr& canal);
void mostrar_suscripciones(const SuscriptorPtr& suscriptor);

int main() {
  //---------Crear canales---------//
  auto programacion_cpp =std::make_shared<Canal>();
  auto cocina_facil =std::make_shared<Canal>();
  auto musica_electronica =std::make_shared<Canal>();

  //Asignar valores a los canales.
  programacion_cpp->nombre = "Programacion C++";
  programacion_cpp->total_videos = 50;

  cocina_facil->nombre = "Cocina Fácil";
  cocina_facil->total_videos = 120;

  musica_electronica->nombre = "Música Electrónica";
  musica_electronica->total_videos = 75;

  //---------Crear suscriptores---------//
  auto Ana = std::make_shared<Suscriptor>();
  auto Carlos = std::make_shared<Suscriptor>();
  auto Beatriz = std::make_shared<Suscriptor>();
  auto David = std::make_shared<Suscriptor>();

  //Asignar valores a los suscriptores.
  Ana->nombre = "Ana";
  Carlos->nombre = "Carlos";
  Beatriz->nombre = "Beatriz";
  David->nombre = "David";

  //Suscribir a los suscriptores a los canales.
  suscribir(Ana, programacion_cpp);
  suscribir(Carlos, cocina_facil);
  suscribir(Beatriz, musica_electronica);
  suscribir(David, programacion_cpp);
  suscribir(David, cocina_facil);
  suscribir(David, musica_electronica);

  //Eliminar canal de Cocina Fácil.
  cocina_facil.reset();
  mostrar_suscripciones(David);

  mostrar_suscriptores(musica_electronica);

  //Eliminar a Beatriz.
  Beatriz.reset();
  mostrar_suscriptores(musica_electronica);

  


  //Si vas a usar un switch, procura validar que los suscriptores no se suscriban a un canal que ya estén suscritos.

  return 0;
}

//------------Funciones------------//
//------------Funciones------------//
void suscribir(SuscriptorPtr& suscriptor, CanalPtr& canal) {
  //Validar si los punteros no son nulos.
  if (!canal || !suscriptor) {
    std::cout << "\n[!] Error: Canal o suscriptor inválidos.\n" << std::endl;
    return;
  }
  suscriptor->canales.emplace_back(canal);
  canal->suscriptores.emplace_back(suscriptor);
  std::cout << ">> Nuevo registro: " << suscriptor->nombre << " se ha suscrito a '" << canal->nombre << "'.\n";
}

void mostrar_suscriptores(const CanalPtr& canal) {
  if (!canal) {
    std::cout << "\n[!] Error: El canal consultado ya no existe.\n";
    return;
  }

  std::cout << "\n========================================\n";
  std::cout << " 📺  CANAL: " << canal->nombre << "\n";
  std::cout << "========================================\n";
  std::cout << " Suscriptores activos:\n";

  if (canal->suscriptores.empty()) {
    std::cout << "    (Sin suscriptores)\n";
  }

  for (const auto& suscripcion : canal->suscriptores) {
    //.expired() retorna true si el puntero ha sido destruido.
    if (suscripcion.expired()) {
      std::cout << "  ❌ [Inactivo] Usuario eliminado\n";
    } else {
      std::cout << "  ✅ " << suscripcion.lock()->nombre << "\n";
    }
  }
  std::cout << "========================================\n";
}

void mostrar_suscripciones(const SuscriptorPtr& suscriptor) {
  if (!suscriptor) {
    std::cout << "\n[!] Error: El suscriptor consultado no existe.\n";
    return;
  }
  
  std::cout << "\n========================================\n";
  std::cout << " 👤  PERFIL: " << suscriptor->nombre << "\n";
  std::cout << "========================================\n";
  std::cout << " Suscripciones:\n";

  if (suscriptor->canales.empty()) {
    std::cout << "    (Sin suscripciones)\n";
  }

  for (const auto& suscripcion : suscriptor->canales) {
    if (suscripcion.expired()) {
      std::cout << "  🚫 [Eliminado] Canal cerrado\n";
    } else {
      std::cout << "  📺 " << suscripcion.lock()->nombre << "\n";
    }
  }
  std::cout << "========================================\n";
}
