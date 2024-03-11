#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;  // hard-wired for UNO shields anyway.
#include <TouchScreen.h>
#include "vector.h"
#include "button.h"

const int XP = 8, XM = A2, YP = A3, YM = 9;  //ID=0x9341
const int TS_LEFT = 66, TS_RT = 916, TS_TOP = 89, TS_BOT = 943;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MIN_PRESSURE 25
#define MAX_PRESSURE 1000
#define BUTTON_SIZE 64
#define PADDING 32

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Vector<Button> buttons = new Vector<Button>(0);

void setup(void) {
  tft.reset();
  // Setup tft id
  uint16_t ID = tft.readID();
  tft.begin(ID);
  Serial.begin(9600);
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  drawButtons(BUTTON_SIZE, PADDING);
}

void loop() {
  uint16_t xpos, ypos;
  tp = ts.getPoint();

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (isPressed(&tp)) {
	xpos = map(tp.y, TS_LEFT, TS_RT, 0, 320);
	ypos = map(tp.x, TS_TOP, TS_BOT, 0, 240);

	for (uint8_t i = 0; i < buttons.length(); i++) {
	  Button* btn = buttons[i];

	  if (btn == nullptr) {
		continue;
	  }

	  if (!btn->inButtonArea(xpos, ypos)) {
		continue;
	  }

	  // This tells the program its a button msg
	  byte btnMsg[8];
	  btnMsg[0] = "b";
	  btnMsg[1] = btn->getId();
	  btnMsg[2] = 0;
	  btnMsg[3] = 0;
	  btnMsg[4] = 0;
	  btnMsg[5] = 0;
	  btnMsg[6] = 0;
	  btnMsg[7] = 0;

	  Serial.write(btnMsg, 8);
	}
  }
}

bool isPressed(TSPoint* p) {
  return p->z > MIN_PRESSURE && p->z < MAX_PRESSURE;
}

void drawButtons(uint8_t btnSize, uint8_t padding) {
  buttons.clear();
  uint8_t cols = tft.width() / (btnSize + padding);
  uint8_t rows = tft.height() / (btnSize + padding);

  uint16_t offsetX = (tft.width() - (cols * btnSize) - ((cols - 1) * padding)) / 2;
  uint16_t offsetY = (tft.height() - (rows * btnSize) - ((rows - 1) * padding)) / 2;

  uint8_t id = 0;
  for (uint8_t row = 0; row < rows; row++) {
	for (uint8_t col = 0; col < cols; col++) {
	  uint16_t x = (col * (btnSize + padding)) + offsetX;
	  uint16_t y = (row * (btnSize + padding)) + offsetY;
	  tft.fillRect(x, y, btnSize, btnSize, WHITE);

	  Button button(x, y, id, btnSize);
	  buttons.push_back(button);

	  id++;
	}
  }
}
