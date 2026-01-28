/* -------------------------------- */
/* Sistema de Gestión de Biblioteca */
/* -------------------------------- */

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cctype>
#include <memory>
#include <iomanip>

//Estructura para cada libro del catálogo.
struct libro {
  int id;
  std::string titulo;
  std::string autor;
  int ano_publicacion;
  bool disponible;
};



//______________________________________________________//

   //------------- Prototipos de Función -------------//
//______________________________________________________//

void mostrar_catalogo(const std::vector<libro>& libros);
void mostrar_libros_disponibles(const std::vector<libro>& libros);
void contar_libros_por_decada(const std::vector<libro>& libros);
void mostrar_catalogo_alreves(std::vector<libro>& libros);

//+++++++ funciones de acción +++++++//.
void prestar_libro(std::vector<libro>& libros, std::list<std::string>& lista);
void devolver_libro(std::vector<libro>& catalogo, std::list<std::string>& lectores);
void libros_por_autor(std::vector<libro> libros);
void mostrar_siguiente_espera(const std::list<std::string>& lectores);

//Para eliminar libros duplicados
void ordenar_vector(std::vector<libro>& libros);
void eliminar_duplicados(std::vector<libro>& libros);

//Añadir función para msotrar estadísticas de los libros.
void analizar_estadisticas(const std::vector<libro>& libros, const std::list<std::string>& lista_espera);


/* *********************************** */
/* ******** Función principal ******** */
/* *********************************** */
int main() {
  //Crear libros;
  std::vector<libro> catalogo;
  catalogo.push_back({1, "Don Quijote", "Miguel de Cervantes", 1605, true});
  catalogo.push_back({2, "Cien Años de Soledad", "Gabriel García Márquez", 1967, false});
  catalogo.push_back({3, "1984", "George Orwell", 1949, true});
  catalogo.push_back({4, "El Principito", "Antoine de Saint-Exupéry", 1943, true});
  catalogo.push_back({5, "Orgullo y Prejuicio", "Jane Austen", 1813, false});
  catalogo.push_back({6, "Fahrenheit 451", "Ray Bradbury", 1953, true});
  catalogo.push_back({7, "El Hobbit", "J.R.R. Tolkien", 2012, false});


  //Lista de espera. Cada lector en esta lista está esperando un libro que no está disponible.
  //Una vez que el libro está disponible, se presta a la primera persona en la lista.
  //esta lista es general, así que si cualquier libro está disponible, se presta automáticamente al primero en lista.

  std::list<std::string> lista_espera = {
    "Elver Galarga", //Primero en la lista
    "Elva Ginon", //Segunda.
    "Rosa Melano", //Tercera.
    "Armando Bronca", //Cuarto.
    "Aquiles Bailo", //Quinto.
    "Dolores Delano" //Sexto.
  }; 

  
  mostrar_catalogo(catalogo);
  mostrar_catalogo_alreves(catalogo);
  mostrar_libros_disponibles(catalogo);
  contar_libros_por_decada(catalogo); 

  prestar_libro(catalogo, lista_espera);

  devolver_libro(catalogo, lista_espera);

  libros_por_autor(catalogo);
  mostrar_siguiente_espera(lista_espera);

  eliminar_duplicados(catalogo);
  // for (const auto& libro : catalogo) {
  //   std::cout << "[" << libro.id << "] " 
  //             << libro.titulo << " - " 
  //             << libro.autor << " (" 
  //             << libro.ano_publicacion << ") - "
  //             << (libro.disponible ? "Disponible" : "No disponible") 
  //             << std::endl;
  // }

  analizar_estadisticas(catalogo, lista_espera);

  return 0;
}



// -----------------------------------------------------------------------------
//  FUNCIONES
// -----------------------------------------------------------------------------

