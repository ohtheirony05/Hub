//Resources used:
/*

https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlangetprofile - API used to get a specific wireless profile data.
    ^Will need to loop through this to get all the different profiles.
https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlanopenhandle - Used to open a connection to the server. (creates a handle)
https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlanenuminterfaces - Lists all the GUIDs of wireless interfaces that is currently enabled on the local machine.
https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlanclosehandle - Close the handle created by WlanOpenHandle API.
https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlangetprofilelist - Retrieves a list of WLAN profiles in preference order.
https://learn.microsoft.com/en-us/windows/win32/api/wlanapi/nf-wlanapi-wlangetprofile - Retrieves all info about a specific WLAN profile.

Steps:
    1: Open a handle.
    2: Get the list of all GUIDs of WLAN interfaces currently enabled.
    3: Open the file I'm going to be writing to.
    4: Get a list of WLAN profiles.
    5: Loop through each WLAN profile grabbing the name and password.
    6: Clean up resources.
    7: Exit.

*/
#include <windows.h>
#include <wlanapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Wlanapi.lib")
#pragma comment(lib, "Ole32.lib")

#ifndef WLAN_PROFILE_GET_PLAINTEXT_KEY
#define WLAN_PROFILE_GET_PLAINTEXT_KEY 0x4 
//Unless it's needed in hex could probably just do 4 because 4 dec is = 0x4 hex.
#endif

void grabInfo(HANDLE hClientH, const GUID* pInterfaceGuid, PWLAN_PROFILE_INFO pProfileInfo, FILE* fp){
    char profileName[256]; //256 bytes so a length of 256 normal 1 byte characters.
    char password[256] = "N/A"; //If there is no password then put N/A or if we can't find keyMaterial. Buffer size is 256 bytes.
    char profileXml[4096]; //Allocate 4096 bytes of space to the buffer to hold pProfileXml.
    DWORD dwFlags = WLAN_PROFILE_GET_PLAINTEXT_KEY;
    LPWSTR pProfileXml = NULL;

    DWORD dwResult = NULL;
    dwResult = WlanGetProfile(hClientH, pInterfaceGuid, pProfileInfo->strProfileName, NULL, &pProfileXml, &dwFlags, NULL);

    wcstombs(profileXml, pProfileXml, sizeof(profileXml));
    wcstombs(profileName, pProfileInfo->strProfileName, sizeof(profileName));

    char *pskTag = strstr(profileXml, "<keyMaterial>");
    if (pskTag){
        pskTag += strlen("<keyMaterial>");
        char *endTag = strstr(pskTag, "</keyMaterial>");
        if (endTag){
            *endTag = '\0';
            strcpy(password, pskTag);
        }
    }

    fprintf(fp, "%s %s\n", profileName, password);
    if (pProfileXml != NULL){
        WlanFreeMemory(pProfileXml);
    }

}



int main(){
    DWORD CurVer = 2;
    HANDLE hClientH = NULL; //For the handle.
    DWORD dwResult = NULL; //Going to use this variable to check for errors. If it returns anything other than ERROR_SUCCESS it failed.
    PWLAN_INTERFACE_INFO_LIST ppInterfaceList = NULL; //Pointer to an area of memory to store a pointer to receive and return list of wireless LAN interfaces to.
    // ^ Stores the address to the WLAN_INTERFACE_INFO_LIST struct which stores info about the NIC and contains an array of WLAN_INTERFACE_INFO structs that contain interface info.
    
    PWLAN_INTERFACE_INFO ProfileArray = NULL;
    PWLAN_PROFILE_INFO_LIST ppProfileList = NULL;



    dwResult = WlanOpenHandle(CurVer, NULL, &CurVer, &hClientH); //Opening a handle.
    if(dwResult != ERROR_SUCCESS){ //If a error success is not returned something went wrong and will attempt to close handle.
        printf("WlanOpenHandle failed with error: %d\n", dwResult); //Say what went wrong.
        exit(0); //Handle was not created so no point in continuing.
    }


    dwResult = WlanEnumInterfaces(hClientH, NULL, &ppInterfaceList); //Getting a list of GUIDs for wireless interfaces on the local machine.
    if(dwResult != ERROR_SUCCESS){ //If a error success is not returned something went wrong and will attempt to close handle.
        printf("WlanEnumInterfaces failed with error: %d\n", dwResult); //Say what went wrong.
        WlanCloseHandle(hClientH, NULL); //Attempt to close the handle.
            switch(dwResult){
                case ERROR_INVALID_PARAMETER: //When closing the handle an invalid parameter was used.
                    printf("Invalid parameter!");
                    break;
                case ERROR_INVALID_HANDLE:
                    printf("Invalid handle!"); //The handle sent to WlanCloseHandle was not valid.
                    break;
                case ERROR_SUCCESS:
                    printf("Handle closed successfully!"); //A error success was returned and it went well.
                    break;
                default:
                    printf("Something very unexpected happened!"); //An error that was not specified under the API document was returned.
            }
    }

    FILE *fp = fopen("wifi_profiles.txt", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open output file.\n");
        WlanFreeMemory(ppInterfaceList); //Frees memory allocated by native wifi functions. WlanEnumInterface allocated memory for the list of GUIDs for wireless interfaces that were currently enabled on the local machine.
        //^ So if we can't open wifi_profiles.txt then the rest of the code cannot work so we're going to clean up the memory we allocated.
        WlanCloseHandle(hClientH, NULL); //Close the handle.
            if(dwResult != ERROR_SUCCESS){
                printf("WlanCloseHandle failed with error: %d\n", dwResult); //Decided this would be easier than using switch for each one. As long as I know what each error message means.
                exit(0);
            }
        exit(0);
    }
    ProfileArray = (WLAN_INTERFACE_INFO *)&ppInterfaceList-> InterfaceInfo[0];
    dwResult = WlanGetProfileList(hClientH,&ProfileArray->InterfaceGuid, NULL, &ppProfileList);


    int AmountOfProfilesFound = (int)ppProfileList->dwNumberOfItems;
        printf("Amount of profiles found %d\n", AmountOfProfilesFound);

    for(int j = 0; j < AmountOfProfilesFound; j++){
        grabInfo(hClientH,&ProfileArray->InterfaceGuid, &ppProfileList->ProfileInfo[j], fp);
    }

    fclose(fp);
    WlanFreeMemory(ppInterfaceList);
    WlanCloseHandle(hClientH, NULL);
        if(dwResult != ERROR_SUCCESS){
            printf("WlanCloseHandle failed with error: %d\n", dwResult); //Decided this would be easier than using switch for each one. As long as I know what each error message means.

        }
    return 0;
}