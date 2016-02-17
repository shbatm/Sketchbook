#ifndef __SETTINGS_H__
#define __SETTINGS_H__

typedef struct {
  char board_name[64];
  char ssid[64];
  char pass[64];
} settings_t __attribute__ ((packed));

settings_t settings
{
    "test_host",
    "ssid",
    "pass"
};

settings_t settings_original;

size_t settings_size = sizeof(settings);

#endif