void mostrar_catalogo(const std::vector<libro>& libros) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << "📚 CATÁLOGO COMPLETO DE LIBROS\n";
  std::cout << std::string(60, '=') << "\n";
  for_each(libros.begin(), libros.end(), [](const libro Libro){
    std::cout << "  [" << Libro.id << "] " << Libro.titulo << "\n";
    std::cout << "      Autor: " << Libro.autor << " (" << Libro.ano_publicacion << ")\n";
    std::cout << "      Estado: " << (Libro.disponible ? "✓ Disponible" : "✗ Prestado") << "\n\n";
  });
  std::cout << std::string(60, '=') << "\n";
}


void mostrar_catalogo_alreves(std::vector<libro>& libros) {
  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << "📚 CATÁLOGO INVERSO\n";
  std::cout << std::string(60, '=') << "\n";
  for_each(libros.rbegin(), libros.rend(), [](const libro Libro){
    std::cout << "  [" << Libro.id << "] " << Libro.titulo << "\n";
    std::cout << "      Autor: " << Libro.autor << " (" << Libro.ano_publicacion << ")\n";
    std::cout << "      Estado: " << (Libro.disponible ? "✓ Disponible" : "✗ Prestado") << "\n\n";
  });
  std::cout << std::string(60, '=') << "\n";
}


void mostrar_libros_disponibles(const std::vector<libro>& libros) {
  std::cout << "\n" << std::string(60, '-') << "\n";
  std::cout << "✓ LIBROS DISPONIBLES PARA PRÉSTAMO\n";
  std::cout << std::string(60, '-') << "\n";
  bool ninguno_disponible = true;
  
  for_each(libros.cbegin(), libros.cend(), [&ninguno_disponible](const libro Libro){
    if (Libro.disponible) {
      std::cout << "  [" << Libro.id << "] " << Libro.titulo << "\n";
      std::cout << "      " << Libro.autor << " (" << Libro.ano_publicacion << ")\n\n";
      ninguno_disponible = false;
    }
  });
  if (ninguno_disponible) {
    std::cout << "  ⚠ No hay libros disponibles en el catálogo.\n";
  }
  std::cout << std::string(60, '-') << "\n";
}


void contar_libros_por_decada(const std::vector<libro>& libros) {
  int contador = 0;
  std::vector<int> decada(10);
  
  std::cout << "\n" << std::string(60, '-') << "\n";
  std::cout << "📅 CONTADOR DE LIBROS POR DÉCADA\n";
  std::cout << std::string(60, '-') << "\n";
  std::cout << "Ingrese el primer año de la década: "; std::cin >> decada.at(0);

  //Bucle para ingresar los años de la década en el vector.
  for (size_t i = 1; i < decada.size(); i++) {
    decada.at(i) = decada.at(0)+i;
  }

  //Bucles para comparar años de publicación de los libros.
  for (const auto libro : libros) {
    for (const auto año : decada) {
      if (libro.ano_publicacion == año) {
        contador++;
        break;
      }
    }
  }

  std::cout << "\nResultado:\n";
  if (contador != 0) {
    std::cout << "  ✓ Se publicaron " << contador << " libro(s) en la década "
              << decada.at(0) << "-" << decada.at(9) << "\n";
  } else {
    std::cout << "  ✗ Ningún libro del catálogo se publicó en la década "
              << decada.at(0) << "-" << decada.at(9) << "\n";
  }
  std::cout << std::string(60, '-') << "\n";
}


