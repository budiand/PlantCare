 // function to print messages to lcd display
 
  void print_lcd_message(LCD_I2C lcd, int moisture_value, float temp_value) {
 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.write(1);
    lcd.print("PlantCare");
    lcd.write(1);
    delay(3000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Moisture: ");
    lcd.setCursor(0,1);
    if(moisture_value < 350) {
      lcd.print("HIGH");
      lcd.print("(");
      lcd.print(moisture_value);
      lcd.print(")");
    }
    else if(moisture_value >=350 && moisture_value <=850) {
      lcd.print("MODERATE");
      lcd.print("(");
      lcd.print(moisture_value);
      lcd.print(")");
    }
    else if(moisture_value > 850) {
      lcd.print("LOW");
      lcd.print("(");
      lcd.print(moisture_value);
      lcd.print(")");
    }
    delay(3000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature: ");
    lcd.setCursor(0,1);
    lcd.print(temp_value);
    lcd.write((byte)0);
    lcd.print("C");
    delay(3000);
  }