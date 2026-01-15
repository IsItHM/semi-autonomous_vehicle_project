import pygame
import requests
import websockets
import asyncio
import json

# Configuration
ESP32_IP = "http://192.168.0.191"
WEBSOCKET_URL = "ws://192.168.0.191:81"
CAMERA_URL = "http://192.168.0.194:8080/video"

class RCCarController:
    def __init__(self):
        pygame.init()
        pygame.joystick.init()
        
        if pygame.joystick.get_count() == 0:
            print("No joystick detected!")
            raise Exception("No joystick connected")
        
        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()
        
        self.is_autonomous = False
        self.current_mode = "manual"
        self.websocket = None

    def send_http_command(self, command):
        """Send movement command to ESP32 via HTTP"""
        url = f"{ESP32_IP}/move?cmd={command}"
        try:
            response = requests.get(url, timeout=0.5)
            print(f"Sent command: {command}, Response: {response.text}")
        except Exception as e:
            print(f"HTTP Command Error: {e}")

    async def connect_websocket(self):
        """Enhanced WebSocket connection with retry"""
        max_retries = 3
        for attempt in range(max_retries):
            try:
                self.websocket = await websockets.connect(WEBSOCKET_URL)
                print("WebSocket connected successfully")
                return
            except Exception as e:
                print(f"WebSocket connection attempt {attempt + 1} failed: {e}")
                await asyncio.sleep(2)
        
        raise ConnectionError("Could not establish WebSocket connection")

    async def send_websocket_mode(self, mode):
        """Send mode change via WebSocket"""
        if self.websocket:
            try:
                message = json.dumps({
                    "type": "mode",
                    "mode": mode
                })
                await self.websocket.send(message)
                print(f"Mode changed to: {mode}")
            except Exception as e:
                print(f"WebSocket send error: {e}")

    async def send_joystick_data(self, x, y):
        """Send normalized joystick data via WebSocket"""
        if self.websocket:
            try:
                message = json.dumps({
                    "type": "joystick",
                    "x": x,
                    "y": y
                })
                await self.websocket.send(message)
            except Exception as e:
                print(f"Joystick data send error: {e}")

    async def receive_websocket_data(self):
        """Listen for WebSocket messages"""
        while True:
            try:
                message = await self.websocket.recv()
                data = json.loads(message)
                
                if data.get('type') == 'distance':
                    print(f"Obstacle Distance: {data['value']} cm")
            except websockets.exceptions.ConnectionClosed:
                print("WebSocket connection closed")
                break
            except Exception as e:
                print(f"WebSocket receive error: {e}")
            
            await asyncio.sleep(0.1)

    async def handle_joystick_input(self):
        """Enhanced joystick input processing"""
        DEADZONE = 0.2
        
        while True:
            pygame.event.pump()
            
            x_axis = self.joystick.get_axis(0)
            y_axis = self.joystick.get_axis(1)
            
            # Apply deadzone
            x_axis = 0 if abs(x_axis) < DEADZONE else x_axis
            y_axis = 0 if abs(y_axis) < DEADZONE else y_axis
            
            # Normalize axis values
            x_normalized = int(x_axis * 100)
            y_normalized = int(y_axis * 100)
            
            if self.current_mode == "manual":
                await self.send_joystick_data(x_normalized, y_normalized)
            
            # Mode toggle logic
            if self.joystick.get_button(0):
                await self.toggle_mode()
            
            await asyncio.sleep(0.1)

    async def toggle_mode(self):
        """Toggle between manual and autonomous modes"""
        self.is_autonomous = not self.is_autonomous
        self.current_mode = "autonomous" if self.is_autonomous else "manual"
        await self.send_websocket_mode(self.current_mode)

    async def main(self):
        """Enhanced main control loop"""
        await self.connect_websocket()
        
        tasks = [
            asyncio.create_task(self.handle_joystick_input()),
            asyncio.create_task(self.receive_websocket_data())
        ]
        
        await asyncio.gather(*tasks)

if __name__ == "__main__":
    try:
        controller = RCCarController()
        asyncio.run(controller.main())
    except Exception as e:
        print(f"RC Car Controller Error: {e}")
    finally:
        pygame.quit()