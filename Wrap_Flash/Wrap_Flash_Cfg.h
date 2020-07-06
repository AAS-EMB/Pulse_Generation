/*
 * Wrap_Flash_Cfg.h
 *
 *  Created on: 30 окт. 2019 г.
 *      Author: User
 */

#ifndef WRAP_FLASH_CFG_H_
#define WRAP_FLASH_CFG_H_
#ifdef __cplusplus
 extern "C" {
#endif

// Период выходного сигнала каждой дискретной линии в мс
#define DISCRETS_OUT_PERIOD				(1000)

// Коэффициент заполнения каждой дискретной линии в %
#define DISCRETS_OUT_PULSE				(50)

// Поправочный коэффициент
#define DISCRETS_CORRECTION_FACTOR		(1000)

#ifdef __cplusplus
}
#endif
#endif /* WRAP_FLASH_CFG_H_ */
