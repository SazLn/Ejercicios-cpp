/* --------------------------------------*/
/*  Sistema de Biblioteca con Préstamos  */
/* ------------------------------------- */

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

//Prototipos de [struct]
struct libro;
struct lector;

using shared_libro = std::shared_ptr<libro>;
using shared_lector = std::shared_ptr<lector>;

//Estructuras
struct libro {
  std::string titulo;
  std::string autor;
  int ano_publicacion;
  std::vector<std::weak_ptr<lector>> prestado_a; //Lectores a los que se les prestó el libro.
};

struct lector {
  std::string nombre;
  int id;
  std::vector<std::weak_ptr<libro>> libros_prestados; //Libros prestados al lector.
  std::weak_ptr<lector> recomendado_por; //Lector que le recomendó entrar a la biblioteca.
};

//------Prototipos de funciones------//
void prestar_libro(shared_libro libro, shared_lector lector);
void devolver_libro(shared_lector lector, shared_libro libro);
void recomendar_lector(shared_lector recomendado, shared_lector recomendador);
void mostrar_libros_prestados(shared_lector Lector);
void mostrar_lectores_de_un_libro(shared_libro Libro);
void mostrar_libros_huerfanos(std::vector<shared_libro> libros);
void cadena_recomendaciones(shared_lector Lector);



//-----Función principal-----//
int main() {
  //Temp: Algoritmo para prestar libros.

  //Crear los libros
  std::vector<shared_libro> libros;
  shared_libro quijote(new libro{"Don Quijote", "Miguel de Cervantes", 1605});
  shared_libro cien_anos(new libro{"Cien años de soledad", "Gabriel García Márquez", 1967});
  shared_libro mil_novecientos(new libro{"1984", "George Orwell", 1949});
  shared_libro fisica(new libro{"Física", "Isaac Newton", 1687});

  libros.push_back(quijote);
  libros.push_back(cien_anos);
  libros.push_back(mil_novecientos);
  libros.push_back(fisica);

  

  //Crear los lectores
  
  //Crear un lector para probar el algoritmo de prestar libros.
  shared_lector lector1(new lector{"Juan", 1});
  shared_lector lector2(new lector{"Pedro", 2});
  shared_lector lector3(new lector{"Elver", 3});

  recomendar_lector(lector1, lector2);
  recomendar_lector(lector2, lector3);

  prestar_libro(quijote, lector1);
  prestar_libro(quijote, lector2);
  prestar_libro(quijote, lector3);

  mostrar_libros_prestados(lector1);
  mostrar_libros_prestados(lector2);

  //Se devuelve un libro.
  devolver_libro(lector2, quijote);

  mostrar_lectores_de_un_libro(quijote);

  //Eliminar a Elver.
  lector3.reset();
  mostrar_libros_huerfanos(libros);

  cadena_recomendaciones(lector1);

  //Continuar con una función que muestre la cadena de recomendaciones de los lectores.


  

  return 0;
}

//----------Funciones----------//
void prestar_libro(shared_libro libro, shared_lector lector) {
  //Añadir libro al vector del lector.
  lector->libros_prestados.emplace_back(libro);

  //Añadir lector al vector del libro.
  libro->prestado_a.emplace_back(lector);
}

void devolver_libro(shared_lector Lector, shared_libro Libro) {
  //Eliminar libro del vector del lector.
  Lector->libros_prestados.erase(std::remove_if(Lector->libros_prestados.begin(), Lector->libros_prestados.end(), [Libro](const auto& libro){
    //La lambda captura el libro a devolver y luego lo compara con cada libro prestado.
    return libro.lock() == Libro;
  }));

  //Eliminar lector del vector del libro.
  Libro->prestado_a.erase(std::remove_if(Libro->prestado_a.begin(), Libro->prestado_a.end(), [Lector](const auto& lector){
    //La lambda captura el lector a devolver y luego lo compara con cada lector prestado.
    return lector.lock() == Lector;
  }));
}

