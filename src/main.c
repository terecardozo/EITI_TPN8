/* Copyright 2017, Esteban Volentini - Facet UNT, Fi UNER
 * Copyright 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** @file EITI_TPN° 8
 **
 ** @brief TPN° 8  - FreeRTOS
 **
 ** Ejemplo de un led parpadeando utilizando la capa de abstraccion de
 ** hardware y con sistema operativo FreeRTOS.
 **
 ** | RV | YYYY.MM.DD | Autor       | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  2 | 2017.10.16 | evolentini  | Correción en el formato del archivo     |
 ** |  1 | 2017.09.21 | evolentini  | Version inicial del archivo             |
 **
 ** @defgroup ejemplos Proyectos de ejemplo
 ** @brief Proyectos de ejemplo de la Especialización en Sistemas Embebidos
 ** @{
 */

/* === Inclusiones de cabeceras ============================================ */
#include "FreeRTOS.h"
#include "bsp.h"
#include "task.h"
#include <stdbool.h>

/* === Definicion y Macros ================================================= */

/* === Declaraciones de tipos de datos internos ============================ */

typedef struct parametros_s {
    digital_output_t led;
    uint16_t periodo;
    digital_input_t tecla;   
} * parametros_t;					 						 

/* === Declaraciones de funciones internas ================================= */

/* === Definiciones de variables internas ================================== */

static board_t board;

/* === Definiciones de variables externas ================================== */

/* === Definiciones de funciones internas ================================== */

void Blinking(void * parameters) {
   
      parametros_t parametros = (parametros_t) parameters;
	  
    while (true) {
        DigitalOutputToggle(parametros->led);
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo));
    }
}									   
 
void BlinkingAmarillo(void * parameters) {
	
    parametros_t parametros = (parametros_t) parameters;
	
    TickType_t ultimo_valor;

    ultimo_valor = xTaskGetTickCount();

    while (true) {
		
        DigitalOutputToggle(parametros->led);
        vTaskDelayUntil(&ultimo_valor,pdMS_TO_TICKS(parametros->periodo));
    }
}

/*void Azul(void * parameters){
	
    parametros_t parametros = (parametros_t) parameters;
	
    while (true) {
		
        if(DigitalInputHasActivated(parametros->tecla)){
            DigitalOutputToggle(parametros->led);
        }
		
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo)); // eliminamos rebote del teclado
    }
}

void Teclado(void * parameters){
    parametros_t parametros = (parametros_t) parameters;
    TaskHandle_t tarea;
    eTaskState state;

    tarea = xTaskGetHandle("Rojo");
    state = eTaskGetState(tarea);
    
    while (true) {
        if(DigitalInputHasActivated(parametros->tecla) && state == eSuspended){
            vTaskResume(tarea);
        } else if(DigitalInputHasActivated(parametros->tecla) && state == eReady){
            vTaskSuspend(tarea);
        }
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo)); // // eliminamos rebote del teclado
    }
}
*/
void Teclado(void * parameters){
	
    parametros_t parametros = (parametros_t) parameters;
    TaskHandle_t tarea;
    eTaskState estado;

    tarea = xTaskGetHandle("Rojo");
    estado = eTaskGetState(tarea); /*Returns as an enumerated type the state in which a task existed at the time eTaskGetState() was executed.*/
   
	
		while (true) {
			if(parametros->tecla == board->boton_prueba){
				
				if(DigitalInputHasActivated(parametros->tecla)){
					DigitalOutputToggle(parametros->led);
				}
			}
			if(parametros->tecla ==  board->boton_prender){
				
				if(DigitalInputHasActivated(parametros->tecla) && estado == eSuspended){
					vTaskResume(tarea);
				} else if(DigitalInputHasActivated(parametros->tecla) && estado == eReady){
					vTaskSuspend(tarea);
				}
				
			}	
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo)); // eliminamos rebote del teclado
		}
}
/* === Definiciones de funciones externas ================================== */

/** @brief Función principal del programa
 **
 ** @returns 0 La función nunca debería termina
 **
 ** @remarks En un sistema embebido la función main() nunca debe terminar.
 **          El valor de retorno 0 es para evitar un error en el compilador.
 */
int main(void) {
	
	static struct parametros_s parametros[4];									 
    /* Inicializaciones y configuraciones de dispositivos */
    board = BoardCreate();

    parametros[0].led = board-> led_rojo;
    parametros[0].periodo = 500;

    parametros[1].led = board-> led_verde;
    parametros[1].periodo = 750;
	
	parametros[2].led = board-> led_amarillo;
    parametros[2].periodo = 250; 

    parametros[3].led = board-> led_azul;
    parametros[3].periodo = 250;	
    parametros[3].tecla = board->boton_prueba;

    parametros[4].tecla = board-> boton_prender;
    parametros[4].periodo = 250;
	
    /* Creación de las tareas */
	xTaskCreate(Blinking, "Rojo", configMINIMAL_STACK_SIZE, &parametros[0], tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Blinking, "Verde", configMINIMAL_STACK_SIZE, &parametros[1], tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(BlinkingAmarillo, "Amarillo", configMINIMAL_STACK_SIZE, &parametros[2], tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(Teclado, "Azul", configMINIMAL_STACK_SIZE, &parametros[3], tskIDLE_PRIORITY + 4, NULL);
    xTaskCreate(Teclado, "Teclado", configMINIMAL_STACK_SIZE, &parametros[4], tskIDLE_PRIORITY +4, NULL);
	
    /* Arranque del sistema operativo */
    vTaskStartScheduler();

    /* vTaskStartScheduler solo retorna si se detiene el sistema operativo */
    while (1) {
    };

    /* El valor de retorno es solo para evitar errores en el compilador*/
    return 0;
}
/* === Ciere de documentacion ============================================== */
/** @} Final de la definición del modulo para doxygen */
