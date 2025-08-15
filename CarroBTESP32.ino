/*
Control para Carro Bluetooth con ESP32 y Driver L298.

Desarrollado por: Carlos Andrés Rodríguez Pérez && Cristian Camilo Cañaveral Avilés
Fecha de creación: 11 de agosto de 2025
Contacto: carlos.rodriguez@itspereira.edu.co && camilo.canaveral@itspereira.edu.co
*/

// ****************************************** Librerías *********************************************************************************
// Se incluye la librería para usar la comunicación Bluetooth Serial.
// Esta librería es específica de la ESP32 y permite que se comporte como un dispositivo Bluetooth clásico.
#include <BluetoothSerial.h>

// Se crea un objeto de la clase BluetoothSerial. 
// Es como crear una "instancia" de la comunicación Bluetooth que usaremos en el código.
BluetoothSerial SerialBT;

// --------------------------------- Definición de Pines GPIO para el Control de Motores -----------------------------------------------
// Se usan constantes para definir los pines de la ESP32. Esto hace el código más legible y fácil de modificar.

// Los pines 12 y 14 controlan la dirección del Motor 1.
const int motor1Pin1 = 12; 
const int motor1Pin2 = 14; 
// Los pines 27 y 26 controlan la dirección del Motor 2.
const int motor2Pin1 = 27; 
const int motor2Pin2 = 26; 
// Los pines 13 y 25 controlan la velocidad de los motores usando PWM (Pulse-Width Modulation).
// Estos pines deben ser compatibles con PWM en la ESP32.
const int motor1Vel = 13;
const int motor2Vel = 25;

// --------------------------------------- Función de Configuración (setup) -----------------------------------------------------------
// Se ejecuta una sola vez al inicio del programa.
void setup() {
  // Inicializa la comunicación serial a través del puerto USB para depuración.
  // La velocidad de 115200 baudios es estándar para la ESP32.
  Serial.begin(115200);

  // Inicializa la comunicación Bluetooth con un nombre específico.
  // Este es el nombre que verás en la lista de dispositivos Bluetooth de tu teléfono.
  // Es muy útil para diferenciar tu robot de otros dispositivos.
  SerialBT.begin("Robot"); 
  
  // Imprime un mensaje en la consola serial para saber que el Bluetooth está listo.
  Serial.println("Bluetooth iniciado. ¡Listo para emparejar!");

  // Configura los pines de control de los motores como salidas.
  // Esto es crucial para que la ESP32 pueda enviar señales a los drivers de los motores.
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(motor1Vel, OUTPUT);
  pinMode(motor2Vel, OUTPUT);
}

// --------------------------------------------- Bucle Principal (loop) ---------------------------------------------------------------
// Se ejecuta repetidamente mientras la ESP32 está encendida.
void loop() {
  // Comprueba si hay datos disponibles en el buffer de entrada de Bluetooth.
  // Esto evita leer datos cuando no hay nada y causa errores.
  if (SerialBT.available() > 0) {
    // Si hay datos, lee el primer carácter recibido y lo almacena en la variable 'dato'.
    char dato = SerialBT.read();
    
    // Imprime el dato recibido en la consola serial para depuración.
    // Esto es muy útil para verificar que la app está enviando los comandos correctos.
    Serial.println(dato); 

    // Estructura de control 'switch' que evalúa el valor del 'dato' recibido.
    // Dependiendo del carácter, llama a una función específica para mover el robot.
    switch (dato) {
      case 'B': // Adelante (Comando de la app)
        adelante();
        break;
      case 'E': // Izquierda
        izquierda();
        break;
      case 'D': // Derecha
        derecha();
        break;
      case 'C': // Atrás
        atras();
        break;
      case 'A': // Detener
        detener();
        break;
    }
  }
  // Introduce un pequeño retraso para evitar que el bucle se ejecute demasiado rápido.
  // Esto es útil para estabilizar la comunicación y el control.
  delay(20); 
}

// ------------------------------------------ Funciones para el Control de Movimiento ------------------------------------------------
// Estas funciones encapsulan la lógica para cada movimiento del robot.

// Mueve el robot hacia adelante.
void adelante() {
  // Se usa analogWrite para controlar la velocidad de los motores usando PWM.
  // Un valor de 255 es la velocidad máxima.
  analogWrite(motor1Vel, 255);
  analogWrite(motor2Vel, 255);

  // Se configura la dirección de los motores.
  // HIGH y LOW en los pines de dirección determinan si el motor gira en un sentido o en el otro.
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Mueve el robot hacia atrás.
void atras() {
  analogWrite(motor1Vel, 255);
  analogWrite(motor2Vel, 255);
  
  // Se invierte la dirección de los motores con respecto a la función 'adelante()'.
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Gira el robot hacia la izquierda.
// Un motor va hacia adelante y el otro hacia atrás.
void izquierda() {
  analogWrite(motor1Vel, 255);
  analogWrite(motor2Vel, 255);
  
  // El motor 1 va hacia atrás (LOW, HIGH) y el motor 2 va hacia adelante (HIGH, LOW).
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

// Gira el robot hacia la derecha.
// Se invierte la lógica del giro a la izquierda.
void derecha() {
  analogWrite(motor1Vel, 255);
  analogWrite(motor2Vel, 255);
  
  // El motor 1 va hacia adelante (HIGH, LOW) y el motor 2 va hacia atrás (LOW, HIGH).
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Detiene completamente el movimiento del robot.
void detener() {
  // Se establece la velocidad de los motores a 0.
  analogWrite(motor1Vel, 0);
  analogWrite(motor2Vel, 0);
  
  // Se apagan ambos pines de dirección de cada motor para asegurarse de que estén detenidos.
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}