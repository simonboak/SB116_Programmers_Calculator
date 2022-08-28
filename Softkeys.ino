/*
 * Softkeys manager
 * A really kludgy way of handling a nested menu structure that I just made up as I went along.
 */


/**
 * Displays the keys for the current softkey menu
 */
void updateSoftkeys() {
  if (currentMode != BOOT_SCREEN) {
    switch (currentSoftkeyMenu) {
      case MAIN_MENU:
        drawSoftKey(0, "STORE", true);
        drawSoftKey(1, "STACK", true);
        drawSoftKey(4, "INFO", true);
        break;
      case INFO:
        //drawSoftKey(4, "EXIT", true);
        break;
      case STORE_MENU:
        if (!storageModeWaitingOnInput) {
          drawSoftKey(0, "READ", false);
          drawSoftKey(1, "WRITE", false);
          drawSoftKey(3, "PREV", true);
          drawSoftKey(4, "NEXT", true);
        }
        break;
      case STACK_MENU:
        if (!storageModeWaitingOnInput) {
          drawSoftKey(0, "PULL", false);
          
          if (stackPointer < 15) {
            drawSoftKey(1, "PUSH", false);
          }
          
          drawSoftKey(2, "CLEAR", false);

          if (stackPointer > 3) {
            drawSoftKey(3, "PREV", true);
            drawSoftKey(4, "NEXT", true);
          }
        }
        break;
    }
  }
}


/**
 * Decide what to do depending on the current menu and softkey that is pressed
 */
void pressedSoftkey(char key) {
  switch (currentSoftkeyMenu) {
    case MAIN_MENU:
      switch (key) {
        case '!':
          currentSoftkeyMenu = STORE_MENU;
          currentMode = STORE_SCREEN;
          break;
        case '@':
          currentSoftkeyMenu = WORD_SIZE;
          currentSoftkeyMenu = STACK_MENU;
          currentMode = STACK_SCREEN;
          break;
        case '%':
          currentSoftkeyMenu = INFO;
          currentMode = INFO_SCREEN;
          break;
      }
      break;
    case INFO:
      /*switch (key) {
        
      }*/
      break;
    case STORE_MENU:
      switch (key) {
        case '!':
          // Read handler
          storageOperation = 0;
          storageModeWaitingOnInput = true;
          break;
        case '@':
          // Write handler
          storageOperation = 1;
          storageModeWaitingOnInput = true;
          break;
        case '$':
          storeModePrevPage();
          break;
        case '%':
          storeModeNextPage();
          break;
      }
      break;
    case STACK_MENU:
      switch (key) {
        case '!':
          // Pull handler
          stackModePull();
          break;
        case '@':
          // Push handler
          stackModePush();
          break;
        case '#':
          stackModeClear();
          break;
        case '$':
          stackModePrevPage();
          break;
        case '%':
          stackModeNextPage();
          break;
      }
      break;
  }
}
