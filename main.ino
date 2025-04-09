#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #include <EEPROM.h> implementação futura
// #include <string.h> implementação futura

LiquidCrystal_I2C lcd(0x27, 16, 4);

// botões
const int botaoCima = 13;
const int botaoCentro = 12;
const int botaoBaixo = 11;
const int botaoJogador1 = 10;
const int botaoJogador2 = 9;

// LEDs
const int ledGreenJ1 = 8;
const int ledGreenJ2 = 7;
const int ledGreen = 6;
const int ledRed1 = 2;
const int ledRed2 = 3;
const int ledRed3 = 4;
const int ledRed4 = 5;

// estados dos botões principais
int estBotaoCima = 0;
int estBotaoCentro = 0;
int estBotaoBaixo = 0;
int estadoBotaoJogador1 = 0;
int estadoBotaoJogador2 = 0;

// Buzzer
int pinoBuzzer = 22;
int notasVitoria[] = { 392, 523, 440, 523, 392 };
int duracoesVitoria[] = { 300, 300, 300, 300, 300 };
int notasDerrota[] = { 349, 330, 311, 250}; 
int duracoesDerrota[] = { 300, 300, 300, 500 };

// Configurações de Menu
int modoSelecionado = 0;
const char * modos[] = {
    "Modo Treinamento",
    "Modo Competitivo",
    "Modo Desafio"
};
const int numModos = 3;

unsigned long tempoInicio = 0;
unsigned long tempoFinal = 0;
unsigned long tempoInicioJ1 = 0;
unsigned long tempoInicioJ2 = 0;
bool treinamentoAtivo = false;
bool jogoAtivo = false;
bool ledVerdeAcendeu = false;

int vitoriasJogador1 = 0;
int vitoriasJogador2 = 0;
int rodadaAtual = 1;
const int numRodadas = 5;
unsigned long tempoEspera = 0;

unsigned long tempoVerdeAcendido = 0; // Marca o tempo em que o LED verde acendeu
unsigned long tempoPressionado = 0; // Armazena o tempo total em que o botão foi pressionado
unsigned long tempoInicioPressionamento = 0; // Marca o início do pressionamento do botão

void setup() {
    pinMode(botaoCima, INPUT_PULLUP);
    pinMode(botaoCentro, INPUT_PULLUP);
    pinMode(botaoBaixo, INPUT_PULLUP);
    pinMode(botaoJogador1, INPUT_PULLUP);
    pinMode(botaoJogador2, INPUT_PULLUP);

    pinMode(ledGreenJ1, OUTPUT);
    pinMode(ledGreenJ2, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledRed1, OUTPUT);
    pinMode(ledRed2, OUTPUT);
    pinMode(ledRed3, OUTPUT);
    pinMode(ledRed4, OUTPUT);

    pinMode(pinoBuzzer, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    mostrarMenu(modoSelecionado);
    Serial.begin(9600);
}

void loop() {
    estBotaoCima = digitalRead(botaoCima);
    estBotaoCentro = digitalRead(botaoCentro);
    estBotaoBaixo = digitalRead(botaoBaixo);
    estadoBotaoJogador1 = digitalRead(botaoJogador1);
    estadoBotaoJogador2 = digitalRead(botaoJogador2);

    // Navegar para cima
    if (estBotaoCima == HIGH) {
        modoSelecionado++;
        if (modoSelecionado >= numModos) {
            modoSelecionado = 0;
        }
        mostrarMenu(modoSelecionado);
        tone(pinoBuzzer, 355);
        delay(200);
        noTone(pinoBuzzer);
    }

    // Navegar para baixo
    if (estBotaoBaixo == HIGH) {
        modoSelecionado--;
        if (modoSelecionado < 0) {
            modoSelecionado = numModos - 1;
        }
        mostrarMenu(modoSelecionado);
        tone(pinoBuzzer, 355);
        delay(200);
        noTone(pinoBuzzer);
    }

    // Confirmar seleção
    if (estBotaoCentro == HIGH) {
        tone(pinoBuzzer, 555);
        delay(200);
        noTone(pinoBuzzer);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Selecionado: ");
        lcd.setCursor(0, 1);
        lcd.print(modos[modoSelecionado]);
        delay(1500);
        lcd.clear();
        if (modoSelecionado == 0) {
          iniciarModoTreinamento();
        }
        if (modoSelecionado == 1) {
          iniciarModoCompetitivo();
        }
        if (modoSelecionado == 2) {
          iniciarModoDesafio();
        }
    }

}

void mostrarMenu(int modoSelecionado) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Escolher opcao:");

    lcd.setCursor(0, 1);
    lcd.print(modos[modoSelecionado]);
}

