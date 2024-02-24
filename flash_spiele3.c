// momefilo Desing
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
//debug
#include "./ili9341/ili9341.h"
//end debug
#include "./graphics/graphics.h"


uint32_t Klotz_FlashOffset;
uint8_t *Klotz_FlashContent;
uint K0_size = KLOTS_0_LEN / FLASH_PAGE_SIZE +1, K1_size = KLOTS_1_LEN / FLASH_PAGE_SIZE +1;
uint K2_size = KLOTS_2_LEN / FLASH_PAGE_SIZE +1, K3_size = KLOTS_3_LEN / FLASH_PAGE_SIZE +1;
uint K4_size = KLOTS_4_LEN / FLASH_PAGE_SIZE +1, K5_size = KLOTS_5_LEN / FLASH_PAGE_SIZE +1;
uint K6_size = KLOTS_6_LEN / FLASH_PAGE_SIZE +1, K7_size = KLOTS_7_LEN / FLASH_PAGE_SIZE +1;
uint K0_offset, K1_offset, K2_offset, K3_offset, K4_offset, K5_offset, K6_offset, K7_offset;
uint PageSum;
uint SectorSum;

uint8_t *getKlots(uint8_t klotsId){
	if(klotsId == 0) return &Klotz_FlashContent[K0_offset];
	else if(klotsId == 1) return &Klotz_FlashContent[K1_offset];
	else if(klotsId == 2) return &Klotz_FlashContent[K2_offset];
	else if(klotsId == 3) return &Klotz_FlashContent[K3_offset];
	else if(klotsId == 4) return &Klotz_FlashContent[K4_offset];
	else if(klotsId == 5) return &Klotz_FlashContent[K5_offset];
	else if(klotsId == 6) return &Klotz_FlashContent[K6_offset];
	else if(klotsId == 7) return &Klotz_FlashContent[K7_offset];
}

//debug
void paintHeader(){
	uint8_t *fl = getKlots(0);
	int fehlersum1 = 0;
	for(int i=0; i<KLOTS_0_LEN; i++) if(KLOTS_0[i] != fl[i]) fehlersum1++;
	fl = getKlots(1);
	int fehlersum2 = 0;
	for(int i=0; i<KLOTS_1_LEN; i++)if(KLOTS_1[i] != fl[i])fehlersum2++;
	fl = getKlots(2);
	int fehlersum3 = 0;
	for(int i=0; i<KLOTS_2_LEN; i++)if(KLOTS_2[i] != fl[i])fehlersum3++;
	fl = getKlots(3);
	int fehlersum4 = 0;
	for(int i=0; i<KLOTS_3_LEN; i++)if(KLOTS_3[i] != fl[i])fehlersum4++;
	fl = getKlots(4);
	int fehlersum5 = 0;
	for(int i=0; i<KLOTS_4_LEN; i++)if(KLOTS_4[i] != fl[i])fehlersum5++;
	fl = getKlots(5);
	int fehlersum6 = 0;
	for(int i=0; i<KLOTS_5_LEN; i++)if(KLOTS_5[i] != fl[i])fehlersum6++;
	fl = getKlots(6);
	int fehlersum7 = 0;
	for(int i=0; i<KLOTS_6_LEN; i++)if(KLOTS_6[i] != fl[i])fehlersum7++;
	fl = getKlots(7);
	int fehlersum8 = 0;
	for(int i=0; i<KLOTS_7_LEN; i++)if(KLOTS_7[i] != fl[i])fehlersum8++;

	printf("Fehler 1: %d 2: %d, 3: %d 4: %d\n",fehlersum1,fehlersum2,fehlersum3,fehlersum4);
	printf("Fehler 5: %d 6: %d, 7: %d 8: %d\n",fehlersum5,fehlersum6,fehlersum7,fehlersum8);
	uint16_t area[] = {0, 0, 99, 99};
	drawRect(area, KLOTS_6);
	area[0] = 100; area[2] = 199;
	drawRect(area, getKlots(6));
}
//end debug
void flash_Klotski(){
	printf("Writeflash\n");
	uint32_t flags = save_and_disable_interrupts();
	flash_range_erase(Klotz_FlashOffset, (PageSum/16+1) * FLASH_SECTOR_SIZE);
	flash_range_program(Klotz_FlashOffset + K0_offset, KLOTS_0, K0_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K1_offset, KLOTS_1, K1_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K2_offset, KLOTS_2, K2_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K3_offset, KLOTS_3, K3_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K4_offset, KLOTS_4, K4_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K5_offset, KLOTS_5, K5_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K6_offset, KLOTS_6, K6_size * FLASH_PAGE_SIZE);
	flash_range_program(Klotz_FlashOffset + K7_offset, KLOTS_7, K7_size * FLASH_PAGE_SIZE);
	restore_interrupts(flags);
	printf("Writeflash End\n");
}


void klotski_flashInit(){
	K0_offset = 1;
	K1_offset = K0_size * FLASH_PAGE_SIZE+1;
	K2_offset = K1_offset + K1_size * FLASH_PAGE_SIZE;
	K3_offset = K2_offset + K2_size * FLASH_PAGE_SIZE;
	K4_offset = K3_offset + K3_size * FLASH_PAGE_SIZE;
	K5_offset = K4_offset + K4_size * FLASH_PAGE_SIZE;
	K6_offset = K5_offset + K5_size * FLASH_PAGE_SIZE;
	K7_offset = K6_offset + K6_size * FLASH_PAGE_SIZE;

	PageSum = K0_size + K1_size + K2_size + K3_size + K4_size + K5_size + K6_size + K7_size;
	SectorSum = PageSum / 16 + 1;
	SectorSum = (SectorSum / 16 + 1) * 16;
	printf("SectorSum = %d\n", SectorSum);
	Klotz_FlashOffset = (PICO_FLASH_SIZE_BYTES - SectorSum * FLASH_SECTOR_SIZE)-2049;
	Klotz_FlashContent = (uint8_t *) (XIP_BASE + Klotz_FlashOffset);
	flash_Klotski();
}

int main(){
	stdio_init_all();
	sleep_ms(100);

	sleep_ms(100);
	klotski_flashInit();
	for(int i=0; i<8; i++) printf("Klots id: %d, %x\n",i, getKlots(i)[0]);
	//debug
	ili9341_init();
	setOrientation(VERTICAL);
	paintHeader();
	//end debug
}
