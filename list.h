
void printBuffer(void) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  Heltec.display->setLogBuffer(5, 30);

  // Some test data
  const char* test[] = {
      "Hello",
      "World" ,
      "----",
      "Show off",
      "how",
      "the log buffer",
      "is",
      "working.",
      "Even",
      "scrolling is",
      "working"
  };

  for (uint8_t i = 0; i < 11; i++) {
    Heltec.display->clear();
    // Print to the screen
    Heltec.display->println(test[i]);
    // Draw it to the internal screen buffer
    Heltec.display->drawLogBuffer(0, 0);
    // Display it on the screen
    Heltec.display->display();
    delay(500);
  }
}
