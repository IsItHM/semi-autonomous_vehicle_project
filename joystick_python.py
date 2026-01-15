import pygame
import requests

# ESP32 IP Address
ESP32_IP = "http://192.168.0.206"  # Replace with your ESP32's IP address

# Initialize pygame
pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

def send_command(command):
    url = f"{ESP32_IP}/move?cmd={command}"
    try:
        requests.get(url)
    except Exception as e:
        print("Error:", e)

# Main loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            x_axis = joystick.get_axis(0)  # Horizontal axis
            y_axis = joystick.get_axis(1)  # Vertical axis

            if y_axis < -0.5:
                send_command("forward")
            elif y_axis > 0.5:
                send_command("backward")
            elif x_axis < -0.5:
                send_command("left")
            elif x_axis > 0.5:
                send_command("right")
            else:
                send_command("stop")
