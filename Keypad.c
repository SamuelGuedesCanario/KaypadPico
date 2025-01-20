
const int rowPins[4] = {1, 2, 3, 4}; 
const int colPins[4] = {5, 6, 7, 8}; 
char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


const int redPin = 10;
const int greenPin = 11;
const int bluePin = 12;


const int buzzerPin = 21;

// Função para inicializar o buzzer com PWM
void pwm_init_buzzer() {
    pinMode(buzzerPin, OUTPUT);
}

// Função para emitir um beep no buzzer
void beep(int frequency, int duration) {
    ledcWriteTone(0, frequency);  // Especifica a frequência do buzzer
    delay(duration);               // Duração do som
    ledcWriteTone(0, 0);           // Para o som
}

void setup() {
    Serial.begin(115200);

    
    for (int i = 0; i < 4; i++) {
        pinMode(rowPins[i], OUTPUT);
        digitalWrite(rowPins[i], HIGH); 
        pinMode(colPins[i], INPUT_PULLUP); 
    }

    
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);

    
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(buzzerPin, LOW);

    Serial.println("Teclado matricial pronto com LEDs e Buzzer!");
}

void loop() {
    for (int i = 0; i < 4; i++) {
        
        digitalWrite(rowPins[i], LOW);
        for (int j = 0; j < 4; j++) {
            /
            if (digitalRead(colPins[j]) == LOW) {
                char key = keys[i][j];
                Serial.print("Tecla pressionada: ");
                Serial.println(key);
                delay(200); 

                
                switch (key) {
                    case '1': 
                        digitalWrite(redPin, HIGH);
                        delay(500);
                        digitalWrite(redPin, LOW);
                        break;
                    case '2': 
                        digitalWrite(greenPin, HIGH);
                        delay(500);
                        digitalWrite(greenPin, LOW);
                        break;
                    case '3':
                        digitalWrite(bluePin, HIGH);
                        delay(500);
                        digitalWrite(bluePin, LOW);
                        break;
                    case 'A': 
                        beep(1000, 500);  // Chama a função beep com frequência de 1000Hz e duração de 500ms
                        break;
                    default:
                        Serial.println("Tecla sem função atribuída.");
                        break;
                }

                
                while (digitalRead(colPins[j]) == LOW);
            }
        }
        
        digitalWrite(rowPins[i], HIGH);
    }
}
