/* 
 * File:   sensor.h
 * Author: peces
 *
 * Created on September 20, 2018, 7:37 PM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    
unsigned int LeerLuxes(unsigned int canal);
    
unsigned int InterpolarSensor(unsigned int valor_adc);

unsigned int LeerADC(unsigned int canal);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */
