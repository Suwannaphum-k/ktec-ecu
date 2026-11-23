// เพิ่มในสเก็ตช์เกจ (ESP32 + WebServer) ให้เว็บ KTEC ECU ดึงค่าสดได้
// เปลี่ยนชื่อตัวแปรด้านล่างให้ตรงกับตัวแปรที่เกจของคุณเก็บค่าจาก K-line
// ถ้าค่าไหนเกจไม่มี ให้ลบบรรทัดนั้นออก เว็บจะแสดงเป็น –

// ---- ตัวแปรของคุณ (ตัวอย่างชื่อ ต้องแก้) ----
extern float g_rpm;      // รอบเครื่อง rpm
extern float g_tps;      // TPS %
extern float g_map;      // MAP kPa
extern float g_ect;      // น้ำหล่อเย็น °C
extern float g_batt;     // แบต V
extern float g_afr;      // AFR (แปลงจาก O2 แล้ว)
extern float g_gpsKmh;   // ความเร็ว GPS km/h
extern float g_inj;      // ระยะฉีด ms (ถ้ามี)
extern float g_ign;      // องศาไฟ °BTDC (ถ้ามี)
extern bool  g_ecuOnline; // ต่อ ECU ได้อยู่หรือเปล่า

// ---- เพิ่มใน setup() หลังสร้าง server แล้ว ----
//   server.on("/data", HTTP_GET, handleData);
//   server.on("/data", HTTP_OPTIONS, handleOptions);

void sendCors() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");
  server.sendHeader("Cache-Control", "no-store");
}

void handleOptions() {
  sendCors();
  server.send(204);
}

void handleData() {
  sendCors();
  char buf[256];
  snprintf(buf, sizeof(buf),
    "{\"ecu\":%d,\"rpm\":%.0f,\"tps\":%.1f,\"map\":%.0f,\"ect\":%.0f,"
    "\"batt\":%.1f,\"afr\":%.1f,\"spd\":%.0f,\"inj\":%.2f,\"ign\":%.1f}",
    g_ecuOnline ? 1 : 0, g_rpm, g_tps, g_map, g_ect,
    g_batt, g_afr, g_gpsKmh, g_inj, g_ign);
  server.send(200, "application/json", buf);
}
