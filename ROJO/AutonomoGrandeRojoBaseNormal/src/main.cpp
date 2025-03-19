/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       kenneth_busta                                             */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
using namespace vex;

// Librerías propias
#include "configuration.h"
#include "funciones_posicionales.h"
#include "driver.h"

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
    // Paso 1: Comienza con 1 anillo de precarga en la pinza, toma el anillo de en frente con la pinza y pone ambos en la estaca de color
    move(50, 100, 1.0, 0.4, true, true, false, false);
    move(25, 100, 1.0, 1.0, false, false, false, false);
    recoleccion(100, 0.5);

    // Paso 2: Toma la estaca
    Pinza.close();
    move(65, -100, 1.0, 0.4, false, false, false, false);
    Pinza.open();

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (true) {
    // Cambiar el modo de control con el botón A
    if (Controller1.ButtonY.pressing()) {
        switchControlMode();
        while (Controller1.ButtonA.pressing()) {
            // Esperar a que se suelte el botón A
            task::sleep(10);
        }
    }

    // Control del robot basado en el modo seleccionado
    if (controlMode == 0) {
        twoJoysticksControl();
    }else if(controlMode == 1){
        joystickNewControl(); 
    }

    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonL1.pressing()) {
        Recolector.spin(directionType::fwd, 100, velocityUnits::pct);
        Rampa.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (Controller1.ButtonL2.pressing()) {
        Recolector.spin(directionType::rev, 100, velocityUnits::pct);
        Rampa.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
        Recolector.stop(brakeType::hold);
        Rampa.stop(brakeType::hold);
    }

    // Control del motor recolector y rampa usando L1 y L2
    if (Controller1.ButtonX.pressing()) {
        //Garra.spin(directionType::fwd, 100, velocityUnits::pct);
    } else if (Controller1.ButtonA.pressing()) {
        //Garra.spin(directionType::rev, 100, velocityUnits::pct);
    } else {
        //Garra.stop(brakeType::hold);
    }

    if(Controller1.ButtonR2.pressing())
    {
        // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
        while(Controller1.ButtonR2.pressing()) {
            // Espera a que el botón se suelte
        }

        // Cambiamos el estado del pistón
        pistonAbierto = !pistonAbierto;
        
        // Ejecutamos la acción correspondiente
        if(pistonAbierto) {
            Pinza.open();
        } else {
            Pinza.close();
        }
    }

    if(Controller1.ButtonR1.pressing())
    {
        RecolectorNeumatica.open();
    }else{RecolectorNeumatica.close();}

    if(Controller1.ButtonB.pressing())
    {
        // Esperamos a que el botón sea liberado para evitar múltiples activaciones en una sola pulsación
        while(Controller1.ButtonB.pressing()) {
            // Espera a que el botón se suelte
        }

        // Cambiamos el estado del pistón
        pistonAbierto = !pistonAbierto;
        
        // Ejecutamos la acción correspondiente
        if(pistonAbierto) {
            brazo.open();
        } else {
            brazo.close();
        }
    }
  }   


  // Espera para evitar saturar el CPU
  wait(20, msec);
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}