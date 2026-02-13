/* --------------------------------------------- */
//  Sistema Bancario con Clases y Smart Pointers //
/* --------------------------------------------- */

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

//Prototipos de clase y puntero.
class Cliente;
class Cuenta_bancaria;

using shared_cuenta = std::shared_ptr<Cuenta_bancaria>;
using weak_cliente = std::weak_ptr<Cliente>;
using shared_cliente = std::shared_ptr<Cliente>;


//
//  CLASES
//_____________________________________________________________________________________________________

class Cliente {
    private:
        std::string nombre;
        int dni;
        std::vector<shared_cuenta> cuentas; //Un cliente puede tener varias cuentas.

    public:
        //Constructor
        Cliente(std::string C_nombre, int C_dni) : nombre(C_nombre), dni(C_dni) {};
        ~Cliente(){};

        //GETTERS
        std::string getNombre() const {
            return nombre;
        }
        int getDni() const {
            return dni;
        }

        std::vector<shared_cuenta> getVector_cuentas() {
            return cuentas;
        }

        //MÉTODOS
        void crear_cuenta(shared_cliente& cli_cliente) {
            //Crear cuenta
            std::string num_cuenta;
            int sal_cuenta;

            std::cout << "\n✨ [NUEVA CUENTA]" << std::endl;
            std::cout << "➤ Ingrese el número de cuenta: "; std::cin >> num_cuenta;
            std::cout << "➤ Ingrese el saldo inicial: $"; std::cin >> sal_cuenta;

            auto nueva_cuenta = std::make_shared<Cuenta_bancaria>(num_cuenta, sal_cuenta, cli_cliente);

            //Añadirla al vector.
            cuentas.emplace_back(nueva_cuenta);
        }
        

        void eliminar_cuenta() {
            //Validar que la cuenta a eliminar existe.
            std::string cuenta_eliminar;
            std::cout << "Ingrese el número de cuenta que desea eliminar: "; std::cin >> cuenta_eliminar;

            //*Encontrar una manera de optimizar la validación y eliminación con menos código.
            
            auto existe = std::find_if(cuentas.begin(), cuentas.end(), [cuenta_eliminar](auto& cuenta){
                return cuenta->getNumeroCuenta() == cuenta_eliminar;
            });

            if (existe == cuentas.end()) {
                std::cout << "La cuenta ingresada no existe." << std::endl;
                return;
            } else {
                std::cout << "Cuenta encontrada." << std::endl;
            }

            cuentas.erase(std::remove_if(cuentas.begin(), cuentas.end(), [cuenta_eliminar](const auto& cuenta){
                return cuenta->getNumeroCuenta() == cuenta_eliminar;
            }), cuentas.end());
        }


        void mostrar_datos() {
            std::cout << "\n========================================" << std::endl;
            std::cout << "👤 DATOS DEL CLIENTE" << std::endl;
            std::cout << "========================================" << std::endl;
            std::cout << "Nombre: " << nombre << std::endl;
            std::cout << "DNI:    " << dni << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "📂 CUENTAS ASOCIADAS:" << std::endl;
            if (cuentas.empty()) {
                std::cout << "  (No hay cuentas registradas)" << std::endl;
            } else {
                std::for_each(cuentas.cbegin(), cuentas.cend(), [](const auto& cuenta){
                    std::cout  << "  🔹 Nº: " << cuenta->getNumeroCuenta() << std::endl;
                });
            }
            std::cout << "========================================\n" << std::endl;
        }
};


class Cuenta_bancaria {
    private:
        std::string numero_cuenta;
        int saldo;
        weak_cliente cliente; //Cada cuenta tiene un único cliente, el dueño es el cliente. //*para evitar ciclos de referencia.

    public:
        //Constructor
        Cuenta_bancaria(std::string Cu_numeroCuenta, int Cu_saldo, shared_cliente Cu_cliente) : numero_cuenta(Cu_numeroCuenta), saldo(Cu_saldo), cliente(Cu_cliente) {};
        
        //Destructor.
        ~Cuenta_bancaria() {
            std::cout << "Cuenta eliminada con exito." << std::endl;
        }

        //GETTERS
        std::string getNumeroCuenta() const {
            return numero_cuenta;
        }

        int getSaldo() const {
            return saldo;
        }
        
        shared_cliente getCliente() {
            return cliente.lock(); //*Devuelve un shared_ptr porque [.lock] devuelve un shared.
        }

        //MÉTODOS
        bool depositar(const float& deposito) {
            //Validar si el dato es correcto.
            if (deposito < 0) {
                //Debe ser no negativo.
                std::cout << "❌ Error: El depósito debe ser un monto positivo." << std::endl;
                return false;
            }

            saldo += static_cast<int>(deposito);
            return true;
        }


        bool retirar(const float& monto) {
            //Validar que el monto a retirar no exceda el saldo actual.
            if (monto > saldo) {
                std::cout << "❌ Error: Fondos insuficientes. El monto excede el saldo actual." << std::endl;
                return false;
            } else if (monto < 0) {
                std::cout << "❌ Error: El monto a retirar debe ser un valor positivo." << std::endl;
                return false;
            }

            saldo -= static_cast<int>(monto);
            return true;
        }


        bool transferir(shared_cuenta& cuenta_destino, const float& monto_transferir) {
            //Validar que el monto a transferir sea correcto.
            cuenta_destino->depositar(monto_transferir);
            return true;
        }

