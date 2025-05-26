**Description:**  
After a relentless week-long effort, a lone Task Force operative successfully breached Volnaya's cloud-based ICS monitoring network, securing us permanent access to their front-end monitoring dashboard and opening a critical window into their industrial infrastructure. Your goal is to infiltrate deeper, locate sensitive data, and enable the Task Force to assume control of their core systems. IP: 3.15.107.79  
**HTB Difficulty Rating:** Very easy  
**How it felt:** WOOOH LIMITLESS PAIN!

# Steps taken
## Initial Recon:
Unfortunately, Shodan.io didn't have any information on this 3.15.107.79, but IP identified it as an Amazon AWS server. Launched community Burp Suite and browsed to 3.15.107.79 where a dashboard was presented. The Dashboard contained multiple blocks of information and an input button (Check Status).
![Visual ICS Monitoring Dashboard](Dashboard.png)  

When inspecting the response in HTTP History of Burp Suite, the button sends a POST request to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/status, leading to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/, revealing /private API.  
![Form](Form.png)   
![Private Endpoint API](Endpoints.png)

Unfortunately, https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private required an authentication token.
## Exploitation
Tinkered with https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private in curl for a bit and eventually got stuck requiring a security token. Resulted in pivoting back to the Dashboard, and was curious to see what would happen if the URL was switched in the form to https://inyunqef0e.execute-api.us-east-2.amazonaws.com/api/private.
