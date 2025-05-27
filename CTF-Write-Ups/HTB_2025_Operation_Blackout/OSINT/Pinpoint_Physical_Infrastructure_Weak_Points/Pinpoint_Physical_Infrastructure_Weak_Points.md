# Presupplied challenge information.
**Description:**   
With the shell company, financial hub, and key personnel identified, our focus shifts to locating physical infrastructure vulnerabilities. Intelligence suggests "Facility Alpha" houses critical Operation Blackout components with exploitable security weaknesses. Your task is to analyze facility documentation, satellite imagery, and personnel information to identify a specific security vulnerability that could enable access to the facility. For more information about the mission, please download the briefing. Submit findings in the format: HTB{LOCATION_VULNERABILITY_TYPE} Example: HTB{NORTH_GATE_CAMERA_BLINDSPOT} Note: The flag uses only uppercase letters, numbers, and underscores.   

**HTB Rated Difficulty:** Medium   
**How it felt:** Pretty straightforward.
**Flag format:** HTB{LOCATION_VULNERABILITY_TYPE}   
**Example flag:** HTB{NORTH_GATE_CAMERA_BLINDSPOT}   

**Docker IPs provided:**
```
94.237.51.19:50243 -> HTB City Planning Archives
94.237.51.19:52980 -> HTB GeoSurveillance Portal
94.237.51.19:34705 -> HTB LocalNews Archive
94.237.51.19:47108 -> HTB IndustryCareers Network
```
# LocalNews Archive
Many articles about the new data center (Facility Alpha) and each mention "Service Entrance B"

#GeoSurveillance Portal
There were constantly changing security alerts, but at the bottom was the "Camera Coverage Report," which put Service Entrance B at a high risk due to blindspots in camera coverage.

#🏁 Flag 🏁
HTB{SERVICE_ENTRANCE_B_CAMERA_BLINDSPOT}