        void mostrar_datos() {
            std::cout << "\n----------------------------------------" << std::endl;
            std::cout << "🏦 DETALLES DE LA CUENTA" << std::endl;
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "📌 Número: " << numero_cuenta << std::endl;
            std::cout << "💰 Saldo:  $" << saldo << std::endl;
            if (auto cli = cliente.lock()) {
                std::cout << "👤 Titular: " << cli->getNombre() << std::endl;
            }
            std::cout << "----------------------------------------\n" << std::endl;
        }
};


//
//  PROTOTIPOS DE FUNCIÓN
//_____________________________________________________________________________________________________
void validar_montos(float& monto);
void validar_numero_cuenta(std::string& numeroCuenta, const std::vector<shared_cuenta>& Cuentas, int& indice);


//
//  FUNCIÓN PRINCIPAL
//_____________________________________________________________________________________________________

int main() {
    //Crear un cliente.
    std::cout << "🚀 Iniciando Sistema Bancario..." << std::endl;
    auto cliente_1 = std::make_shared<Cliente>("Juan", 123);
    
    cliente_1->crear_cuenta(cliente_1);
    cliente_1->crear_cuenta(cliente_1);

    //Mostrar info de cuentas (lado del usuario)
    std::string numero_transferir;
    auto Cuentas = cliente_1->getVector_cuentas(); //Cuentas del cliente 1.

    std::cout << "\n" << "🔍 BUSCAR CUENTA" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "➤ Ingrese el número de cuenta a consultar: "; std::cin >> numero_transferir;
    int indice = 0;

    //Validar que la cuenta exista en el vector.
    std::cout << "⌛ Procesando búsqueda..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    validar_numero_cuenta(numero_transferir, Cuentas, indice);

    if (indice >= 0) {
        Cuentas.at(indice)->mostrar_datos();
    }


    //DEPOSITAR
    std::cout << "\n" << "📥 OPERACIÓN: DEPÓSITO" << std::endl;
    std::cout << "========================================" << std::endl;
    float Deposito;
    std::cout << "➤ Depositando dinero en cuenta: " << Cuentas.at(1)->getNumeroCuenta() << std::endl;
    std::cout << "➤ Ingrese el monto a depositar: $"; std::cin >> Deposito;
    
    std::cout << "⌛ Procesando depósito..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    if (Cuentas.at(1)->depositar(Deposito)) {
        std::cout << "✅ Depósito procesado." << std::endl;
    }
    Cuentas.at(1)->mostrar_datos();


    //RETIRAR
    std::cout << "\n" << "📤 OPERACIÓN: RETIRO" << std::endl;
    std::cout << "========================================" << std::endl;
    float monto_retiro;
    std::cout << "➤ Retirando dinero de cuenta: " << Cuentas.at(1)->getNumeroCuenta() << std::endl;
    std::cout << "➤ Ingrese el monto a retirar: $"; std::cin >> monto_retiro;
    
    std::cout << "⌛ Procesando retiro..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    if (Cuentas.at(1)->retirar(monto_retiro)) {
        std::cout << "✅ Retiro procesado." << std::endl;
    }
    Cuentas.at(1)->mostrar_datos();
   

    //TRANSFERIR
    std::cout << "\n" << "💸 OPERACIÓN: TRANSFERENCIA" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "🔹 Origen:  " << Cuentas.at(0)->getNumeroCuenta() << std::endl;
    std::cout << "🔹 Destino: " << Cuentas.at(1)->getNumeroCuenta() << std::endl;
    std::cout << "➤ Ingrese el monto a transferir: $"; std::cin >> Deposito;
    
    std::cout << "\n⌛ Procesando envío..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    if (Cuentas.at(0)->retirar(Deposito)) {
        Cuentas.at(1)->transferir(Cuentas.at(1), Deposito);
        std::cout << "✅ Transferencia realizada con éxito." << std::endl;
    } else {
        std::cout << "❌ Transferencia cancelada por falta de fondos." << std::endl;
    }

    std::cout << "\n📋 RESUMEN FINAL DE CUENTAS:" << std::endl;
    Cuentas.at(0)->mostrar_datos();
    Cuentas.at(1)->mostrar_datos();

    std::cout << "Cerrando sesión del cliente..." << std::endl;
    return 0;
}


//
//  FUNCIONES
//_____________________________________________________________________________________________________
void validar_montos(float &monto) {
  while (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(1000, '\n');
    std::cout << "⚠️  Valor ingresado inválido. Por favor, ingrese nuevamente: ";
    std::cin >> monto;
  }
}


void validar_numero_cuenta(std::string& numeroCuenta, const std::vector<shared_cuenta>& Cuentas, int& indice) {
    auto existe = std::find_if(Cuentas.begin(), Cuentas.end(), [numeroCuenta](const auto& Cuenta){
        return Cuenta->getNumeroCuenta() == numeroCuenta;
    });

    //obtiene el índice si la encuentra.
    if (existe != Cuentas.end()) {
        indice = std::distance(Cuentas.begin(), existe);
        std::cout << "✅ Cuenta encontrada correctamente." << std::endl;
    } else {
        std::cout << "❌ Error: El número de cuenta ingresado es incorrecto o no existe." << std::endl;
        indice = -1; // Marcamos como no encontrado
    }
}