void prestar_libro(std::vector<libro>& libros, std::list<std::string>& lista) {
  std::string nombre;
  int id = 0, indice = 0;
  bool existente = false;

  std::cout << "\n" << std::string(60, '-') << "\n";
  std::cout << "📖 PRÉSTAMO DE LIBRO\n";
  std::cout << std::string(60, '-') << "\n";
  std::cout << "ID del libro: "; std::cin >> id;
  std::cin.ignore();
  std::cout << "Nombre del lector: "; std::getline(std::cin, nombre);

  //Validar que el libro existe en el catálogo.
  for (const auto libro : libros) {
    if (id == libro.id) {
      existente = true; 
      break;
    }
    indice++;
  }

  if (!existente) {
    std::cout << "\n  ✗ ERROR: El libro no existe en el catálogo.\n";
    std::cout << std::string(60, '-') << "\n";
    return;
  }

  //Mostrar información del libro a prestar si existe.
  std::cout << "\nLibro seleccionado:\n";
  std::cout << "  [" << libros.at(indice).id << "] " << libros.at(indice).titulo << "\n";
  std::cout << "  Autor: " << libros.at(indice).autor << " (" << libros.at(indice).ano_publicacion << ")\n";


  //Revisar si el lector ya está en la lista de espera.
  existente = false;
  for (const auto lector : lista) {
    if (nombre == lector) {
      existente = true;
      break; // No se agrega a la lista de espera.
    }
  }

  //Prestar el libro.
  if (libros.at(indice).disponible) {
    //En caso tal el libro si esté disponible al momento de prestarlo.
    libros.at(indice).disponible = false;

    //Si el lector ya existía en la lista, se elimina.
    if (existente) {
      lista.remove_if([&nombre](std::string& Nombre){
        return nombre == Nombre;
      });
    }

  } else {
    //Si el libro no está disponible.

    //En caso de que el lector no exista en la lista, se añade al final.
    if (!existente) {
      std::cout << "\n  ⚠ Libro no disponible. " << nombre << " añadido a la lista de espera.\n";
      lista.emplace_back(nombre);
    }
  }
  std::cout << std::string(60, '-') << "\n";
}


void devolver_libro(std::vector<libro>& catalogo, std::list<std::string>& lectores) {
  int id, indice = 0;
  bool disponible = false;
  /* std::string nombre; */

  std::cout << "Ingrese el id del libro a devolver: "; std::cin >> id;
  
  //Validar si el libro existe en el catálogo.
  disponible = std::any_of(catalogo.cbegin(), catalogo.cend(), [&id, &indice](const auto& libro){
    if (libro.id != id) {indice++;}
    return libro.id == id;
  }); //Buscar el índice dentro del [std::any_of] es una mala práctica jaajajsa

  if (!disponible) {
    std::cout << "El libro a devolver no está registrado en el catálogo." << std::endl;
    return;
  }
  
  //Devolver libro. 
  disponible = false;
  
  if (catalogo.at(indice).disponible == false) {
    if (lectores.empty()) {
      catalogo.at(indice).disponible = true;
      std::cout << "Libro disponible para préstamo." << std::endl;
    } else {
      std::cout << "Libro devuelto y prestado a " << lectores.front() << std::endl;
      lectores.pop_front();  
    }
  
  } else {
    std::cout << "ERROR: El libro que intenta devolver ya está disponible." << std::endl;
    return;
  } 
}

void libros_por_autor(std::vector<libro> libros) {
  std::string nombre_autor; 
  std::cin.ignore();
  std::cout << "Ingrese el nombre del autor: "; std::getline(std::cin, nombre_autor);

  //Para hacer esta parte insensible a mayúsculas y minúculas tengo una idea.
  /* 
   Usar [std::tolower] tanto para el nombre ingresado como el que está en el catálogo.
   Pero con unos truquitos extra ya que [std::tolower] solo sirve para char.
  */

  //Transfomar el nombre ingresado a minúsculas.
  for (auto& caracter : nombre_autor) {
    caracter = std::tolower(static_cast<unsigned char>(caracter));
  }
  
  //Transformar a minúsculas cada nombre de autor del catálogo. Para eso se usa [std::transform].
  for (auto& libro : libros) {
    std::transform(libro.autor.begin(), libro.autor.end(), libro.autor.begin(), [](auto& caracter){
      return std::tolower(static_cast<unsigned char>(caracter));
    });
  }

  //Validar que el autor ingresado exista en el catálogo.
  bool existe = std::any_of(libros.cbegin(), libros.cend(), [&nombre_autor](const auto& Libro){
    return nombre_autor == Libro.autor;
  });

  if (existe) {
    // Mostrar libros por autor.
    std::cout << "Libros del autor " << nombre_autor << std::endl;

    std::for_each(libros.cbegin(), libros.cend(), [&nombre_autor](const auto &libro) {
      if (nombre_autor == libro.autor) {
        std::cout << "[" << libro.id << "] " << libro.titulo << " - " << libro.ano_publicacion << std::endl;
      }
    });

  } else {
    std::cout << "El autor ingresado no existe." << std::endl;
  }
}