void iniciarModoCompetitivo() {
    // Resetar variáveis de vitória e rodada
    int vitoriasJogador1 = 0;
    int vitoriasJogador2 = 0;
    int numRodadas = 5;
    unsigned long tempoVerdeAcendido = 0;
    unsigned long tempoPressionamentoJ1 = 0;
    unsigned long tempoFinalJ1 = 0;
    unsigned long tempoPressionamentoJ2 = 0;
    unsigned long tempoFinalJ2 = 0;

    // Começar as rodadas
    for (int i = 0; i < numRodadas; i++) {
        tempoVerdeAcendido = 0;
        tempoPressionamentoJ1 = 0;
        tempoPressionamentoJ2 = 0;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Rodada ");
        lcd.print(i + 1);
        lcd.setCursor(0, 1);
        lcd.print("Prepare-se!");


        // Acender LEDs vermelhos
        acenderLedsVermelhos();
        // Acionar LED verde em tempo aleatório
        delay(random(2000, 5000));
        digitalWrite(ledGreen, HIGH); // Acender LED verde
        tempoVerdeAcendido = millis();
        // desligar LEDs vermelhos
        digitalWrite(ledRed1, LOW);
        digitalWrite(ledRed2, LOW);
        digitalWrite(ledRed3, LOW);
        digitalWrite(ledRed4, LOW);


        // Marca o tempo quando o botão é pressionado
        while (digitalRead(botaoJogador1) == LOW && digitalRead(botaoJogador2) == LOW) {
            continue;
        }

        if (digitalRead(botaoJogador1) == HIGH) {
            tempoPressionamentoJ1 = millis();
        }

        if (digitalRead(botaoJogador2) == HIGH) {
            tempoPressionamentoJ2 = millis();
        }

        if (tempoPressionamentoJ1 == 0) {
            tempoFinalJ1 = 0;
        } else {
            tempoFinalJ1 = tempoPressionamentoJ1 - tempoVerdeAcendido;
        }
        if (tempoPressionamentoJ2 == 0) {
            tempoFinalJ2 = 0;
        } else {
            tempoFinalJ2 = tempoPressionamentoJ2 - tempoVerdeAcendido;
        }

        // Exibir resultado da rodada e atualizar contagem de vitórias
        if (tempoFinalJ1 > tempoFinalJ2) {
            tone(pinoBuzzer, 400);
            delay(300);
            tone(pinoBuzzer, 600);  
            delay(300);
            noTone(pinoBuzzer);
            vitoriasJogador1++;
            digitalWrite(ledGreenJ1, HIGH);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Vitoria J1!");
            lcd.setCursor(0, 1);
            lcd.print(tempoFinalJ1);
            lcd.print("ms");
            digitalWrite(ledGreen, LOW);
            delay(3000);
        } else if (tempoFinalJ2 > tempoFinalJ1) {
            tone(pinoBuzzer, 400);
            delay(300);
            tone(pinoBuzzer, 600);  
            delay(300);
            noTone(pinoBuzzer);
            vitoriasJogador2++;
            digitalWrite(ledGreenJ2, HIGH);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Vitoria J2!");
            lcd.setCursor(0, 1);
            lcd.print(tempoFinalJ2);
            lcd.print("ms");
            digitalWrite(ledGreen, LOW);
            delay(3000);
        } else {
            tone(pinoBuzzer, 100);
            delay(200);
            tone(pinoBuzzer, 50);  
            delay(200);
            tone(pinoBuzzer, 100);  
            delay(200);
            noTone(pinoBuzzer);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Empate!");
            i--;
            digitalWrite(ledGreen, LOW);
            delay(3000);
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("      J1 - J2      ");
        lcd.setCursor(0, 1);
        lcd.print("       ");
        lcd.print(vitoriasJogador1);
        lcd.print(" - ");
        lcd.print(vitoriasJogador2);
        lcd.print("       ");
        digitalWrite(ledGreenJ1, LOW);
        digitalWrite(ledGreenJ2, LOW);
        delay(2000);

        rodadaAtual++;

        // Verificar se alguém ganhou 3 rodadas
        if (vitoriasJogador1 == 3 || vitoriasJogador2 == 3) {
            mostrarVencedor(vitoriasJogador1, vitoriasJogador2);
            for (int i = 0; i < 3; i++) {
                  tone(pinoBuzzer, notasVitoria[i]); 
                  delay(duracoesVitoria[i]); 
                  noTone(pinoBuzzer);
                  delay(50);
                }
            delay(4000);
            return; // Finaliza o jogo se alguém vencer 3 rodadas
        }

    }
}

void acenderLedsVermelhos() {
    tone(pinoBuzzer, 1000);
    delay(200);
    noTone(pinoBuzzer);
    digitalWrite(ledRed1, HIGH);
    delay(2000);
    
    tone(pinoBuzzer, 1000);
    delay(200);
    noTone(pinoBuzzer);
    digitalWrite(ledRed2, HIGH);
    delay(2000);
    
    tone(pinoBuzzer, 1000);
    delay(200);
    noTone(pinoBuzzer);
    digitalWrite(ledRed3, HIGH);
    delay(2000);
    
    tone(pinoBuzzer, 1000);
    delay(200);
    noTone(pinoBuzzer);
    digitalWrite(ledRed4, HIGH);
    delay(2000);
}

void mostrarVencedor(int vitoriasJogador1, int vitoriasJogador2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (vitoriasJogador1 > vitoriasJogador2) {
        lcd.print("Vitoria Final: ");
        lcd.setCursor(0, 1);
        lcd.print("Jogador 1!");
    } else {
        lcd.print("Vitoria Final: ");
        lcd.setCursor(0, 1);
        lcd.print("Jogador 2!");
    }
    delay(3000);
    mostrarMenu(modoSelecionado);
}

void iniciarModoTreinamento() {
    unsigned long tempos[5]; // Array para armazenar os tempos de resposta
    unsigned long somaTempos = 0; // Variável para armazenar a soma dos tempos
    unsigned long tempoMedio = 0; // Variável para armazenar o tempo médio
    unsigned long tempoInicioPressionamento = 0;
    int numTestes = 5; // Número de testes

    for (int i = 0; i < numTestes; i++) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Teste ");
        lcd.print(i + 1);
        lcd.setCursor(0, 1);
        lcd.print("Aguarde LED verde");
        delay(1500); // Espera 1,5 segundos entre os testes

        // Acende o LED verde e começa a medir o tempo de pressionamento após o LED acender
        tempoVerdeAcendido = 0; // Reseta o tempo do LED verde
        tempoPressionado = 0; // Reseta o tempo de pressionamento
        tempoInicioPressionamento = 0;

        // Aguarda o LED verde acender
        delay(random(1000, 5000));
        digitalWrite(ledGreen, HIGH); // Acende o LED verde
        tempoVerdeAcendido = millis(); // Marca o momento que o LED verde acende

        // Marca o tempo quando o botão é pressionado
        while (digitalRead(botaoJogador1) == LOW) {
            continue;
        }

        if (digitalRead(botaoJogador1) == HIGH) {
            tempoInicioPressionamento = millis();
        }

        // Calcula o tempo total de pressionamento
        tempoPressionado = tempoInicioPressionamento - tempoVerdeAcendido;

        // Armazena o tempo de resposta no array
        tempos[i] = tempoPressionado;
        somaTempos += tempos[i]; // Soma os tempos

        // Exibe o tempo de resposta no LCD
        tone(pinoBuzzer, 400);
        delay(300);
        tone(pinoBuzzer, 600);  
        delay(300);  
        noTone(pinoBuzzer);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempo: ");
        lcd.print(tempoPressionado);
        lcd.print(" ms");
        delay(3000);

        // Reseta as variáveis para o próximo teste
        digitalWrite(ledGreen, LOW); // Desliga o LED verde
        tempoVerdeAcendido = 0; // Reseta o estado do LED verde
    }

    // Calcula a média dos tempos
    tempoMedio = somaTempos / numTestes;

    // Exibe a média dos tempos no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Media dos tempos:");
    lcd.setCursor(0, 1);
    lcd.print(tempoMedio);
    lcd.print(" ms");

    delay(3000); // Exibe a média por 3 segundos

    // Volta ao menu principal após o teste
    mostrarMenu(modoSelecionado);
}

