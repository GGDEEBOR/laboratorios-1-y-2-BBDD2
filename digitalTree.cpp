#include <iostream>
#include <map>
#include <string>
using namespace std;

template <typename T>
class ArbolDigital {
private:
    template <typename U>
    class Nodo {
    public:
        map<char, Nodo<U>*> hijos;
        U valor;

    public:
        Nodo() {}
        Nodo(U val) : valor(val) {}
    };

    Nodo<T>* raiz;

public:
    ArbolDigital() : raiz(nullptr) {}

    // Función para insertar una clave y valor en el árbol
    void Insertar(const T& clave, const T& valor) {
        if (raiz == nullptr) {
            raiz = new Nodo<T>(); // Crear el nodo raíz si aún no existe
        }

        InsertarRecursivo(raiz, clave, valor);
    }

    // Función para obtener el valor asociado a una clave
    T ObtenerValor(const T& clave) {
        Nodo<T>* nodo = ObtenerNodo(raiz, clave);

        if (nodo != nullptr) {
            return nodo->valor;
        }

        return T(); // Si la clave no existe, retornar un valor por defecto
    }

    // Función para eliminar un nodo del árbol
    void EliminarNodo(const T& clave) {
        EliminarNodoRecursivo(raiz, clave);
    }

public:
    // Función auxiliar para insertar recursivamente una clave y valor en el árbol
    void InsertarRecursivo(Nodo<T>* nodo, const T& clave, const T& valor) {
        if (clave.empty()) {
            nodo->valor = valor; // Si la clave está vacía, asignar el valor al nodo actual
            return;
        }

        char primerCaracter = clave[0]; // Obtener el primer carácter de la clave
        T nuevaClave = clave.substr(1); // Obtener el resto de la clave (sin el primer carácter)

        if (nodo->hijos.find(primerCaracter) == nodo->hijos.end()) {
            // Si el primer carácter no tiene un hijo asociado, crear un nuevo nodo hijo
            Nodo<T>* nuevoNodo = new Nodo<T>();
            nodo->hijos[primerCaracter] = nuevoNodo;
        }

        Nodo<T>* siguienteNodo = nodo->hijos[primerCaracter];
        InsertarRecursivo(siguienteNodo, nuevaClave, valor); // Insertar recursivamente con la nueva clave
    }

    // Función auxiliar para obtener recursivamente el nodo asociado a una clave
    Nodo<T>* ObtenerNodo(Nodo<T>* nodo, const T& clave) {
        if (clave.empty()) {
            return nodo; // Si la clave está vacía, retornar el nodo actual
        }

        char primerCaracter = clave[0]; // Obtener el primer carácter de la clave
        T nuevaClave = clave.substr(1); // Obtener el resto de la clave (sin el primer carácter)

        if (nodo->hijos.find(primerCaracter) != nodo->hijos.end()) {
            // Si el primer carácter tiene un hijo asociado, continuar la búsqueda recursiva
            Nodo<T>* siguienteNodo = nodo->hijos[primerCaracter];
            return ObtenerNodo(siguienteNodo, nuevaClave); // Obtener recursivamente el nodo con la nueva clave
        }
		   return nullptr; // Si no se encuentra el nodo, retornar nullptr
    }

    // Función auxiliar para eliminar recursivamente un nodo del árbol
    void EliminarNodoRecursivo(Nodo<T>* nodo, const T& clave) {
        if (clave.empty()) {
            nodo->valor = T(); // Si la clave está vacía, eliminar el valor asignándole un valor por defecto
            return;
        }

        char primerCaracter = clave[0]; // Obtener el primer carácter de la clave
        T nuevaClave = clave.substr(1); // Obtener el resto de la clave (sin el primer carácter)

        if (nodo->hijos.find(primerCaracter) != nodo->hijos.end()) {
            // Si el primer carácter tiene un hijo asociado, continuar la eliminación recursiva
            Nodo<T>* siguienteNodo = nodo->hijos[primerCaracter];
            EliminarNodoRecursivo(siguienteNodo, nuevaClave);

            if (siguienteNodo->valor == T() && siguienteNodo->hijos.empty()) {
                // Si el nodo hijo no tiene valor asignado y no tiene hijos, eliminarlo
                nodo->hijos.erase(primerCaracter);
                delete siguienteNodo;
            }
        }
    }
};

int main() {
    ArbolDigital<string> arbol;
    arbol.Insertar("ninho", "23");
    arbol.Insertar("ninhera", "28");
    arbol.Insertar("ninhita", "12");

    string valor1 = arbol.ObtenerValor("ninhita");
    string valor2 = arbol.ObtenerValor("ninho");
    string valor3 = arbol.ObtenerValor("ninhera");

    cout << "Valor de ninhita es: " << valor1 << endl;  // Output: "28"
    cout << "Valor de ninho es: " << valor2 << endl;  // Output: "23"
    cout << "Valor de ninhera es: " << valor3 << endl;  // Output: "28"
											 
    arbol.EliminarNodo("ninhita");
    cout << "Eliminando ninhita" << endl;
    string valor4 = arbol.ObtenerValor("ninhita");
    cout << "Valor de ninhita es: " << valor4 << endl;  // Output: ""

    return 0;
}
