/* --------------------------------------- */
//   Sistema de Amigos en una Red Social   //
/* --------------------------------------- */

#include <iostream>
#include <vector>
#include <memory>

//Estructura para usuario.
struct usuario {
  std::string nombre;
  int edad;
  std::shared_ptr<usuario> mejor_amigo;
  std::vector<std::weak_ptr<usuario>> amigos;
};



//------------ Prototipos de funciones ------------//
void mostrar_amigos(const std::shared_ptr<usuario>& Usuario, std::string nombre_eliminado);


//Función principal.
int main() {
  //Crear usuarios.
  std::shared_ptr<usuario> Ana = std::make_shared<usuario>();
  std::shared_ptr<usuario> Carlos = std::make_shared<usuario>();
  std::shared_ptr<usuario> Beatriz = std::make_shared<usuario>();

  //Asignar valores.
  Ana->nombre = "Ana";
  Ana->edad = 23;
  Ana->mejor_amigo = Carlos;
  Ana->amigos.emplace_back(Carlos); //emplace_back() constuye el weak_ptr en el vector. (mejor que push_back())
  Ana->amigos.emplace_back(Beatriz);
  

  Carlos->nombre = "Carlos";
  Carlos->edad = 22;
  Carlos->mejor_amigo = Ana;
  Carlos->amigos.emplace_back(Ana);
  Carlos->amigos.emplace_back(Beatriz);
 

  Beatriz->nombre = "Beatriz";
  Beatriz->edad = 24;
  Beatriz->amigos.emplace_back(Ana);
  Beatriz->amigos.emplace_back(Carlos);
  
  mostrar_amigos(Ana, "");

  std::cout << "Carlos ha sido eliminado de la red social.\n";
  std::string nombre_eliminado = Carlos->nombre;
  Ana->mejor_amigo.reset();
  Carlos.reset();
  mostrar_amigos(Ana, nombre_eliminado);
  

  
  return 0;
}

void mostrar_amigos(const std::shared_ptr<usuario>& Usuario, std::string nombre_eliminado) {
  std::cout << "Amigos de " << Usuario->nombre << ":\n";

  for (const auto amigo : Usuario->amigos) {
    //Siempre se usa .lock() para obtener el puntero al objeto al que apunta el weak_ptr.
    auto ptr_amigo = amigo.lock();

    //Siempre hay que asegurarse de que el puntero no sea nulo antes de utilizarlo.
    if (ptr_amigo != nullptr) {
      //REvisar primero si el mejor amigo existe.
      if (Usuario->mejor_amigo != nullptr && ptr_amigo->nombre == Usuario->mejor_amigo->nombre) {
        std::cout << "- " << ptr_amigo->nombre << " (mejor amigo)\n";
      } else {
        std::cout << "- " << ptr_amigo->nombre << "\n";
      }
    } else {
      std::cout << "- " << nombre_eliminado << " (usuario eliminado o inactivo)\n";
    }
    
  }
}