void iniciarModoDesafio() {
    unsigned long tempoVerdeAcendido = 0;
    unsigned long tempoPressionamento = 0;
    unsigned long tempoFinal = 0;
    int i = 0;

    // Começar as rodadas
    while (i < 5) {
        tempoVerdeAcendido = 0;
        tempoPressionamento = 0;

        lcd.clear();
        lcd.setCursor(0, 0);
        if (i == 0) {
            lcd.print("Nv Facil (1s)");
            lcd.setCursor(0, 1);
            lcd.print("Prepare-se!");
            delay(2000);
        }
        if (i == 1) {
            lcd.print("Nv Simples (0.5s)");
            lcd.setCursor(0, 1);
            lcd.print("Prepare-se!");
            delay(2000);
        }
        if (i == 2) {
            lcd.print("Nv Medio (0.3s)");
            lcd.setCursor(0, 1);
            lcd.print("Prepare-se!");
            delay(2000);
        }
        if (i == 3) {
            lcd.print("Dificil! (0.21s)");
            lcd.setCursor(0, 1);
            lcd.print("Prepare-se!");
            delay(2000);
        }
        if (i == 4) {
            lcd.print("Extremo! (0.1s)");
            lcd.setCursor(0, 1);
            lcd.print("Prepare-se!");
            delay(2000);
        }


        // Acender LEDs vermelhos sequencialmente a cada 2 segundos
        acenderLedsVermelhos();
        // Acionar LED verde em tempo aleatório
        if (i == 0) {
            delay(random(1000, 1500));
            digitalWrite(ledGreen, HIGH); // Acender LED verde
            tempoVerdeAcendido = millis();
            digitalWrite(ledRed1, LOW);
            digitalWrite(ledRed2, LOW);
            digitalWrite(ledRed3, LOW);
            digitalWrite(ledRed4, LOW);
        }
        if (i == 1) {
            delay(random(300, 1000));
            digitalWrite(ledGreen, HIGH); // Acender LED verde
            tempoVerdeAcendido = millis();
            digitalWrite(ledRed1, LOW);
            digitalWrite(ledRed2, LOW);
            digitalWrite(ledRed3, LOW);
            digitalWrite(ledRed4, LOW);
        }
        if (i == 2) {
            delay(random(300, 600));
            digitalWrite(ledGreen, HIGH); // Acender LED verde
            tempoVerdeAcendido = millis();
            digitalWrite(ledRed1, LOW);
            digitalWrite(ledRed2, LOW);
            digitalWrite(ledRed3, LOW);
            digitalWrite(ledRed4, LOW);
        }
        if (i == 3) {
            delay(random(100, 300));
            digitalWrite(ledGreen, HIGH); // Acender LED verde
            tempoVerdeAcendido = millis();
            digitalWrite(ledRed1, LOW);
            digitalWrite(ledRed2, LOW);
            digitalWrite(ledRed3, LOW);
            digitalWrite(ledRed4, LOW);
        }
        if (i == 4) {
            delay(random(50, 200));
            digitalWrite(ledGreen, HIGH); // Acender LED verde
            tempoVerdeAcendido = millis();
            digitalWrite(ledRed1, LOW);
            digitalWrite(ledRed2, LOW);
            digitalWrite(ledRed3, LOW);
            digitalWrite(ledRed4, LOW);
        }


        // Marca o tempo quando o botão é pressionado
        while (digitalRead(botaoJogador1) == LOW) {}

        if (digitalRead(botaoJogador1) == HIGH) {
            tempoPressionamento = millis();
        }

        tempoFinal = tempoPressionamento - tempoVerdeAcendido;

        // Exibir resultado da rodada e atualizar contagem de vitórias
        if (i == 0) {
            if (tempoFinal < 1000) {
                digitalWrite(ledGreen, LOW);
                digitalWrite(ledGreenJ1, HIGH);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Venceu o Facil!");
                lcd.setCursor(0, 1);
                lcd.print(tempoFinal);
                lcd.print("ms");
                tone(pinoBuzzer, 400);
                delay(300);
                tone(pinoBuzzer, 600);  
                delay(300);
                noTone(pinoBuzzer);  
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Voce perdeu!");
                lcd.setCursor(0, 1);
                lcd.print("Faltou reflexo!");
                tone(pinoBuzzer, 50);
                delay(300);
                tone(pinoBuzzer, 100);  
                delay(300);
                tone(pinoBuzzer, 50);
                delay(300);
                noTone(pinoBuzzer);  
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
                break;
            }
        }
        if (i == 1) {
            if (tempoFinal < 500) {
                digitalWrite(ledGreen, LOW);
                digitalWrite(ledGreenJ1, HIGH);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Venceu o Simples!");
                lcd.setCursor(0, 1);
                lcd.print(tempoFinal);
                lcd.print("ms");
                tone(pinoBuzzer, 400);
                delay(300);
                tone(pinoBuzzer, 600);  
                delay(300);
                noTone(pinoBuzzer); 
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Voce perdeu!");
                lcd.setCursor(0, 1);
                lcd.print("Faltou reflexo!");
                tone(pinoBuzzer, 50);
                delay(300);
                tone(pinoBuzzer, 100);  
                delay(300);
                tone(pinoBuzzer, 50);
                delay(300);
                noTone(pinoBuzzer);  
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
                break;
            }
        }
        if (i == 2) {
            if (tempoFinal < 300) {
                digitalWrite(ledGreen, LOW);
                digitalWrite(ledGreenJ1, HIGH);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Venceu o Medio!");
                lcd.setCursor(0, 1);
                lcd.print(tempoFinal);
                lcd.print("ms");
                tone(pinoBuzzer, 400);
                delay(300);
                tone(pinoBuzzer, 600);  
                delay(300);
                noTone(pinoBuzzer); 
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Voce perdeu!");
                lcd.setCursor(0, 1);
                lcd.print("Faltou reflexo!");
                tone(pinoBuzzer, 50);
                delay(300);
                tone(pinoBuzzer, 100);  
                delay(300);
                tone(pinoBuzzer, 50);
                delay(300);
                noTone(pinoBuzzer);
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
                break;
            }
        }
        if (i == 3) {
            if (tempoFinal < 210) {
                digitalWrite(ledGreen, LOW);
                digitalWrite(ledGreenJ1, HIGH);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Venceu o Dificil!");
                lcd.setCursor(0, 1);
                lcd.print(tempoFinal);
                lcd.print("ms");
                tone(pinoBuzzer, 400);
                delay(300);
                tone(pinoBuzzer, 600);  
                delay(300);
                noTone(pinoBuzzer); 
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Voce perdeu!");
                lcd.setCursor(0, 1);
                lcd.print("Faltou reflexo!");
                tone(pinoBuzzer, 50);
                delay(300);
                tone(pinoBuzzer, 100);  
                delay(300);
                tone(pinoBuzzer, 50);
                delay(300);
                noTone(pinoBuzzer);
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
                break;
            }
        }
        if (i == 4) {
            if (tempoFinal < 100) {
                digitalWrite(ledGreen, LOW);
                digitalWrite(ledGreenJ1, HIGH);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Venceu o Extremo!");
                lcd.setCursor(0, 1);
                lcd.print(tempoFinal);
                lcd.print("ms");
                for (int i = 0; i < 3; i++) {
                  tone(pinoBuzzer, notasVitoria[i]); 
                  delay(duracoesVitoria[i]); 
                  noTone(pinoBuzzer);
                  delay(50);
                }
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Voce perdeu!");
                lcd.setCursor(0, 1);
                lcd.print("Faltou reflexo!");
                for (int i = 0; i < 3; i++) {
                  tone(pinoBuzzer, notasDerrota[i]); 
                  delay(duracoesDerrota[i]); 
                  noTone(pinoBuzzer);
                  delay(50);
                }
                delay(3000);
                digitalWrite(ledGreenJ1, LOW);
                digitalWrite(ledGreen, LOW);
                break;
            }
        }

        i++;

    }

    delay(2000);
    mostrarMenu(modoSelecionado);
    return;
}