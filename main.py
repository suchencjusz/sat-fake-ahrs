import serial
import serial.tools.list_ports
import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect

ports = serial.tools.list_ports.comports()
for idx,(port, desc, hwid) in enumerate(ports):
    print(f"[{idx}] {port}: {desc} [{hwid}]")

chosen_port = int(input("Select port: "))


ser = serial.Serial(ports[chosen_port][0], baudrate=115200, timeout=3)
print("connected")

print("[1] print")
print("[2] save to file")
print("[3] send to websocket")
mode = int(input("Enter mode: "))

app = FastAPI()

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        try:
            data = ser.readline().decode().strip()
            if data:
                print(f"Data from serial: {data}")
                await websocket.send_text(data)
                print("sending...")
            else:
                print("No data received from serial port.")
        except WebSocketDisconnect:
            print("websocket disconnected")
            break
        except KeyboardInterrupt:
            break
        except Exception as e:
            print(f"Unexpected error: {e}")

# while True:
#     try:
#         if mode == 1:
#             print(ser.readline().decode().strip())
#         elif mode == 2:
#             with open("data.txt", "a") as f:
#                 f.write(ser.readline().decode().strip() + "\n")
#         elif mode == 3:
#                 print ("websocket mode")      

#     except KeyboardInterrupt:
#         break

uvicorn.run(app, port=8004)
ser.close()