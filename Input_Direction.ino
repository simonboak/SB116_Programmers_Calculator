void getInput() {
  char key = keypad.getKey();

  // User has pressed a button in this loop
  if (key != NO_KEY) {
    if (
      (key == '!') ||
      (key == '@') ||
      (key == '#') ||
      (key == '$') ||
      (key == '%')
    ) {
      pressedSoftkey(key);
    } else if (key == 'E') { // E for Exit out of the current mode, back to Register mode
      if (currentMode == REGISTER_DISPLAY) {
        X = 0; // bonus feature: Exit also clears X reg if in reg mode.
      }
      
      // Exit back to the main register display mode and reset any required variables
      currentMode = REGISTER_DISPLAY;
      currentSoftkeyMenu = MAIN_MENU;
      
      storageModeWaitingOnInput = false;
      storageOperation = 0;

      //stackModeWaitingOnInput = false;
    } else {
      // Send input to the current mode
      switch (currentMode) {
        case REGISTER_DISPLAY:
          registerModeInputHandler(key);
          break;
        case STORE_SCREEN:
          storeModeInputHandler(key);
          break;
        case STACK_SCREEN:
          stackModeInputHandler(key);
          break;
      }
    }
  }
    
}
