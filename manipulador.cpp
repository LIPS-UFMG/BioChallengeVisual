// Definindo os pinos de controle dos 3 motores de passo
#define MOTOR1_IN1_PIN  2
#define MOTOR1_IN2_PIN  3
#define MOTOR1_IN3_PIN  4
#define MOTOR1_IN4_PIN  5

#define MOTOR2_IN1_PIN  6
#define MOTOR2_IN2_PIN  7
#define MOTOR2_IN3_PIN  8
#define MOTOR2_IN4_PIN  9

#define MOTOR3_IN1_PIN  10
#define MOTOR3_IN2_PIN  11
#define MOTOR3_IN3_PIN  12
#define MOTOR3_IN4_PIN  13

// Sequência de passos do motor (passo completo)
int steps[4][4] = {
  {1, 1, 0, 0}, // Passo 1
  {0, 1, 1, 0}, // Passo 2
  {0, 0, 1, 1}, // Passo 3
  {1, 0, 0, 1}  // Passo 4
};

int motor1Speed = 50; // Velocidade do motor 1 em passos por segundo
int motor2Speed = 50; // Velocidade do motor 2 em passos por segundo
int motor3Speed = 50; // Velocidade do motor 3 em passos por segundo

void setup() {
  // Configura os pinos como saída para cada motor
  setupMotor(MOTOR1_IN1_PIN, MOTOR1_IN2_PIN, MOTOR1_IN3_PIN, MOTOR1_IN4_PIN);
  setupMotor(MOTOR2_IN1_PIN, MOTOR2_IN2_PIN, MOTOR2_IN3_PIN, MOTOR2_IN4_PIN);
  setupMotor(MOTOR3_IN1_PIN, MOTOR3_IN2_PIN, MOTOR3_IN3_PIN, MOTOR3_IN4_PIN);

  // Define as velocidades dos motores
  setMotorSpeed(1, motor1Speed);
  setMotorSpeed(2, motor2Speed);
  setMotorSpeed(3, motor3Speed);
}

void loop() {
  // Exemplo de controle dos 3 motores
  rotateRight(1, 90);  // Gira motor 1 para a direita 90 graus
  rotateLeft(2, 90);   // Gira motor 2 para a esquerda 90 graus
  rotateRight(3, 90); // Gira motor 3 para a direita 180 graus
  delay(1000);
  
  // Pausa
  rotateLeft(1, 90);   // Gira motor 1 para a esquerda 90 graus
  rotateRight(2, 90);  // Gira motor 2 para a direita 90 graus
  rotateLeft(3, 90);  // Gira motor 3 para a esquerda 180 graus
  delay(1000);
}