void mostrar_siguiente_espera(const std::list<std::string>& lectores) {
  if (lectores.empty()) {
    std::cout << "No hay espera." << std::endl;
  
  } else {
    std::cout << "Siguiente en espera: " << lectores.front() << std::endl;
  }
}  


void ordenar_vector(std::vector<libro>& libros) {
  //Dado que el orden de los libros no es importante, se procede a ordenar con [std::sort].
  std::sort(libros.begin(), libros.end(), [](auto& actual, auto& siguiente){
    if (actual.titulo != siguiente.titulo) {
      return actual.titulo > siguiente.titulo;
    }
    return actual.autor > siguiente.autor;
  });
}


void eliminar_duplicados(std::vector<libro>& libros) {
  ordenar_vector(libros);

  // Se mueven al final los libros repetidos con [std::unique].
  // 'a_eliminar' es un iterador que apunta al inicio del rango de duplicados a eliminar.
  auto a_eliminar = std::unique(libros.begin(), libros.end(),[](auto& duplicado_1, auto& duplicado_2){
    return duplicado_1.titulo == duplicado_2.titulo && duplicado_1.autor == duplicado_2.autor;
  });

  libros.erase(a_eliminar, libros.end());
}


void analizar_estadisticas(const std::vector<libro>& libros, const std::list<std::string>& lista_espera) {
  //Punteros para el más reciente y el más antiguo. Para no hacer copias.
  const libro *antiguo = &libros.at(0), *reciente = &libros.at(0);
  float contador_disponibles = 0, no_disponibles = 0;


  // 1. Indicar el libro más antiguo y el más reciente. (Hecho)
  // 2. Indicar el porcentaje de libros disponibles. (Hecho)
  // 3. Indicar la cantidad de lectores en espera por libro prestado. (Hecho)

  //Preguntarle a Antigravity si hay una fórma más optimizada de hacer esto.
  for (size_t i = 0; i < libros.size(); i++) {
    //Se comparan las fechas.
    if (libros.at(i).ano_publicacion < antiguo->ano_publicacion) {
      antiguo = & libros.at(i);
    } else if (libros.at(i).ano_publicacion > reciente->ano_publicacion) {
      reciente = &libros.at(i);
    }
  }

  std::cout << "\n" << std::string(60, '=') << "\n";
  std::cout << "📊 ESTADÍSTICAS DE LA BIBLIOTECA\n";
  std::cout << std::string(60, '=') << "\n";
  
  std::cout << "📕 Libro más antiguo: " << antiguo->titulo << " (" << antiguo->ano_publicacion << ")\n";
  std::cout << "📗 Libro más reciente: " << reciente->titulo << " (" << reciente->ano_publicacion << ")\n\n";

  // Para saber qué porcentaje representa una cantidad con respecto a otra
  // Se usa (cantidad/total)*100

  //Se cuentan los disponibles.
  std::for_each(libros.cbegin(), libros.cend(), [&contador_disponibles, &no_disponibles](const libro& Libro){
    if (Libro.disponible) {contador_disponibles++;} else {no_disponibles++;}
  });
  
  std::cout << "📈 Disponibilidad: " << std::fixed << std::setprecision(0) << (contador_disponibles / libros.size()) * 100 << "% de los libros disponibles\n";
  std::cout << "👥 Lista de espera: " << std::defaultfloat << std::setprecision(1) << (lista_espera.size() / no_disponibles) << " lectores por libro prestado\n";
  std::cout << std::string(60, '=') << "\n";
}
