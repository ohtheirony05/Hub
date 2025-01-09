Title: CSRF vulnerability with No Defenses
Rank/Tier: Apprentice
Author: Portswigger Web Security Academy
Link: https://portswigger.net/web-security/learning-paths/csrf/csrf-how-to-construct-a-csrf-attack/csrf/lab-no-defenses

# CSRF vulnerability with no defenses
Provided with the following description:
>This lab's email change functionality is vulnerable to CSRF.
To solve the lab, craft some HTML that uses a CSRF attack to change the viewer's email address and upload it to your exploit server.
You can log in to your own account using the following credentials: **```wiener:peter```**

Main take aways from the provided description:
- Credentials to an account: ```wiener:peter```.
- Email change function is vulnerable to CSRF.
- Goal: Create and store HTML that performs CSRF on a malicious server that can change the viewers email.

# Reconnaissance
Accessing the lab we're presented with a blog. Feel free to look through the various posts. The 2 areas of interest is  ```Go to exploit server``` and ```My account``` to reach the goal of the lab. 
- Rationale for ```Go to exploit server``` is the name. In the description it said to upload HTML to the exploit server.
- Rationale for ```My account``` is in the lab description about the change email functionality being vulnerable to CSRF. Which is most likely located in a page related to my account than a blog post.
![First displayed page](Lab1_Blog_Page.png)

## Exploit server
Clicking on the ```Go to exploit server``` button we're taken to a new web page with the description
> This is your server. You can use the form below to save an exploit, and send it to the victim.
>
> Please note that the victim uses Google Chrome. When you test your exploit against yourself, we recommend using Burp's Browser or Chrome.

URL to exploit server: 
```
https://exploit-0aac00e003b9867880722ace01de0089.exploit-server.net/exploit
```
Protocol:
```
HTTPS
```
File:
```
exploit
```
Head: 
```
HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
```
Body:
```
Hello, world!
```
4 options:
- Store (Save changes to exploit server)
- View exploit (View result of the exploit server impact on the lab website)
- Deliver exploit to victim (Perform the exploit)
- Access Log

![Exploit_Server](Lab1_Exploit_Server_Original.png)
