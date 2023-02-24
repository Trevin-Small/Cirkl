# Cirkl
Cirkl is a desktop IoT device built on [LilyGO's ESP32 T-RGB board](https://www.lilygo.cc/products/t-rgb).

# What is it?
- Text messages and other instant communication methods have become so mainstream that communication isn't special anymore.
- Old school letters have a certain charm to them, and Cirkl aims to revive the feeling you get from opening a sentimental
letter in a 21st-century form factor by giving its users a physical way to view photos, messages, and drawings that others have sent them.
- Further, Cirkl also conveniently displays the time, weather, and calendar reminders right on your desk. Brilliant!

![Cirkl Device](https://github.com/Trevin-Small/Cirkl/blob/main/cirkl_device/images/cirkl_device.jpg)

Currently the firmware of the device is being developed. In the future, however, a web app with a Google Firebase 
backend will be built which Cirkl can communicate with. This will allow Cirkl to complete its purpose of photo sharing.

# For Example:
- Two users have their own Cirkl devices which are connected to the internet.
- User A visits the web app and uploads a photo from their phone/computer.
- User B's Cirkl pings, displaying a notification that it received a new message. User B can now view the image sent to their device.
- The same process can happen in reverse with user B sending a message to user A's Cirkl.

# Features Cirkl will implement:
- Internet time and weather
- Support sharing photos, text, and drawings
- "Gallery" mode - flip through x amount of most recent messages
- Sync on-device reminders with Google Calendar
- Settings app to modify device color scheme and other configurations
- SD Card support (offline message storage, device settings, etc)
- App extensibility (for future feature additions)

# What's it built with?
- LilyGO T-RGB board and touch-screen display
- LVGL
- CAD & 3D Printing