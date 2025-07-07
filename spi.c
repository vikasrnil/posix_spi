#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

#define SPI_DEVICE_PATH "/dev/spidev0.0"

int main() {
    int spi_file;
    uint8_t tx_buffer[50] = "hello world!";
    uint8_t rx_buffer[50];

    if ((spi_file = open(SPI_DEVICE_PATH, O_RDWR)) < 0) {
        perror("Failed to open SPI device");
        return -1;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    if (ioctl(spi_file, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(spi_file);
        return -1;
    }
    if (ioctl(spi_file, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("Failed to set SPI bits per word");
        close(spi_file);
        return -1;
    }

    if(ioctl(spi_file,SPI_IOC_WR_MAX_SPEED,100000) <0){
	perror("failed to set speed");
	close(spi_file);
	return -1;
    }

    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = sizeof(tx_buffer),
        .delay_usecs = 0,
        .speed_hz = 1000000,
        .bits_per_word = 8,
    };

    if (ioctl(spi_file, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        perror("Failed to perform SPI transfer");
        close(spi_file);
        return -1;
    }

    printf("\rtx_buffer: \n %s\n ", tx_buffer);
    printf("\rrx_buffer: \n %s\n ", rx_buffer);

    close(spi_file);

    return 0;
}
