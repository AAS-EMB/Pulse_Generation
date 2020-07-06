# 16-CHANNEL SIGNAL GENERATION

16-канальный генератор сигналов на основе конечных автоматов на МК - STM32F103. Изменяемые параметры: коэффициент заполнения, период сигнала, 
количество импульсов, активация выходов (данные конфиги хранятся в структуре Flash-памяти контроллера, целостность структуры проверяется с помощью CRC32). 
Взаимодействие с устройством происходит через UART с использованием FIFO-буфера по приему и передаче (в качестве терминала используется широкораспространенный - YAT). 
Для парсинга команд необходимо использовать EOL (End-Of-Line) sequence - LF. 
Данный проект разрабатывался для тестирования дискретных входов устройств, но также может быть использован в качестве генератора сигналов. 

16-channel signal generator based on finite state machines on MCU - STM32F103. Changeable parameters: fill factor, signal period, 
quantity impulse, activation of outputs (configs are stored in the controller's Flash memory structure, and the integrity of the structure is checked using CRC32). 
Interaction with the device occurs via UART using a FIFO buffer for receiving and transmitting (the widely distributed - YAT is used as a terminal). 
To parse commands, use EOL (End-of-Line) sequence - LF. 
This project was developed for testing digital inputs of devices, but can also be used as a signal generator. 

# LICENSE

Все исходные материалы для проекта распространяются по лицензии MIT. Вы можете использовать проект в любом виде,
в том числе и для коммерческой деятельности, но стоит помнить, что автор проекта не дает никаких гарантий на
работоспособность устройства или частей проекта, а так же не несет никакой ответственности по искам или
за нанесенный ущерб.

All source materials for the project are distributed under the MIT license. You can use the project in any form,
including for commercial activities, but it is worth remembering that the author of the project does not give any guarantees for
the operation of the device or parts of the project, and does not bear any liability for claims or
for the damage done.
