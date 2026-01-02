#include <common.h>
#include <cli.h>
#include <gl_api.h>
#include <asm/gpio.h>
#include <fdtdec.h>
#include <asm/arch-qca-common/smem.h>
#include <part.h>
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <ubi_uboot.h>

DECLARE_GLOBAL_DATA_PTR;

void led_toggle(const char *gpio_name)
{

	int node, value;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	value = gpio_get_value(gpio);
	value = !value;
	gpio_set_value(gpio, value);
}

void led_on(const char *gpio_name)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	gpio_set_value(gpio, 1);
}

void led_off(const char *gpio_name)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return;
	}

	gpio_set_value(gpio, 0);
}

bool button_is_press(const char *gpio_name, int value)
{
	int node;
	unsigned int gpio;
	node = fdt_path_offset(gd->fdt_blob, gpio_name);
	if (node < 0) {
		printf("Could not find %s node in fdt\n", gpio_name);
		return false;
	}
	gpio = fdtdec_get_uint(gd->fdt_blob, node, "gpio", 0);
	if (gpio < 0) {
		printf("Could not find %s node's gpio in fdt\n", gpio_name);
		return false;
	}

	if(gpio_get_value(gpio) == value)
	{
		mdelay(10);
		if(gpio_get_value(gpio) == value)
			return true;
		else
			return false;
	}
	else
		return false;
}

void check_button_is_press(void)
{
	int counter = 0;
	char *button_name = NULL;

	// 检测哪个按键被按下
    if (button_is_press("reset_key", GL_RESET_BUTTON_IS_PRESS)) {
        button_name = "RESET";
    }
#ifdef HAS_WPS_KEY
	else if (button_is_press("wps_key", GL_WPS_BUTTON_IS_PRESS)) {
        button_name = "WPS";
    }
#endif

	// 如果任一按键被按下
	while (button_name != NULL) {
		// 重新检测按键状态
        int still_pressed = 0;

        if (strcmp(button_name, "RESET") == 0) {
            still_pressed = button_is_press("reset_key", GL_RESET_BUTTON_IS_PRESS);
        }
#ifdef HAS_WPS_KEY
		else if (strcmp(button_name, "WPS") == 0) {
            still_pressed = button_is_press("wps_key", GL_WPS_BUTTON_IS_PRESS);
        }
#endif

        if (!still_pressed) {
            break;  // 按键已释放
        }

		if (counter == 0)
			printf("%s button is pressed for: %2d ", button_name, counter);

		// LED 闪烁
		led_off("power_led");
		mdelay(350);
		led_on("power_led");
		mdelay(350);

		counter++;

		// how long the button is pressed?
		printf("\b\b\b%2d ", counter);

		if(counter >= 5){
			led_off("power_led");
			led_on("blink_led");
			printf("\n");
			run_command("httpd 192.168.1.1", 0);
			cli_loop();
			break;
		}
	}

	if (counter != 0)
		printf("\n");

	return;
}

// 只检查文件的开头几个特殊 Magic Num
int check_fw_type(void *address) {
	u32 *header_magic1 = (u32 *)(address);
	u32 *header_magic2 = (u32 *)(address + 0x4);

	switch (*header_magic1) {
		case HEADER_MAGIC_CDT:
			return FW_TYPE_CDT;
		case HEADER_MAGIC_ELF:
			return FW_TYPE_ELF;
		case HEADER_MAGIC_FIT:
			return FW_TYPE_FIT;
		case HEADER_MAGIC_MBN1:
			if (*header_magic2 == HEADER_MAGIC_MBN2)
				return FW_TYPE_MIBIB;
			return FW_TYPE_UNKNOWN;
		case HEADER_MAGIC_SBL_NAND1:
			if (*header_magic2 == HEADER_MAGIC_SBL_NAND2)
				return FW_TYPE_NAND;
			return FW_TYPE_UNKNOWN;
		case HEADER_MAGIC_UBI:
			return FW_TYPE_UBI;
		default:
			return FW_TYPE_UNKNOWN;
	}
}

