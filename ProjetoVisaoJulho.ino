const int botao = A0;
int val = 0;

const int trigPinD = 3;
const int echoPinD = 2;
const int vibra_call_direita = A1;

const int trigPinF = 5;
const int echoPinF = 4;
const int piezo = 8;

const int trigPinE = 7;
const int echoPinE = 6;
const int vibra_call_esquerda = A2;

float largura_corredor = 100; //1 metro
float distancia_min = 15; // 10 cm
float sensorD = 0;
float sensorF = 0;
float sensorE = 0;

bool livreD = false;
bool livreF = false;
bool livreE = false;

float distancia_frente = 40;
float distancia_direita = 40;
float distancia_esquerda = 40;

void setup() {
  pinMode(botao, INPUT);
  pinMode(trigPinD, OUTPUT);
  pinMode(echoPinD, INPUT);
  pinMode(vibra_call_direita, OUTPUT);

  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(piezo, OUTPUT);
  
  pinMode(trigPinE, OUTPUT);
  pinMode(echoPinE, INPUT);
  pinMode(vibra_call_esquerda, OUTPUT);

  pinMode(botao, INPUT);
  Serial.begin(9600);
}

float perto(int trigPin, int echoPin) {
  long distancia = 0;
  for(int i = 0; i<10; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long tempo = pulseIn(echoPin, HIGH);
    distancia += tempo*0.0343/2;
  }
  distancia /= 10;
  Serial.println(distancia);
  return distancia;
}

// Da 2 beeps para virar a direita
void seguir_direita(){
	digitalWrite(piezo, HIGH);
  	delay(200);
  	digitalWrite(piezo, LOW);
  	delay(200);
  	digitalWrite(piezo, HIGH);
  	delay(200);
    digitalWrite(piezo, LOW);
  //espera 1 segundos para mudar a direçao
  	delay(1000);
}

// Da 3 beeps para virar a direita
void seguir_esquerda(){
	digitalWrite(piezo, HIGH);
  	delay(200);
  	digitalWrite(piezo, LOW);
  	delay(200);
  	digitalWrite(piezo, HIGH);
  	delay(200);
    digitalWrite(piezo, LOW);
  	delay(200);
    digitalWrite(piezo, HIGH);
  	delay(200);
    digitalWrite(piezo, LOW);
    //espera 1 segundos para mudar a direçao
  	delay(1000);
}

//Não habilita para ir pra frente
void seguir_frente(){
	digitalWrite(piezo, LOW);
}

//Habilita continuamente
void seguir_pratras(){
 	digitalWrite(piezo, HIGH); 
}

void labirinto(){
  if(sensorF >= distancia_frente){
    digitalWrite(piezo, LOW);
  }
  else{
    digitalWrite(piezo, HIGH);
  }
  if(sensorD >= distancia_direita){
    analogWrite(vibra_call_direita, 0);
  }
  else{
    analogWrite(vibra_call_direita, 255);
  }
  if(sensorE >= distancia_esquerda){
    analogWrite(vibra_call_esquerda, 0);
  }
  else{
    analogWrite(vibra_call_esquerda, 255);
  }
}


void casos(bool frente, bool direita, bool esquerda){
  if(frente & direita & esquerda){ //111
    Serial.println("Rua sem saida");
    seguir_pratras();
  }
  else if(frente && direita && !esquerda){ //110
    Serial.println("esquerda livre");
    seguir_esquerda();
  }
  else if(frente && !direita && esquerda){ //101
    Serial.println("Direita livre");
    seguir_frente();
  }
  else if(frente && !direita && !esquerda){ //100
    Serial.println("Frente ocupada");
    seguir_direita();
  }
  else if(!frente && direita && esquerda){ //011
    Serial.println("Frente livre");
    seguir_frente();
  }
  else if(!frente && direita && !esquerda){ //010
    Serial.println("Direita ocupada");
    seguir_frente();
  }
  else if(!frente && !direita && esquerda){ //001
    Serial.println("esquerda ocupada");
    seguir_direita();
  }
  else if(!frente && !direita && !esquerda){ //000
    Serial.println("Tudo livre");
    seguir_direita();
  }
}

void vibrar(){
    /*Se a distancia somadas da esquerda é menor que
    a largura do corredor, eh porque o usuario esta no corredor*/
        //Se o sensor estiver muito proximo a parede ativa o vibracall
    if (sensorD <= distancia_min){
      analogWrite(vibra_call_direita, 255);
    }
    // Se nao fica desligado
    else{
      analogWrite(vibra_call_direita, 0);
    }
    //Se o sensor estiver muito proximo a parede ativa o vibracall
    if (sensorE <= distancia_min){
      analogWrite(vibra_call_esquerda, 255);
    }
    // Se nao fica desligado
    else{
      analogWrite(vibra_call_esquerda, 0);
    }
    
    if (sensorF <= distancia_min){
      digitalWrite(piezo, HIGH);
    }
    // Se nao fica desligado
    else{
      digitalWrite(piezo, LOW);
    }
}

void caminhos_livres(){
  	/* Se as distancias somadas eh maior que 
  	a largura do corredor um dos caminhos esta livre*/
      	//Se a direita esta livre
  	float soma = sensorD + sensorE;
  	if (soma >= largura_corredor){
        if (sensorD >= largura_corredor){
          livreD = true;
          //Serial.println("direita livre");
        }
        else{
          livreD = false;
          //Serial.println("direita ocupada");
        }
        //Se a esqueda esta livre
        if (sensorE >= largura_corredor){
          livreE = true;
          //Serial.println("Esquerda livre");
        }
        else{
          livreE = false;
          //Serial.println("Esquerda ocupada");
        }
    }
  	else{
		  livreE = false; 
    	livreD = false;
      	//Serial.println("No corredor");
  	}
    //Se a frente esta livre, pela distancia minima
    if (sensorF >= distancia_min){
      livreF = true;
      //Serial.println("Frente livre");
    }
    else{
      livreF = false;
      //Serial.println("Frente ocupada");
    }
  //Serial.println("Saindo");
}

void suspender(){
  int ligado = digitalRead(botao);
  if (ligado == HIGH){
      digitalWrite(piezo, LOW);

      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
      delay(200);
      analogWrite(vibra_call_direita, 255);
      analogWrite(vibra_call_esquerda, 255);
      delay(200);
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
      delay(200);
      analogWrite(vibra_call_direita, 255);
      analogWrite(vibra_call_esquerda, 255);
      delay(200);
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0); 
  }
  while(ligado == HIGH){
    Serial.println("Modo standBy");
    ligado = digitalRead(botao);
    if(ligado == LOW){      
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
      delay(200);
      digitalWrite(piezo, HIGH);
      analogWrite(vibra_call_direita, 255);
      analogWrite(vibra_call_esquerda, 255);
      delay(200);
      digitalWrite(piezo, LOW);
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
      delay(200);
      analogWrite(vibra_call_direita, 255);
      analogWrite(vibra_call_esquerda, 255);
      delay(200);
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
      delay(200);
      analogWrite(vibra_call_direita, 255);
      analogWrite(vibra_call_esquerda, 255);
      delay(200);
      analogWrite(vibra_call_direita, 0);
      analogWrite(vibra_call_esquerda, 0);
    }
  }
}

void loop() { 
  //val = digitalRead(botao);
  //if(val == HIGH) {
  sensorF = perto(trigPinF, echoPinF);
  sensorD = perto(trigPinD, echoPinD);
  sensorE = perto(trigPinE, echoPinE);
  //vibrar();
  //caminhos_livres();
  //casos(!livreF, !livreD, !livreE);
  labirinto();
  suspender();
}
