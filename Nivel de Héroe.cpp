/* ------------------------- */
/*       Nivel de Héroe      */
/* ------------------------- */

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

//Estructura para el arma del héroe.
struct arma {
  std::string nombre;
  int poder;
};
using unique_arma = std::unique_ptr<arma>;

// Se define 'arma' y 'unique_arma' antes de 'heroe' porque 'heroe' depende de ellos.
// En C++, los tipos deben declararse antes de ser utilizados como miembros.


//Estructura para el héroe.
struct heroe {
  std::string nombre;
  int nivel;
  int vida;
  unique_arma Arma;
};


using unique_heroe = std::unique_ptr<heroe>;

enum opciones {Crear_Heroe = 1, Equipar_Arma = 2, Subir_Nivel = 3, Mostrar_Heroe = 4, Salir = 5};

//------Prototipos de funciones-------//
unique_heroe crear_heroe();
void subir_nivel(unique_heroe& Heroe);
void mostrar_heroe(const unique_heroe& Heroe);
void equipar_arma(unique_heroe& Heroe);


//------Función principal-------//
int main() {
  int opcion;
  unique_heroe Heroe;


  do {
    std::cout << "\nMenú de opciones:\n";
    std::cout << "1. Crear héroe\n";
    std::cout << "2. Equipar arma\n";
    std::cout << "3. Subir nivel\n";
    std::cout << "4. Mostrar héroe\n";
    std::cout << "5. Salir\n";

    std::cout << "Ingrese una opción (1-5): ";
    std::cin >> opcion;
    std::cin.ignore();

    switch (opcion) {
      case Crear_Heroe:
        Heroe = crear_heroe();
        break;

      case Subir_Nivel:
        if (!Heroe) {
          std::cout << "\nDebe crear un héroe primero.\n";
          continue;
        }

        subir_nivel(Heroe);
        break;

      case Equipar_Arma:
        if (!Heroe) {
          std::cout << "\nDebe crear un héroe primero.\n";
          continue;
        }

        equipar_arma(Heroe);
        break;

      case Mostrar_Heroe:
        if (!Heroe) {
          std::cout << "\nDebe crear un héroe primero.\n";
          continue;
        }

        mostrar_heroe(Heroe);
        break;

      case Salir:
        std::cout << "\nFinalizando el ejecución...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;

      default:
        std::cout << "Opción inválida.\n";
        break;
    }
  } while (opcion != Salir);

  return 0;
}

//------Funciones-------//
unique_heroe crear_heroe() {
  unique_heroe nuevo_heroe = std::make_unique<heroe>();

  std::cout << "\nIngrese el nombre del héroe: ";
  std::getline(std::cin, nuevo_heroe->nombre);

  std::cout << "Ingrese el nivel del héroe: ";
  std::cin >> nuevo_heroe->nivel;

  std::cout << "Ingrese la vida del héroe: ";
  std::cin >> nuevo_heroe->vida;

  return nuevo_heroe;
}

void subir_nivel(unique_heroe& Heroe) {
  std::cout << "\nSubiendo de nivel..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  Heroe->nivel++;
  Heroe->vida += 20;

  std::cout << "Operación exitosa." << std::endl;
}

void mostrar_heroe(const unique_heroe& Heroe) {
  std::cout << "\nCargando parámetros del héroe..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "El nombre del héroe es: " << Heroe->nombre << std::endl;
  std::cout << "El nivel del héroe es: " << Heroe->nivel << std::endl;
  std::cout << "La vida del héroe es: " << Heroe->vida << std::endl;

  if (Heroe->Arma) {
    std::cout << "El nombre del arma es: " << Heroe->Arma->nombre << std::endl;
    std::cout << "El poder del arma es: " << Heroe->Arma->poder << std::endl;
  } else {
    std::cout << "El héroe no tiene arma equipada." << std::endl;
  }
}

void equipar_arma(unique_heroe& Heroe) {
  //Se crea el arma.
  unique_arma nueva_arma = std::make_unique<arma>();

  std::cout << "\nIngrese el nombre del arma: ";
  std::getline(std::cin, nueva_arma->nombre);

  std::cout << "Ingrese el poder del arma: ";
  std::cin >> nueva_arma->poder;


  Heroe->Arma = std::move(nueva_arma);
}