void print_fw_type(int fw_type) {
	printf("* The upload file type: ");
	switch (fw_type) {
		case FW_TYPE_CDT:
			printf("CDT *");
			break;
		case FW_TYPE_ELF:
			printf("ELF *");
			break;
		case FW_TYPE_FIT:
			printf("FIT IMAGE *");
			break;
		case FW_TYPE_MIBIB:
			printf("MIBIB *");
			break;
		case FW_TYPE_NAND:
			printf("NAND IMAGE *");
			break;
		case FW_TYPE_UBI:
			printf("UBI FIRMWARE *");
			break;
		case FW_TYPE_UNKNOWN:
		default:
			printf("UNKNOWN *");
	}
	return;
}

int image_greater_than_partition(char *part_name, char *file_name, ulong file_size_in_bytes) {
    uint32_t part_size_in_bytes = 0;
	uint32_t size_block, start_block;
	qca_smem_flash_info_t *sfi = &qca_smem_flash_info;

    if (smem_getpart(part_name, &start_block, &size_block)) {
		printf("\n\nPartition %s not found!", part_name);
        return 1;
	}
	part_size_in_bytes = sfi->flash_block_size * size_block;

    if (file_size_in_bytes > part_size_in_bytes) {
        printf("\n\n* Error: image %s size (%lu bytes) > partition %s size (%lu bytes)! *",
			   file_name, file_size_in_bytes, part_name, (ulong)part_size_in_bytes);
        return 1;
    }

    return 0;
}

int check_fw_compat(const int upgrade_type, const int fw_type, const ulong file_size_in_bytes) {
	switch (upgrade_type) {
		case WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE:
			if (fw_type != FW_TYPE_UBI) {
				printf("\n\n* The upload file is NOT supported FIRMWARE!! *\n\n");
				print_fw_type(fw_type);
				return 1;
			}
			return (image_greater_than_partition("rootfs", "total", (size_t)file_size_in_bytes));
		case WEBFAILSAFE_UPGRADE_TYPE_UBOOT:
			if (fw_type != FW_TYPE_ELF) {
				printf("\n\n* The upload file is NOT supported UBOOT ELF!! *\n\n");
				print_fw_type(fw_type);
				return 1;
			}
			break;
		case WEBFAILSAFE_UPGRADE_TYPE_IMG:
			if (fw_type != FW_TYPE_NAND &&
				fw_type != FW_TYPE_MIBIB
			) {
				printf("\n\n* The upload file is NOT supported NAND IMG!! *\n\n");
				print_fw_type(fw_type);
				return 1;
			} else if ((fw_type == FW_TYPE_MIBIB) && (file_size_in_bytes > WEBFAILSAFE_UPLOAD_MIBIB_SIZE_IN_BYTES_NAND)) {
				printf("\n\n## Error: wrong file size, should be less than or equal to: %d bytes!", WEBFAILSAFE_UPLOAD_MIBIB_SIZE_IN_BYTES_NAND);
				return 1;
			}
			break;
		case WEBFAILSAFE_UPGRADE_TYPE_CDT:
			if (fw_type != FW_TYPE_CDT) {
				printf("\n\n* The upload file is NOT supported CDT!! *\n\n");
				print_fw_type(fw_type);
				return 1;
			}
			break;
		case WEBFAILSAFE_UPGRADE_TYPE_UIMAGE:
			if (fw_type != FW_TYPE_FIT) {
				printf("\n\n* The upload file is NOT supported FIT uImage!! *\n\n");
				print_fw_type(fw_type);
				return 1;
			}
			break;
		case WEBFAILSAFE_UPGRADE_TYPE_ART:
			break;
		default:
			printf("\n\n* NOT supported WEBFAILSAFE UPGRADE TYPE!! *");
			return 1;
	}

	return 0;
}
