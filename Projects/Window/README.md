# Window projects


## 🛠️Projects dealing specifically with window OS
- [📶 WLAN profile password grabber](WLAN_PROFILE_Grabber.c)
  - Grabs internet passwords and throws them into a file with the format (WLAN profile name) (Password)
  - File name is based on the machine name + _WLAN_PROFILES.txt
  - Does not require admin
  - Tested on windows 10 and 11
  - gcc -o WLAN_GRABBER.exe ./WLAN_PROFILE_Grabber.c -lwlanpi -lole32

