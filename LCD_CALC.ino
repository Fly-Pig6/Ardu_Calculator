/*
 * Copyright @ZFCX
 * - 计算器
 */

#include <LiquidCrystal.h>
#include <Keypad.h>

/**** 定义引脚 ****/
#define BUZZER A4  // 蜂鸣器
#define CL_BTN A5  // 清屏按钮

#define D4 5  // lcd数据引脚
#define D5 4
#define D6 3
#define D7 2
#define RS_PIN A0
#define EN_PIN A1

LiquidCrystal lcd(RS_PIN, EN_PIN, D4, D5, D6, D7);

/**** Keypad对象 ****/
const byte ROWS = 4;       //四行
const byte COLS = 4;       //四列
char keys[ROWS][COLS] = {  // 按钮代表对应字符
  { '7', '8', '9', '/' },
  { '4', '5', '6', '*' },
  { '1', '2', '3', '-' },
  { '0', '.', '=', '+' }
};
byte rowPins[ROWS] = { 9, 8, 7, 6 };      //Keypad所有行的引脚
byte colPins[COLS] = { 13, 12, 11, 10 };  //Keypad所有列的引脚

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char op = ' ';
String num1;
String num2;
String result;

/* 屏幕上显示输出结果 
 */
void displayChars() {
  String txt = num1 + " " + op + " " + num2;
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(txt);  // 显示第一行过程
  lcd.setCursor(0, 1);
  lcd.print(result);  // 显示第二行结果
}

/* 计算结果
 */
String calculate() {
  switch (op) {  // 计算结果
    case '/': return String(num1.toFloat() / num2.toFloat());
    case '*': return String(num1.toFloat() * num2.toFloat());
    case '-': return String(num1.toFloat() - num2.toFloat());
    case '+': return String(num1.toFloat() + num2.toFloat());
    default: return "Syntax ERROR";
  }
}

void setup() {
  pinMode(CL_BTN, INPUT_PULLUP);
  lcd.begin(16, 2);         // lcd初始化, 多少行, 多少列
  lcd.print("@ZFCX Calc");  // 显示计算器初始化界面

  pinMode(BUZZER, OUTPUT);  // 蜂鸣器报警
  for (byte i = 0; i < 4; i++) {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    delay(100);
  }
  digitalWrite(BUZZER, LOW);

  lcd.clear();
}

void loop() {
  if (num1 == "") {  // 待机时, 第一行总是显示'0'
    lcd.setCursor(0, 0);
    lcd.print("0");
  }

  char key = keypad.getKey();  // 读取Keypad

  if (digitalRead(CL_BTN) == LOW) {
    // 按下按钮后一切重置
    op = ' ';
    num1 = "", num2 = "", result = "";
    lcd.clear();

    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
  }

  if (key && result == "") {
    // 按钮被按下
    if (key != '=') {

      if (key != '/' && key != '*' && key != '-' && key != '+') {
        // 按下数字键或小数点键
        if (op == ' ') {
          // 正在输入第一个数
          if (num1.length() >= 6 || (key == '.' && num1.indexOf('.') >= 0)) return;  // 确保小数点不重复输入, 位数不超限
          num1 = num1 + key;
        } else {
          // 正在输入第二个数
          if (num2.length() >= 6 || (key == '.' && num2.indexOf('.') >= 0)) return;  // 确保小数点不重复输入, 位数不超限
          num2 = num2 + key;
        }

      } else {
        // 按下加减乘除键
        if (op != ' ') return;  // 防止重复输入
        op = key;
      }

    } else {
      // 按下等于键
      result = calculate();
    }
    displayChars();
  }
}
