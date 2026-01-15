import pygame
import asyncio
import websockets
import json
import sys

# Configuration
ESP32_IP = "192.168.0.227"
WEBSOCKET_URL = f"ws://{ESP32_IP}:81"

class RCCarController:
    def __init__(self):
        pygame.init()
        pygame.joystick.init()
        
        self.setup_joystick()
        
        self.is_autonomous = False
        self.current_mode = "manual"
        self.websocket = None
        
    def setup_joystick(self):
        """Robust joystick setup with error handling"""
        try:
            if pygame.joystick.get_count() == 0:
                print("No joystick detected! Please connect a joystick.")
                sys.exit(1)
            
            self.joystick = pygame.joystick.Joystick(0)
            self.joystick.init()
            print(f"Joystick detected: {self.joystick.get_name()}")
        except Exception as e:
            print(f"Joystick initialization error: {e}")
            sys.exit(1)

    async def connect_websocket(self):
        """Robust WebSocket connection with exponential backoff"""
        max_retries = 2
        for attempt in range(max_retries):
            try:
                self.websocket = await websockets.connect(WEBSOCKET_URL)
                print("WebSocket connected successfully")
                return
            except Exception as e:
                wait_time = 2 ** attempt  # Exponential backoff
                print(f"WebSocket connection attempt {attempt + 1} failed: {e}")
                print(f"Retrying in {wait_time} seconds...")
                await asyncio.sleep(wait_time)
        
        raise ConnectionError("Could not establish WebSocket connection")

    async def send_mode_change(self, mode):
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
                print(f"WebSocket mode change error: {e}")

    async def send_joystick_data(self, x, y):
        """Send normalized joystick data via WebSocket"""
        if self.websocket and self.current_mode == "manual":
            try:
                message = json.dumps({
                    "type": "joystick",
                    "x": x,
                    "y": y
                })
                await self.websocket.send(message)
            except Exception as e:
                print(f"Joystick data send error: {e}")

    async def handle_joystick_input(self):
        """Enhanced joystick input processing with state management"""
        DEADZONE = 0.2
        last_mode_button_state = False
        
        while True:
            pygame.event.pump()
            
            # Joystick axes
            x_axis = self.joystick.get_axis(0)
            y_axis = self.joystick.get_axis(1)
            
            # Apply deadzone
            x_axis = 0 if abs(x_axis) < DEADZONE else x_axis
            y_axis = 0 if abs(y_axis) < DEADZONE else y_axis
            
            # Normalize axis values
            x_normalized = int(x_axis * 100)
            y_normalized = int(y_axis * 100)
            
            # Joystick data transmission
            await self.send_joystick_data(x_normalized, y_normalized)
            
            # Mode toggle with button debounce
            current_mode_button_state = self.joystick.get_button(0)
            if current_mode_button_state and not last_mode_button_state:
                await self.toggle_mode()
            
            last_mode_button_state = current_mode_button_state
            
            await asyncio.sleep(0.05)  # Reduced sleep for responsiveness

    async def toggle_mode(self):
        """Toggle between manual and autonomous modes"""
        self.is_autonomous = not self.is_autonomous
        self.current_mode = "autonomous" if self.is_autonomous else "manual"
        await self.send_mode_change(self.current_mode)

    async def main(self):
        """Robust main control loop with error recovery"""
        try:
            await self.connect_websocket()
            
            tasks = [
                asyncio.create_task(self.handle_joystick_input()),
            ]
            
            await asyncio.gather(*tasks)
        
        except ConnectionError as e:
            print(f"Connection Error: {e}")
        except KeyboardInterrupt:
            print("Controller stopped by user")
        finally:
            if self.websocket:
                await self.websocket.close()
            pygame.quit()

if __name__ == "__main__":
    try:
        controller = RCCarController()
        asyncio.run(controller.main())
    except Exception as e:
        print(f"Unexpected error: {e}")







# import pygame
# import asyncio
# import websockets
# import json
# import sys