//Esta función se ejecuta en caso de que el recomendado si tenga recomendador, de otro modo no debería.
void recomendar_lector(shared_lector recomendado, shared_lector recomendador) {
  recomendado->recomendado_por = recomendador;
}

void mostrar_libros_prestados(shared_lector Lector) {
  std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
  std::cout << "║  📚 Libros prestados a " << Lector->nombre << std::endl;
  std::cout << "╠══════════════════════════════════════╣" << std::endl;
  if (Lector->libros_prestados.empty()) {
    std::cout << "║  (Sin libros prestados)" << std::endl;
  } else {
    for (const auto& libro : Lector->libros_prestados) {
      std::cout << "║  → " << libro.lock()->titulo << std::endl;
    }
  }
  std::cout << "╚══════════════════════════════════════╝\n" << std::endl;
}

void mostrar_lectores_de_un_libro(shared_libro Libro){
  std::cout << "\n┌──────────────────────────────────────┐" << std::endl;
  std::cout << "│  👥 Lectores de: \"" << Libro->titulo << "\"" << std::endl;
  std::cout << "├──────────────────────────────────────┤" << std::endl;
  bool tiene_lectores = false;
  for (const auto& lector : Libro->prestado_a) {
    if (lector.expired()) {
      std::cout << "│  ⚠️  Lector eliminado (libro huérfano)" << std::endl;
      continue;
    }
    std::cout << "│  • " << lector.lock()->nombre << std::endl;
    tiene_lectores = true;
  }
  if (!tiene_lectores && Libro->prestado_a.empty()) {
    std::cout << "│  (Ningún lector tiene este libro)" << std::endl;
  }
  std::cout << "└──────────────────────────────────────┘\n" << std::endl;
}

void mostrar_libros_huerfanos(std::vector<shared_libro> Libros) {
  std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
  std::cout << "║  🔍 Búsqueda de Libros Huérfanos     ║" << std::endl;
  std::cout << "╠══════════════════════════════════════╣" << std::endl;
  bool hay_huerfanos = false;
  for (const auto& libro : Libros) {
    for (const auto& lector : libro->prestado_a) {
      if (lector.expired()) {
        std::cout << "║  ⚠️  " << libro->titulo << std::endl;
        hay_huerfanos = true;
        break;
      }
    }
  }
  if (!hay_huerfanos) {
    std::cout << "║  ✅ No hay libros huérfanos" << std::endl;
  }
  std::cout << "╚══════════════════════════════════════╝\n" << std::endl;
}

void cadena_recomendaciones(shared_lector Lector) {
  shared_lector lector_actual = Lector;
  
  std::cout << "\n┌──────────────────────────────────────┐" << std::endl;
  std::cout << "│  🔗 Cadena de Recomendaciones        │" << std::endl;
  std::cout << "├──────────────────────────────────────┤" << std::endl;
  
  //Para validar que el lector ingresado es válido.
  if (!lector_actual) {
    std::cout << "│  ❌ Lector eliminado o inactivo." << std::endl;
    std::cout << "└──────────────────────────────────────┘\n" << std::endl;
    return;
  } else if (!lector_actual->recomendado_por.lock()) {
    std::cout << "│  ℹ️  " << lector_actual->nombre << " no tiene recomendador" << std::endl;
    std::cout << "│     o este ha sido eliminado." << std::endl;
    std::cout << "└──────────────────────────────────────┘\n" << std::endl;
    return;
  }
  
  std::cout << "│  Lector: " << Lector->nombre << std::endl;
  std::cout << "│" << std::endl;
  
  while (lector_actual->recomendado_por.lock()) {
    std::cout << "│  " << lector_actual->nombre << std::endl;
    std::cout << "│    ↓ recomendado por" << std::endl;
    lector_actual = lector_actual->recomendado_por.lock();
  }
  std::cout << "│  " << lector_actual->nombre << " (origen)" << std::endl;
  std::cout << "└──────────────────────────────────────┘\n" << std::endl;
}