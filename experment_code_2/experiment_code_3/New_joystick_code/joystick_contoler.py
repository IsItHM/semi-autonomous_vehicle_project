import pygame
import bluetooth

class BluetoothRCController:
    def __init__(self):
        # Initialize Pygame and Joystick
        pygame.init()
        pygame.joystick.init()
        
        # Bluetooth socket setup
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        
        # ESP32 Bluetooth MAC Address (replace with your device's MAC)
        self.target_address = "XX:XX:XX:XX:XX:XX"
        
        # Port (typically 1 for RFCOMM)
        self.port = 1
        
        # Initialize joystick
        self.init_joystick()
        
        # Connect to Bluetooth
        self.connect_bluetooth()

    def init_joystick(self):
        if pygame.joystick.get_count() > 0:
            self.joystick = pygame.joystick.Joystick(0)
            self.joystick.init()
            print("Joystick Initialized")
        else:
            print("No Joystick Detected")

    def connect_bluetooth(self):
        try:
            print(f"Connecting to {self.target_address}")
            self.sock.connect((self.target_address, self.port))
            print("Bluetooth Connected Successfully")
        except Exception as e:
            print(f"Bluetooth Connection Error: {e}")

    def run(self):
        try:
            running = True
            while running:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False

                # Get joystick axes
                x_axis = self.joystick.get_axis(0)  # Left/Right
                y_axis = self.joystick.get_axis(1)  # Forward/Backward

                # Determine command based on joystick position
                if abs(y_axis) > 0.2:
                    # Vertical movement
                    command = 'F' if y_axis < -0.2 else 'B' if y_axis > 0.2 else 'S'
                elif abs(x_axis) > 0.2:
                    # Horizontal movement
                    command = 'L' if x_axis < -0.2 else 'R' if x_axis > 0.2 else 'S'
                else:
                    command = 'S'

                # Send command via Bluetooth
                self.send_command(command)

                # Small delay to prevent overwhelming
                pygame.time.delay(50)

        except Exception as e:
            print(f"Error: {e}")
        finally:
            self.sock.close()
            pygame.quit()

    def send_command(self, cmd):
        try:
            self.sock.send(cmd.encode())
            print(f"Sent Command: {cmd}")
        except Exception as e:
            print(f"Send Command Error: {e}")

if __name__ == "__main__":
    controller = BluetoothRCController()
    controller.run()