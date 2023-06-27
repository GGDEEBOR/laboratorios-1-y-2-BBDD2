#include <iostream>
#include <fstream>
#include <cstring>

const int PAGE_SIZE = 4096;
// clase del gestor de base de datos
class GestorBaseDatos {
private:
    std::string nombreArchivo; // archivo
    std::fstream archivo;
    int numTotalPags;
    int posPag;

public:
//constructor
    GestorBaseDatos(const std::string& nombreArchivo) : nombreArchivo(nombreArchivo), numTotalPags(0), posPag(0) {
        archivo.open(nombreArchivo, std::ios::in | std::ios::out | std::ios::binary);

        if (!archivo) {
            // El archivo no existe, así que lo creamos y escribimos el número de páginas
            archivo.open(nombreArchivo, std::ios::out | std::ios::binary);
            archivo.write(reinterpret_cast<const char*>(&numTotalPags), sizeof(numTotalPags));
            archivo.close();
        } else {
            // El archivo ya existe, leemos el número de páginas almacenado al inicio
            archivo.read(reinterpret_cast<char*>(&numTotalPags), sizeof(numTotalPags));
        }
    }
    // destructor
    ~GestorBaseDatos() {
        archivo.close();
    }
    // funcion que lee el bloque
    void leerBloque(int numPag, char* buffer) {
        if (numPag >= numTotalPags) {
            std::cout << "Error: El archivo no tiene la pagina " << numPag << std::endl;
            return;
        }

        archivo.seekg((numPag + 1) * PAGE_SIZE, std::ios::beg);
        archivo.read(buffer, PAGE_SIZE);
        posPag = numPag;
    }

    // obtiene la posición del bloque
    int obtenerPosBloque() const {
        return posPag;
    }
    //lee el bloque
    void leerPrimerBloque(char* buffer) {
        leerBloque(0, buffer);
    }
    //lee ultimo bloque
    void leerUltimoBloque(char* buffer) {
        leerBloque(numTotalPags - 1, buffer);
    }
    // lee un bloque anterior 
    void leerBloqueAnterior(char* buffer) {
        if (posPag == 0) {
            std::cout << "Error: No se puede leer un bloque anterior al primero" << std::endl;
            return;
        }

        leerBloque(posPag - 1, buffer);
    }
    // lo que hace es leer el siguiente bloque
    void leerBloqueSiguiente(char* buffer) {
        if (posPag == numTotalPags - 1) {
            std::cout << "Error: No se puede leer un bloque posterior al ultimo" << std::endl;
            return;
        }

        leerBloque(posPag + 1, buffer);
    }
    // lee bloque actual
    void leerBloqueActual(char* buffer) {
        leerBloque(posPag, buffer);
    }
    // escribe en bloque
    void escribirBloque(int numPag, const char* datos) {
        if (numPag >= numTotalPags) {
            std::cout << "Error: El archivo no tiene la pagina " << numPag << std::endl;
            return;
        }

        archivo.seekp((numPag + 1) * PAGE_SIZE, std::ios::beg);
        archivo.write(datos, PAGE_SIZE);
        posPag = numPag;
    }
    // escribe en el bloque actual
    void escribirBloqueActual(const char* datos) {
        escribirBloque(posPag, datos);
    }

    void agregarBloqueVacio() {
        archivo.seekp(0, std::ios::end);
        char bloqueVacio[PAGE_SIZE];
        std::memset(bloqueVacio, 0, PAGE_SIZE);
        archivo.write(bloqueVacio, PAGE_SIZE);
        ++numTotalPags;
    }
};

int main() {
    std::string nombreArchivo = "basedatos.dat";
    char buffer[PAGE_SIZE];

    GestorBaseDatos gestor(nombreArchivo);

    // Leer el primer bloque
    gestor.leerPrimerBloque(buffer);
    std::cout << "Contenido del primer bloque: " << buffer << std::endl;

    // Leer el último bloque
    gestor.leerUltimoBloque(buffer);
    std::cout << "Contenido del ultimo bloque: " << buffer << std::endl;

    // Agregar un bloque vacío
    gestor.agregarBloqueVacio();
    std::cout << "Numero de paginas despues de agregar un bloque vacio: " << gestor.obtenerPosBloque() << std::endl;

    // Escribir en el bloque actual
    const char* datos = "Hola, este es un nuevo bloque";
    gestor.escribirBloqueActual(datos);

    // Leer el bloque actual
    gestor.leerBloqueActual(buffer);
    std::cout << "Contenido del bloque actual: " << buffer << std::endl;

    return 0;
}