# # Configuration
# ESP32_IP = "10.15.17.65"
# WEBSOCKET_URL = f"ws://{ESP32_IP}:81"

# class RCCarController:
#     def __init__(self):
#         pygame.init()
#         pygame.joystick.init()
        
#         self.setup_joystick()
        
#         self.is_autonomous = False
#         self.current_mode = "manual"
#         self.websocket = None
#         self.connection_retries = 0
#         self.MAX_RETRIES = 3
        
#     def setup_joystick(self):
#         try:
#             if pygame.joystick.get_count() == 0:
#                 print("No joystick detected! Please connect a joystick.")
#                 sys.exit(1)
            
#             self.joystick = pygame.joystick.Joystick(0)
#             self.joystick.init()
#             print(f"Detected Joystick: {self.joystick.get_name()}")
#         except Exception as e:
#             print(f"Joystick initialization error: {e}")
#             sys.exit(1)

#     async def connect_websocket(self):
#         while self.connection_retries < self.MAX_RETRIES:
#             try:
#                 self.websocket = await websockets.connect(WEBSOCKET_URL)
#                 print("WebSocket connected successfully")
#                 self.connection_retries = 0
#                 return
#             except Exception as e:
#                 self.connection_retries += 1
#                 wait_time = min(2 ** self.connection_retries, 30)
#                 print(f"Connection attempt {self.connection_retries} failed: {e}")
#                 print(f"Retrying in {wait_time} seconds...")
#                 await asyncio.sleep(wait_time)
        
#         raise ConnectionError("Could not establish WebSocket connection")

#     async def send_mode_change(self, mode):
#         if self.websocket:
#             try:
#                 message = json.dumps({
#                     "type": "mode",
#                     "mode": mode
#                 })
#                 await self.websocket.send(message)
#                 print(f"Mode changed to: {mode}")
#             except Exception as e:
#                 print(f"Mode change error: {e}")

#     async def send_joystick_data(self, x, y):
#         if self.websocket and self.current_mode == "manual":
#             try:
#                 message = json.dumps({
#                     "type": "joystick",
#                     "x": x,
#                     "y": y
#                 })
#                 await self.websocket.send(message)
#             except Exception as e:
#                 print(f"Joystick data send error: {e}")

#     async def handle_joystick_input(self):
#         DEADZONE = 0.15  # Slightly reduced deadzone
#         last_mode_button_state = False
        
#         while True:
#             pygame.event.pump()
            
#             x_axis = self.joystick.get_axis(0)
#             y_axis = self.joystick.get_axis(1)
            
#             x_axis = 0 if abs(x_axis) < DEADZONE else x_axis
#             y_axis = 0 if abs(y_axis) < DEADZONE else y_axis
            
#             x_normalized = int(x_axis * 100)
#             y_normalized = int(y_axis * 100)
            
#             await self.send_joystick_data(x_normalized, y_normalized)
            
#             current_mode_button_state = self.joystick.get_button(0)
#             if current_mode_button_state and not last_mode_button_state:
#                 await self.toggle_mode()
            
#             last_mode_button_state = current_mode_button_state
            
#             await asyncio.sleep(0.05)

#     async def toggle_mode(self):
#         self.is_autonomous = not self.is_autonomous
#         self.current_mode = "autonomous" if self.is_autonomous else "manual"
#         await self.send_mode_change(self.current_mode)

#     async def main(self):
#         try:
#             await self.connect_websocket()
            
#             tasks = [
#                 asyncio.create_task(self.handle_joystick_input()),
#             ]
            
#             await asyncio.gather(*tasks)
        
#         except ConnectionError as e:
#             print(f"Connection Error: {e}")
#         except KeyboardInterrupt:
#             print("Controller stopped by user")
#         finally:
#             if self.websocket:
#                 await self.websocket.close()
#             pygame.quit()

# if __name__ == "__main__":
#     try:
#         controller = RCCarController()
#         asyncio.run(controller.main())
#     except Exception as e:
#         print(f"Unexpected error: {e}")