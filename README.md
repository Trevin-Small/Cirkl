# Cirkl
Cirkl is a desktop IoT device built on [LilyGO's ESP32 T-RGB board](https://www.lilygo.cc/products/t-rgb).

# What is it?
Cirkl is a device that allows you to view photos that others have sent you.
It's rather charming to receive a nostalgic picture with your friend, a drawing from your child, or a memory from a spouse.
### So Why not put these meaninful messages on display?
Cirkl's main feature, its gallery function, loops through a slideshow of all the meaningful messages that others have sent to your device.
This allows you to view great memories throughout your day in a non-intrusive manner!
- Further, Cirkl also displays the time and weather for convenient reference at anytime. Brilliant!

![Cirkl Device](https://github.com/Trevin-Small/Cirkl/blob/main/repo_images/cirkl_device.jpg)

At this time, the firmware of the device and the accompanying website are still under construction.

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
