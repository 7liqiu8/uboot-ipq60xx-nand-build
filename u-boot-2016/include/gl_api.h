#define GL_RESET_BUTTON_IS_PRESS        0
#define GL_WPS_BUTTON_IS_PRESS          0

#if defined(CONFIG_TARGET_IPQ6018_CMIOT_AX18) || \
    defined(CONFIG_TARGET_IPQ6018_QIHOO_360V6) || \
    defined(CONFIG_TARGET_IPQ6018_ZN_M2)
#define HAS_WPS_KEY 1
#endif

#if defined(CONFIG_TARGET_IPQ6018_CMIOT_AX18) || \
    defined(CONFIG_TARGET_IPQ6018_ZN_M2)
#define ENABLE_256M_RAM_SUPPORT 1
#endif

#define LED_ON 1
#define LED_OFF 0

#define WEBFAILSAFE_PROGRESS_START		0
#define WEBFAILSAFE_PROGRESS_TIMEOUT		1
#define WEBFAILSAFE_PROGRESS_UPLOAD_READY	2
#define WEBFAILSAFE_PROGRESS_UPGRADE_READY	3
#define WEBFAILSAFE_PROGRESS_UPGRADE_FAILED	4

enum {
    WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE,
    WEBFAILSAFE_UPGRADE_TYPE_UBOOT,
    WEBFAILSAFE_UPGRADE_TYPE_ART,
    WEBFAILSAFE_UPGRADE_TYPE_IMG,
    WEBFAILSAFE_UPGRADE_TYPE_CDT,
    WEBFAILSAFE_UPGRADE_TYPE_UIMAGE,
};

#define CONFIG_LOADADDR                                 0x44000000

#if defined(ENABLE_256M_RAM_SUPPORT)
#define WEBFAILSAFE_UPLOAD_RAM_ADDRESS                  0x44000000
#else
#define WEBFAILSAFE_UPLOAD_RAM_ADDRESS                  0x50000000
#endif

#define WEBFAILSAFE_UPLOAD_PADDING_SIZE_IN_BYTES        (2*1024*1024)
#define WEBFAILSAFE_UPLOAD_UBOOT_SIZE_IN_BYTES          (640*1024)
#define WEBFAILSAFE_UPLOAD_ART_SIZE_IN_BYTES            (256*1024)
#define WEBFAILSAFE_UPLOAD_ART_BIG_SIZE_IN_BYTES        (512*1024)
#define WEBFAILSAFE_UPLOAD_CDT_SIZE_IN_BYTES            (256*1024)

#define WEBFAILSAFE_UPLOAD_UBOOT_SIZE_IN_BYTES_NAND     (1536*1024)
#define WEBFAILSAFE_UPLOAD_ART_SIZE_IN_BYTES_NAND       (512*1024)
#define WEBFAILSAFE_UPLOAD_CDT_SIZE_IN_BYTES_NAND       (512*1024)
#define WEBFAILSAFE_UPLOAD_MIBIB_SIZE_IN_BYTES_NAND     (1024*1024)

/*
 * MIBIB for NAND FLASH
 * MBN Header Magic: AC 9F 56 FE 7A 12 7F CD
 * Starting at 0x800 comes the Partition Table.
 * Partition Table Header Magic: AA 73 EE 55 DB BD 5E E3
 */
#define HEADER_MAGIC_MBN1      0xFE569FAC
#define HEADER_MAGIC_MBN2      0xCD7F127A
#define HEADER_MAGIC_TABLE1    0x55EE73AA
#define HEADER_MAGIC_TABLE2    0xE35EBDDB

/*
 * For NAND IMAGE, only check SBL1 Header: D1 DC 4B 84 34 10 D7 73
*/
#define HEADER_MAGIC_SBL_NAND1 0x844BDCD1
#define HEADER_MAGIC_SBL_NAND2 0x73D71034

#define HEADER_MAGIC_CDT       0x00544443
#define HEADER_MAGIC_ELF       0x464C457F
#define HEADER_MAGIC_FIT       0xEDFE0DD0
#define HEADER_MAGIC_UBI       0x23494255

enum {
    FW_TYPE_UNKNOWN = -1,
    FW_TYPE_CDT,
    FW_TYPE_ELF,
    FW_TYPE_FIT,
    FW_TYPE_MIBIB,
    FW_TYPE_NAND,
    FW_TYPE_UBI,
};

int auto_update_by_tftp(void);
int check_fw_type(void *address);
int check_fw_compat(const int upgrade_type, const int fw_type, const ulong file_size_in_bytes);
void print_fw_type(int fw_type);
void led_toggle(const char *gpio_name);
void led_on(const char *gpio_name);
void led_off(const char *gpio_name);
void check_button_is_press(void);
