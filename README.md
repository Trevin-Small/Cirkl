# Cirkl
Cirkl is a desktop IoT device built on [LilyGO's ESP32 T-RGB board](https://www.lilygo.cc/products/t-rgb).

# What is it?
- Text messages and other instant communication methods have become so mainstream that messages aren't special anymore.
- Old-school letters have a certain charm to them that phone calls and text messages just can't replicate.
- Cirkl aims to revive the feeling you get from opening a sentimental letter in a 21st-century form factor. Cirkl provides its users a physical way to view photos, messages, and drawings that others have sent them, and is intended to be a lower volume, higher "meaning" form of communication.
- Further, Cirkl also conveniently displays the time, weather, and calendar reminders right on your desk. Brilliant!

![Cirkl Device](https://github.com/Trevin-Small/Cirkl/blob/main/repo_images/cirkl_device.jpg)

Currently, the both the firmware of the device and the accompanying website are still under development.

# For Example:
- Two users have their own Cirkl devices which are connected to the internet.
- User A visits the web app and uploads a photo from their phone/computer.
- User B's Cirkl pings, displaying a notification that it received a new message. User B can now view the image sent to their device.
- The same process can happen in reverse with user B sending a message to user A's Cirkl.

# Features already implemented:
- Internet time and weather
- Simple App extensibility (system handles app routing, open, & close), so apps are simple to develop

# Features to be implemented:
- SD Card support (offline message storage, device settings, etc)
- Settings app to modify device color scheme and other configurations
- Support sharing photos, text, and drawings
- "Gallery" mode - flip through x amount of most recent messages
- Fetch and display your reminders from Google Calendar

# What's it built with?
- LilyGO T-RGB board and touch-screen display
- LVGL
- CAD & 3D Printing
