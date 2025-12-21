#include <iostream>
#include <array>
#include <algorithm>

//Estructura para los datos de los empleados.
struct Empleado {
    std::string nombre;
    int edad;
    float salario;
};

int main() {
    std::string busqueda_nombre = " ";

    std::array<Empleado, 5> empleados = {{
        {"Juan", 30, 50000},
        {"Maria", 25, 40000},
        {"Pedro", 35, 60000},
        {"Ana", 28, 55000},
        {"Luis", 32, 52000}
    }};

    std::cout << "-----------------Análisis de Datos de Empleados-----------------\n\n";
    std::cout << "Los empleados son:\n";
    for (const auto& empleado : empleados) {
        std::cout << "Nombre: " << empleado.nombre << "\nEdad: " 
                  << empleado.edad << "\nSalario: " << empleado.salario << "\n\n";

    }
    
    //algoritmo para encontrar a un empleado específico.
    std::cout << "Si desea buscar a un empleado específico, por favor digite su nombre: "; 
    std::getline(std::cin, busqueda_nombre);
    

    auto encontrado = std::find_if(empleados.begin(), empleados.end(), [&busqueda_nombre](const Empleado& empleado) {
        /* De la función lambda la [captura] corresponde a la variable del contexto externo
           y los (parámetros) corresponden a los elementos del vector dados por las funciones begin() y end().
        */
       return empleado.nombre == busqueda_nombre; //Si son iguales devuelve 'true' si no encuentra nada en todo el vector, devuelve 'last' que apunta a end().
    });

    if (encontrado != empleados.end()) {
        std::cout << "Encontrado: " << encontrado->nombre << std::endl; //Desreferencia a 'encontrado' que accede a la estructura 'Empleados'.
    } else {
        std::cout << "No se encontró al empleado.\n";
    }

    //Algoritmo para contar los empleados con un salario mayor a 50000.
    auto cantidad_salarios_mayores = std::count_if(empleados.begin(), empleados.end(), [](const Empleado& empleado) {
        /* La función count_if() devuelve cuenta cada elemento del vector que cumple una condición.
           la Lambda reciber por parámetros cada elemento del vector y evalúa según la condición dada.
           Devuelve 'true' si cumple y 'false' si no.
        */
        return empleado.salario > 50000;
    });
    std::cout << "Cantidad de empleados con salarios mayores a 50000: " << cantidad_salarios_mayores << std::endl;

    //Algoritmo para ordenar empleados de forma ascendente con sort().
    std::sort(empleados.begin(), empleados.end(), [](const Empleado& a, const Empleado& b) {
        return a.edad < b.edad;
    });

    std::cout << "\nEmpleados ordenados por edad de forma ascendente:\n";
    for (const auto& empleado : empleados) {
        std::cout << "Nombre: " << empleado.nombre << "\nEdad: " 
                  << empleado.edad << "\nSalario: " << empleado.salario << "\n\n";
    }

    //Algoritmo para aumentar el salario en 5% con for_each().
    std::for_each(empleados.begin(), empleados.end(), [](Empleado& empleado) {
        //for_each() funciona practicamente igual que un bucle 'for' 
        if (empleado.salario < 45000) {
            empleado.salario *= 1.05;

        }
    });
    std::cout << "\nEmpleados con salarios aumentados:\n";
    for (const auto& empleado : empleados) {
        std::cout << "Nombre: " << empleado.nombre << "\nEdad: " 
                  << empleado.edad << "\nSalario: " << empleado.salario << "\n\n";
    }

    return 0;
}
//Siguiente ejercicio: https://g.co/gemini/share/f618edd6ec8f
