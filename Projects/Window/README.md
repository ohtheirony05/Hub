# Window projects


## 🛠️Projects dealing specifically with window OS
- [📶 WLAN profile password grabber](WLAN_PROFILE_Grabber.c)
  - Grabs internet passwords and throws them into a file with the format (WLAN profile name) (Password)
  - Does not require admin
  - Tested on windows 10 and 11
  - gcc -o WLAN_GRABBER.exe ./WLAN_PROFILE_Grabber.c -lwlanpi -lole32
  - Right now only creates or overwrites the file wifi_profiles.txt. In the future will change it to create a file name based on the computer name.

