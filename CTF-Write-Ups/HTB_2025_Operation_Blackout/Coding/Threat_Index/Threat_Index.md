Description:   
Volnayan APTs are exfiltrating data through TOR nodes, embedding attack signals in plain sight. Your job is to scan each outbound stream and identify known malicious keywords linked to Operation Blackout. Each keyword has a threat level — the more hits you find, the higher the danger. Analyze the stream, tally the signals, and calculate the overall threat score.

HTB Difficulty Rating: Very Easy
How I found it: Very Easy

# Threat Index outline:
## Weight table
KEYWORD      -> WEIGHT   
"scan"       -> 1   
"response"   -> 2   
"control"    -> 3   
"callback"   -> 4   
"implant"    -> 5   
"zombie"     -> 6   
"trigger"    -> 7   
"infected"   -> 8   
"compromise" -> 9   
"inject"     -> 10   
"execute"    -> 11   
"deploy"     -> 12   
"malware"    -> 13   
"exploit"    -> 14   
"payload"    -> 15  
"backdoor"   -> 16   
"zeroday"    -> 17   
"botnet"     -> 18   
## Example:
```
Input:
payloadrandompayloadhtbzerodayrandombytesmalware

Expected output:
60

Analyzing the data stream:
payloadrandompayloadhtbzerodayrandombytesmalware
^^^^^^^      ^^^^^^^   ^^^^^^^           ^^^^^^^
  15           15        17                13

Calculating the threat score:

threat score = 2 * 15 + 17 + 13 = 60
```
## Solution code:
```
# take in the number
n = input()

# calculate answer
score = 0
score += n.count("scan") * 1
score += n.count("response") * 2
score += n.count("control") * 3
score += n.count("callback") * 4
score += n.count("implant") * 5
score += n.count("zombie") * 6
score += n.count("trigger") * 7
score += n.count("infected") * 8
score += n.count("compromise") * 9
score += n.count("inject") * 10
score += n.count("execute") * 11
score += n.count("deploy") * 12
score += n.count("malware") * 13
score += n.count("exploit") * 14
score += n.count("payload") * 15
score += n.count("backdoor") * 16
score += n.count("zeroday") * 17
score += n.count("botnet") * 18
n = score

# print answer
print(n)
```
# 🏁 Flag 🏁   
HTB{thr34t_L3v3L_m1dn1ght_6f7ab25d6116e5d42c8958f27cabd5a8}

