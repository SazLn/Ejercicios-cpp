/*-------------------------*/
/* Telemetría de Sensores  */
/*-------------------------*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> //Para std::acumulate()
#include <thread> //Para std::this_thread::sleep_for()
#include <chrono> //Para std::chrono::seconds()

/* enum para el tipo de sensor */
enum tipo_sensor {TEMPERATURA, PRESION, RADIACION};
enum estado_lectura {OPTIMO, ERROR, CRITICO};

//Estructura para la lectura del sensor.
struct lectura {
  int id;
  float valor;
  tipo_sensor tipo;
  estado_lectura estado;
};

int main() {
  float radiacion_limite = 0.3;

  //Agregar un retraso de 1 o 2 segundos luego de un mensaje que diga "Procesando lecturas...".
  //vector con lecturas.
  std::vector<lectura> lecturas = {
    {.id = 1, .valor = 23.5, .tipo = TEMPERATURA, .estado = OPTIMO},
    {.id = 2, .valor = 101.2, .tipo = PRESION, .estado = ERROR},
    {.id = 3, .valor = 0.5, .tipo = RADIACION, .estado = CRITICO},
    {.id = 4, .valor = 25.5, .tipo = TEMPERATURA, .estado = OPTIMO},
    {.id = 5, .valor = 101.2, .tipo = PRESION, .estado = CRITICO},
    {.id = 6, .valor = 0.5, .tipo = RADIACION, .estado = OPTIMO},
    {.id = 7, .valor = 23.5, .tipo = TEMPERATURA, .estado = OPTIMO},
    {.id = 8, .valor = 101.2, .tipo = PRESION, .estado = OPTIMO},
    {.id = 9, .valor = 0.5, .tipo = RADIACION, .estado = CRITICO},
    {.id = 10, .valor = 23.5, .tipo = TEMPERATURA, .estado = ERROR}
  };

  std::cout << "Cantidad de lecturas: " << lecturas.size() << std::endl;
  std::cout << "Imprimiendo lecturas..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (const auto& elemento : lecturas) {
    std::cout << "ID: " << elemento.id << "\tValor: " << elemento.valor << "\tTipo: " << elemento.tipo << "\t  Estado: " << elemento.estado << std::endl;
  }

  std::cout << "\nEliminando lecturas en estado de ERROR..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //Eliminar las lecturas en estado ERROR.
  //Se recorre el vector de inicio a fin y con una lambda se evalúa si el estado de la lectura es ERROR.
  //La lambda retorna true si el estado es ERROR y false en caso contrario.
  auto lecuras_error = std::remove_if(lecturas.begin(), lecturas.end(), [](lectura& elemento) {
    return elemento.estado == ERROR;
  });
  lecturas.erase(lecuras_error, lecturas.end()); //.erase() borra los elementos del vector.

  std::cout << "Imprimiendo lecturas..." << std::endl; 
  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (const auto& elemento : lecturas) {
    std::cout << "ID: " << elemento.id << "\tValor: " << elemento.valor << "\tTipo: " << elemento.tipo << "\t  Estado: " << elemento.estado << std::endl;
  }

  //Reordenar lecturas por ESTADO.
  /* 
    La función std::partition() recorre el vector de inicio a fin y reordena los elementos 
    y según la lambda retorna true si el estado es CRITICO y false en caso contrario.
    Esto reordena el vector de manera que los elementos que cumplan la condición (CRITICO) estén al inicio.
  */
  auto estado_lectura = std::partition(lecturas.begin(), lecturas.end(), [](lectura& elemento) {
    return elemento.estado == CRITICO;
  });

  std::cout << "\nReordenando lecturas por estado..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  for (const auto& elemento : lecturas) {
    std::cout << "ID: " << elemento.id << "\tValor: " << elemento.valor << "\tTipo: " << elemento.tipo << "\t  Estado: " << elemento.estado << std::endl;
  }


  std::cout << "\nPromediando valores de las lecturas..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //Promediar los valores de las lecturas. Se usa std::accumulate() para hacer la suma de los valores.
  //std::accumulate() recorre el vector de inicio a fin y suma los valores, esa suma se acumula con cada recorrido. La función devuelve el valor acumulado.
  //El 0.0 es el valor inicial de la acumulación.
  //La lambda suma los valores de las lecturas. Para que funcione bien, debe recibir primero el valor acumulado y luego el elemento actual en orden de izquierda a derecha.

  float promedio = std::accumulate(lecturas.begin(), lecturas.end(), 0.0, [](float acumulado, const lectura& elemento){
    return acumulado + elemento.valor;
  });
  std::cout << "Promedio de las lecturas: " << promedio/lecturas.size() << std::endl;
  

  std::cout << "\nRevisando valores de los sensores de radiación..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  //Revisar los valores de los sensores de radiación valor > 0.3 y avisar por consola.
    for (const auto& elemento : lecturas) {
    if (elemento.tipo == RADIACION && elemento.valor > radiacion_limite) { // Check simple
      std::cout << "Radiacion superada" << std::endl 
                << "Sensor: " << elemento.id << std::endl 
                << "Valor: " << elemento.valor << std::endl;
    }
  }

  
  return 0;
}

//Ejercicio: https://gemini.google.com/share/2689d294c1f2
