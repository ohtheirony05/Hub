# Love GrannIE OSINT
Goal:
- Find the movie building name
- Street Address (with street number)
- Subrb name   

Provided with:
- Flag format
- FromGranniE.jpg
- Hint to look for old records and use the street number next door.

## Approach taken to solve:   
### Step 1: Reverse image lookup on jpg   
Using Dipli Checker I was able to find 2 sites both poiting towards the Epping Bridge Project in New South Wales:
- https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjojr3D_8qOAxUDSjABHTMhNPUQn5YKegQIERAB&url=https%3A%2F%2Fwww.transport.nsw.gov.au%2Fsystem%2Ffiles%2Fmedia%2Fdocuments%2F2024%2FEpping-Bridge-Project-Frequently-Asked-Questions-for-concept-design-and-Review-of-Environmental-Factors.pdf&usg=AOvVaw2A4NSjXkMkQ0jRFgd7nvrC&opi=89978449
- https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjojr3D_8qOAxUDSjABHTMhNPUQn5YKegQIEBAB&url=https%3A%2F%2Fcaportal.com.au%2Ftfnsw%2Fepping-bridge-project%2Fabout-the-project&usg=AOvVaw0B2BIO9tbTwPaHH4o_g_yl&opi=89978449

This led me to Epping Bridge located:   
<img width="703" height="391" alt="image" src="https://github.com/user-attachments/assets/1e288964-f55d-4462-8ebf-68264b9670d3" />   
### Step 2: Finding the movie building   
When searching up movie in google maps presented me with unsatisfactory results. Going back to the hint I searched "new south wales torn down theaters epping theater" which provided a link (https://cinematreasures.org/theaters/40752) to an old theatre named The Cambria.   
<img width="646" height="349" alt="image" src="https://github.com/user-attachments/assets/46a56ba9-9a58-4bff-86cb-4a88ed410288" />   

After mix and matching the flag format "DUCTF{BuildingName_StreetAddress_Suburb}" I was getting no positive results until I found https://historyandheritage.cityofparramatta.nsw.gov.au/blog/2017/02/03/cambria-hall-a-hidden-cornerstone-of-epping-history. Apparently, it used to be called "Cambria Hall" and not "The Cambria". Was replaced with a Plus Fitness located on "40 Beecroft Rd, Epping NSW 2121, Australia". With this information, the flag was complete.

**Flag:** DUCTF{CambriaHall_46BeecroftRd_Epping}


