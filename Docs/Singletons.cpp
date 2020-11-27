#include <iostream>

class Singleton
{
public:

    // Mètode per poder accedir al Singleton de manera estatica.
    static Singleton& GetInstance()
    {
        // Retorna la instancia declarada a la linia 22 i 26.
        return instance;
    }

    void Function()

private:

    // El constructor es crida al private perque no puegui ser instanciat/cridat desde fora.
    Singleton(){} 
    // Crea una instancia estatica.
    static Singleton instance;
}

// Al ser instance una variable estatica, necesita ser definida en algun lloc del cpp.
Singleton Singleton::instance;


int main()
{
    // Com cridar qualsevol funcio del singleton desde fora.
    Singleton::GetInstance().Function();

    // Tambe es pot optimitzar visualment, Cal declarar-ho com a referencia
    Singleton& instance = Singleton::GetInstance();


    instance.Function();
}