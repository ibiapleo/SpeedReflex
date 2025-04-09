# 🎮 SpeedReflex

![Foto do Projeto Montado](imgs/Prototipo.jpg)

Um sistema interativo com três modos de jogo para testar reflexos, desenvolvido para Arduino Mega com display LCD 20x4.

## 📋 Funcionalidades Principais
- **Modo Treinamento**: Medição precisa do tempo de reação
- **Modo Competitivo**: Duelo multiplayer (melhor de 5 rodadas)
- **Modo Desafio**: 5 fases com dificuldade progressiva
- **Feedback completo**:
  - Visual (LEDs + LCD)
  - Sonoro (buzzer melódico)
  - Dados estatísticos no display

## 🛠 Hardware Utilizado
| Componente              | Quantidade | Especificações |
|-------------------------|------------|----------------|
| Arduino Mega 2560       | 1          | ATmega2560     |
| LCD 20x4 com I2C        | 1          | Compatível com 16x2 |
| Botões arcade           | 5          | 12mm, 4 pinos  |
| LEDs difusos            | 7          | 5mm (4Verm, 3Ver) |
| Buzzer passivo          | 1          | 5V DC          |
| Módulo I2C              | 1          | Para LCD        |

## 🔌 Diagrama de Conexões
```plaintext
ARDUINO MEGA PINOUT:
-------------------
Botões:
- Navegação Cima → 13
- Seleção Centro → 12
- Navegação Baixo → 11
- Jogador 1 → 10
- Jogador 2 → 9

LEDs:
- Verde Jogador 1 → 8
- Verde Jogador 2 → 7 
- Verde Central → 6
- Vermelhos → 2-5 (contagem)

Buzzer → 22
LCD I2C → SDA (20), SCL (21)
```

## 🚀 Instalação
1. Conectar hardware conforme diagrama
2. Carregar o sketch:
```bash
# Clone o repositório
git clone https://github.com/ibiapleo/SpeedReflex.git
# Abra jogo_reacao_mega.ino na IDE Arduino
```

## 🎮 Modos de Operação
### 1. Treinamento
- 5 testes cronometrados
- Calcula média de tempos
- Exibe histórico no LCD

### 2. Competitivo
- Melhor de 5 rodadas
- Sistema de placar
- LEDs de vitória

### 3. Desafio
- Níveis:
  1. Fácil (1.5s)
  2. Intermediário (1s)
  3. Difícil (0.6s) 
  4. Expert (0.3s)
  5. Extremo (0.2s)

## 📊 Especificações Técnicas
- Tensão de operação: 5V DC
- Bibliotecas necessárias:
  - `LiquidCrystal_I2C`
  - `Wire`

## 📝 Documentação Adicional
- [Vídeo Demonstração](https://youtu.be/gLOiu7hUqJU)

## 🌟 Recursos Avançados
```cpp
// Exemplo de configuração do LCD
void setupLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Sistema Pronto!");
}
```