// Função para configurar os pinos de controle de cada motor
void setupMotor(int in1, int in2, int in3, int in4) {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

// Função para definir a velocidade do motor (em passos por segundo)
void setMotorSpeed(int motor, int stepsPerSecond) {
  switch (motor) {
    case 1: motor1Speed = stepsPerSecond; break;
    case 2: motor2Speed = stepsPerSecond; break;
    case 3: motor3Speed = stepsPerSecond; break;
  }
}

// Função para girar o motor para a direita (sentido horário) com um ângulo em graus
void rotateRight(int motor, int angle) {
  int stepsToMove = angleToSteps(angle);  // Converte o ângulo para o número de passos

  // Habilita o motor antes de girar
  enableMotor(motor);

  for (int i = 0; i < stepsToMove; i++) {
    stepMotor(motor, i % 4);  // Passo completo
    delay(getStepDelay(motor));  // Ajuste de tempo baseado na velocidade
  }

  // Desabilita o motor após a rotação
  disableMotor(motor);
}

// Função para girar o motor para a esquerda (sentido anti-horário) com um ângulo em graus
void rotateLeft(int motor, int angle) {
  int stepsToMove = angleToSteps(angle);  // Converte o ângulo para o número de passos

  // Habilita o motor antes de girar
  enableMotor(motor);

  for (int i = stepsToMove; i >= 0; i--) {
    stepMotor(motor, i % 4);  // Passo completo
    delay(getStepDelay(motor));  // Ajuste de tempo baseado na velocidade
  }

  // Desabilita o motor após a rotação
  disableMotor(motor);
}

// Função para converter o ângulo em graus para o número de passos do motor
int angleToSteps(int angle) {
  // Número de passos por rotação completa
  int stepsPerRotation = 2048;

  // Converte o ângulo em graus para o número de passos
  return (angle * stepsPerRotation) / 360;
}

// Função para obter o tempo de delay baseado na velocidade (passos por segundo) para cada motor
unsigned long getStepDelay(int motor) {
  int stepsPerSecond = 0;
  
  switch (motor) {
    case 1: stepsPerSecond = motor1Speed; break;
    case 2: stepsPerSecond = motor2Speed; break;
    case 3: stepsPerSecond = motor3Speed; break;
  }
  
  // Tempo entre os passos (em milissegundos)
  float secondsPerStep = 1.0 / stepsPerSecond;  // Calcula o tempo por passo em segundos
  return (unsigned long)(secondsPerStep * 1000);  // Converte para milissegundos
}

// Função que ativa a sequência de passos no motor
void stepMotor(int motor, int step) {
  int in1, in2, in3, in4;

  switch (motor) {
    case 1:
      in1 = MOTOR1_IN1_PIN;
      in2 = MOTOR1_IN2_PIN;
      in3 = MOTOR1_IN3_PIN;
      in4 = MOTOR1_IN4_PIN;
      break;
    case 2:
      in1 = MOTOR2_IN1_PIN;
      in2 = MOTOR2_IN2_PIN;
      in3 = MOTOR2_IN3_PIN;
      in4 = MOTOR2_IN4_PIN;
      break;
    case 3:
      in1 = MOTOR3_IN1_PIN;
      in2 = MOTOR3_IN2_PIN;
      in3 = MOTOR3_IN3_PIN;
      in4 = MOTOR3_IN4_PIN;
      break;
  }

  digitalWrite(in1, steps[step][0]);
  digitalWrite(in2, steps[step][1]);
  digitalWrite(in3, steps[step][2]);
  digitalWrite(in4, steps[step][3]);
}

// Função para habilitar os pinos de controle do motor
void enableMotor(int motor) {
  int in1, in2, in3, in4;

  switch (motor) {
    case 1:
      in1 = MOTOR1_IN1_PIN;
      in2 = MOTOR1_IN2_PIN;
      in3 = MOTOR1_IN3_PIN;
      in4 = MOTOR1_IN4_PIN;
      break;
    case 2:
      in1 = MOTOR2_IN1_PIN;
      in2 = MOTOR2_IN2_PIN;
      in3 = MOTOR2_IN3_PIN;
      in4 = MOTOR2_IN4_PIN;
      break;
    case 3:
      in1 = MOTOR3_IN1_PIN;
      in2 = MOTOR3_IN2_PIN;
      in3 = MOTOR3_IN3_PIN;
      in4 = MOTOR3_IN4_PIN;
      break;
  }

  // Habilita os pinos de controle
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

// Função para desabilitar os pinos de controle do motor
void disableMotor(int motor) {
  int in1, in2, in3, in4;

  switch (motor) {
    case 1:
      in1 = MOTOR1_IN1_PIN;
      in2 = MOTOR1_IN2_PIN;
      in3 = MOTOR1_IN3_PIN;
      in4 = MOTOR1_IN4_PIN;
      break;
    case 2:
      in1 = MOTOR2_IN1_PIN;
      in2 = MOTOR2_IN2_PIN;
      in3 = MOTOR2_IN3_PIN;
      in4 = MOTOR2_IN4_PIN;
      break;
    case 3:
      in1 = MOTOR3_IN1_PIN;
      in2 = MOTOR3_IN2_PIN;
      in3 = MOTOR3_IN3_PIN;
      in4 = MOTOR3_IN4_PIN;
      break;
  }

  // Desabilita os pinos de controle (coloca-os em modo INPUT)
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
}
