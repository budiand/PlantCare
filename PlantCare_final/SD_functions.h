void write_time(unsigned long start, unsigned long finished, unsigned long elapsed, File file) {

  float h, m, s, ms;
  unsigned long over;

  elapsed = finished - start;
  h = int(elapsed / 3600000);
  over = elapsed % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;

  if (file) {
    file.print("Elapsed time: ");
    file.print(h,0);
    file.print("h ");
    file.print(m,0);
    file.print("m ");
    file.print(s,0);
    file.print("s ");
    file.print(ms,0);
    file.println("ms");
    file.println();
    Serial.println("Written");
    
    file.close();
  } 
}