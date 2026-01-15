import pygame
import requests
import websockets
import asyncio
import json

# Configuration - UPDATE THESE
ESP32_IP = "http://192.168.0.191"  # Your ESP32's IP address
WEBSOCKET_URL = "ws://192.168.0.191:81"  # WebSocket server URL
CAMERA_URL = "http://192.168.0.194:8080/video"  # IP Camera stream URL

class RCCarController:
    def __init__(self):
        # Initialize Pygame for Joystick
        pygame.init()
        pygame.joystick.init()
        
        # Check if joystick is connected
        if pygame.joystick.get_count() == 0:
            print("No joystick detected!")
            raise Exception("No joystick connected")
        
        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()
        
        # State variables
        self.is_autonomous = False
        self.current_mode = "manual"
        
        # Websocket connection
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
        """Establish WebSocket connection"""
        try:
            self.websocket = await websockets.connect(WEBSOCKET_URL)
            print("WebSocket connected successfully")
        except Exception as e:
            print(f"WebSocket connection error: {e}")

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

    async def handle_joystick_input(self):
        """Process joystick inputs and send commands"""
        while True:
            pygame.event.pump()  # Process pygame events
            
            x_axis = self.joystick.get_axis(0)  # Horizontal axis
            y_axis = self.joystick.get_axis(1)  # Vertical axis

            # Mode toggle with button 0 (typically A button)
            if self.joystick.get_button(0):
                await self.toggle_mode()

            if self.current_mode == "manual":
                # Threshold for joystick movement
                if y_axis < -0.5:
                    self.send_http_command("forward")
                elif y_axis > 0.5:
                    self.send_http_command("backward")
                elif x_axis < -0.5:
                    self.send_http_command("left")
                elif x_axis > 0.5:
                    self.send_http_command("right")
                else:
                    self.send_http_command("stop")

            await asyncio.sleep(0.1)

    async def toggle_mode(self):
        """Toggle between manual and autonomous modes"""
        self.is_autonomous = not self.is_autonomous
        self.current_mode = "autonomous" if self.is_autonomous else "manual"
        await self.send_websocket_mode(self.current_mode)

    async def main(self):
        """Main control loop"""
        await self.connect_websocket()
        
        # Setup tasks
        joystick_task = asyncio.create_task(self.handle_joystick_input())
        
        # Wait for tasks to complete
        await asyncio.gather(joystick_task)

if __name__ == "__main__":
    try:
        controller = RCCarController()
        asyncio.run(controller.main())
    except Exception as e:
        print(f"RC Car Controller Error: {e}")
    finally:
        pygame.quit()
# import pygame
# import requests
# import websockets
# import asyncio
# import json

# # Configuration
# ESP32_IP = "http://192.168.0.238"  # Replace with your ESP32's IP address
# WEBSOCKET_URL = "ws://192.168.0.238:81"  # WebSocket server URL
# CAMERA_URL = "http://192.168.0.194:8080/video"  # IP Camera stream URL

# class RCCarController:
#     def __init__(self):
#         # Initialize Pygame for Joystick
#         pygame.init()
#         pygame.joystick.init()
        
#         # Check if joystick is connected
#         if pygame.joystick.get_count() == 0:
#             print("No joystick detected!")
#             raise Exception("No joystick connected")
        
#         self.joystick = pygame.joystick.Joystick(0)
#         self.joystick.init()
        
#         # State variables
#         self.is_autonomous = False
#         self.current_mode = "manual"
        
#         # Websocket connection
#         self.websocket = None

#     def send_http_command(self, command):
#         """Send movement command to ESP32 via HTTP"""
#         url = f"{ESP32_IP}/move?cmd={command}"
#         try:
#             requests.get(url, timeout=0.1)
#         except Exception as e:
#             print(f"HTTP Command Error: {e}")

#     async def connect_websocket(self):
#         """Establish WebSocket connection"""
#         try:
#             self.websocket = await websockets.connect(WEBSOCKET_URL)
#             print("WebSocket connected successfully")
#         except Exception as e:
#             print(f"WebSocket connection error: {e}")

#     async def send_websocket_mode(self, mode):
#         """Send mode change via WebSocket"""
#         if self.websocket:
#             try:
#                 message = json.dumps({
#                     "type": "mode",
#                     "mode": mode
#                 })
#                 await self.websocket.send(message)
#             except Exception as e:
#                 print(f"WebSocket send error: {e}")

#     async def handle_joystick_input(self):
#         """Process joystick inputs and send commands"""
#         while True:
#             for event in pygame.event.get():
#                 x_axis = self.joystick.get_axis(0)  # Horizontal axis
#                 y_axis = self.joystick.get_axis(1)  # Vertical axis

#                 if self.current_mode == "manual":
#                     if y_axis < -0.5:
#                         self.send_http_command("forward")
#                     elif y_axis > 0.5:
#                         self.send_http_command("backward")
#                     elif x_axis < -0.5:
#                         self.send_http_command("left")
#                     elif x_axis > 0.5:
#                         self.send_http_command("right")
#                     else:
#                         self.send_http_command("stop")

#             await asyncio.sleep(0.1)

#     async def toggle_mode(self):
#         """Toggle between manual and autonomous modes"""
#         self.is_autonomous = not self.is_autonomous
#         self.current_mode = "autonomous" if self.is_autonomous else "manual"
#         await self.send_websocket_mode(self.current_mode)

#     async def main(self):
#         """Main control loop"""
#         await self.connect_websocket()
        
#         # Setup tasks
#         joystick_task = asyncio.create_task(self.handle_joystick_input())
        
#         # Wait for tasks to complete
#         await asyncio.gather(joystick_task)

# if __name__ == "__main__":
#     try:
#         controller = RCCarController()
#         asyncio.run(controller.main())
#     except Exception as e:
#         print(f"RC Car Controller Error: {e}")
#     finally:
#         pygame.quit()