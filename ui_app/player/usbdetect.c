#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/bitypes.h>
#include "usbdetect.h"


#define USB_PARTTITION_CHECK                "/proc/partitions"
#define USB_MOUNT_CHECK                     "/proc/mounts"
#define USB_FAT32_FORMAT                    "vfat"
#define USB_NTFS_FORMAT                     "ntfs"
#define USB_MOUNT_DEFAULT_DIR               "/tmp/udisk"

#define MAX_LINE_LEN        512
#define MAX_DEV_NAMELEN     256

static char g_line[MAX_LINE_LEN];
static char g_devName[MAX_DEV_NAMELEN];
static char g_mountName[MAX_DEV_NAMELEN];

static char *freadline(FILE *stream)
{
    int count = 0;

    while(!feof(stream) && (count < MAX_LINE_LEN) && ((g_line[count++] = getc(stream)) != '\n'));
    if (!count)
        return NULL;

    g_line[count - 1] = '\0';

    return g_line;
}

static MI_S32 DetectUsbDev(void)
{
    FILE *pFile = fopen(USB_PARTTITION_CHECK, "r");
    char *pCurLine = NULL;
    char *pSeek = NULL;

    memset(g_devName, 0, sizeof(g_devName));

    if (pFile)
    {
        while((pCurLine = freadline(pFile)) != NULL)
        {
            pSeek = strstr(pCurLine, "sd");
            if (pSeek)
            {
                if ((pSeek[2] >= 'a' && pSeek[2] <= 'z'))
                {
                    if (pSeek[3] == ' ' || pSeek[3] == 0)
                    {
                        memcpy(g_devName, pSeek, 3);
                        fclose(pFile);
                        pFile = NULL;

                        return MI_SUCCESS;
                    }
                    else if (pSeek[3] >= '1' && pSeek[3] <= '9')
                    {
                        memcpy(g_devName, pSeek, 4);
                        fclose(pFile);
                        pFile = NULL;

                        return MI_SUCCESS;
                    }

                }
            }
        }

        fclose(pFile);
        pFile = NULL;
    }

    printf("Can't find usb device\n");
    return -1;
}

static MI_S32 CheckUsbDevMountStatus()
{
    FILE *pFile = fopen(USB_MOUNT_CHECK, "r");
    char *pCurLine = NULL;
    char *pSeek = NULL;

    memset(g_mountName, 0, sizeof(g_mountName));

    if (pFile)
    {
        while((pCurLine = freadline(pFile)) != NULL)
        {
            pSeek = strstr(pCurLine, g_devName);
            if (pSeek)
            {
                char *pMount = NULL;
                pSeek += strlen(g_devName);
                while(*(pSeek) == ' ')
                    pSeek++;

                if (pSeek)
                {
                    pMount = pSeek;
                    while (*pSeek != ' ')
                    {
                        pSeek++;
                    }

                    memcpy(g_mountName, pMount, (pSeek - pMount));
                    printf("/dev/%s has been mounted on %s\n", g_devName, g_mountName);
                    fclose(pFile);
                    pFile = NULL;

                    return MI_SUCCESS;
                }
            }
        }

        fclose(pFile);
        pFile = NULL;
    }
    else
    {
        printf("open %s failed\n", USB_MOUNT_CHECK);
    }

    return -1;
}

static MI_S32 AutoMountUsbDev()
{
    if (CheckUsbDevMountStatus())
    {
        char dirCmd[64] = {0};
        char mountCmd[128] = {0};
        int nRet = 0;

        sprintf(dirCmd, "mkdir -p %s", USB_MOUNT_DEFAULT_DIR);
        nRet = system(dirCmd);
        if (nRet)
        {
            printf("%s execute failed\n", dirCmd);
            return -1;
        }

        sprintf(mountCmd, "mount -t %s /dev/%s %s", USB_FAT32_FORMAT, g_devName, USB_MOUNT_DEFAULT_DIR);
        nRet = system(mountCmd);
        if (!nRet)
        {
            printf("mount /dev/%s on %s success, usb format is FAT32.\n", g_devName, USB_MOUNT_DEFAULT_DIR);
            memset(g_mountName, 0, sizeof(g_mountName));
            memcpy(g_mountName, USB_MOUNT_DEFAULT_DIR, strlen(USB_MOUNT_DEFAULT_DIR));
            return MI_SUCCESS;
        }

        memset(mountCmd, 0, sizeof(mountCmd));
        sprintf(mountCmd, "mount -t %s /dev/%s %s", USB_NTFS_FORMAT, g_devName, USB_MOUNT_DEFAULT_DIR);
        nRet = system(mountCmd);
        if (!nRet)
        {
            printf("mount /dev/%s on %s success, usb format is NTFS.\n", g_devName, USB_MOUNT_DEFAULT_DIR);
            memset(g_mountName, 0, sizeof(g_mountName));
            memcpy(g_mountName, USB_MOUNT_DEFAULT_DIR, strlen(USB_MOUNT_DEFAULT_DIR));
            return MI_SUCCESS;
        }

        printf("mount /dev/%s on %s failed\n", g_devName, USB_MOUNT_DEFAULT_DIR);
        return -1;
    }

    return MI_SUCCESS;
}

static MI_S32 AutoUmountUsbDev()
{
    if (!CheckUsbDevMountStatus())
    {
        char umountCmd[128] = {0};
        int nRet = 0;

        sprintf(umountCmd, "umount /dev/%s", g_devName);
        nRet = system(umountCmd);
        if (nRet)
        {
            printf("umount /dev/%s failed\n", g_devName);
            return -1;
        }
    }

    printf("umount /dev/%s success\n", g_devName);
    return MI_SUCCESS;
}

MI_S32 ST_InitUsbDev(char *pDirName, int nLen)
{
    MI_S32 s32Ret = -1;
    s32Ret = DetectUsbDev();
    if (s32Ret)
        goto exit;

    s32Ret = AutoMountUsbDev();
    if (s32Ret)
        goto exit;

    strncpy(pDirName, g_mountName, nLen);

exit:
    return s32Ret;
}

MI_S32 ST_DeinitUsbDev()
{
    return AutoUmountUsbDev();
}
