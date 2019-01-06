/*******************************************************************************
*
*  (C) COPYRIGHT AUTHORS, 2014 - 2018
*
*  TITLE:       BASIC.C
*
*  VERSION:     1.30
*
*  DATE:        14 July 2018
*
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
* TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
*******************************************************************************/
#include "global.h"

/*
* ScanBasicUacData
*
* Purpose:
*
* Query UserSharedData flags, UAC registry values.
*
*/
VOID ScanBasicUacData(
    BASICDATACALLBACK OutputCallback
)
{
    ULONG       Flags = 0;
    LRESULT     lRet;
    HKEY        hKey = NULL;

    UAC_BASIC_DATA Data;

    if (OutputCallback == NULL)
        return;

    if (!NT_SUCCESS(RtlQueryElevationFlags(&Flags)))
        return;

    RtlSecureZeroMemory(&Data, sizeof(Data));

    Data.Name = T_FLAG_ELEVATION_ENABLED;
    Data.IsValueBool = TRUE;
    Data.Value = ((Flags & DBG_FLAG_ELEVATION_ENABLED) > 0);
    OutputCallback(&Data);

    Data.Name = T_FLAG_VIRTUALIZATION_ENABLED;
    Data.IsValueBool = TRUE;
    Data.Value = ((Flags & DBG_FLAG_VIRTUALIZATION_ENABLED) > 0);
    OutputCallback(&Data);

    Data.Name = T_FLAG_INSTALLERDETECT_ENABLED;
    Data.IsValueBool = TRUE;
    Data.Value = ((Flags & DBG_FLAG_INSTALLER_DETECT_ENABLED) > 0);
    OutputCallback(&Data);

    lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, T_UAC_SETTINGS_KEY, 0, KEY_READ, &hKey);
    if (lRet == ERROR_SUCCESS) {

        RtlSecureZeroMemory(&Data, sizeof(Data));
        lRet = supRegReadDword(hKey, T_UAC_PROMPT_BEHAVIOR, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_PROMPT_BEHAVIOR;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_RESTRICTED_AUTOAPPROVE, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_RESTRICTED_AUTOAPPROVE;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_AUTOAPPROVEIC, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_AUTOAPPROVEIC;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_AUTOAPPROVEMP, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_AUTOAPPROVEMP;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_AUTOAPPROVEHARDCLAIMS, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_AUTOAPPROVEHARDCLAIMS;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_ENABLESECUREUIPATHS, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_ENABLESECUREUIPATHS;
            OutputCallback(&Data);
        }

        Data.Value = 0;
        lRet = supRegReadDword(hKey, T_UAC_SECURE_DESKTOP, &Data.Value);
        if (lRet == ERROR_SUCCESS) {
            Data.Name = T_UAC_SECURE_DESKTOP;
            Data.IsValueBool = TRUE;
            OutputCallback(&Data);
        }

        RegCloseKey(hKey);
    }
}
