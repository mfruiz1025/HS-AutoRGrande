
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       USER                                                      */
/*    Created:      3/7/2025, 6:31:15 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */


#include "vex.h"
#include "configuration.h"
#include "driver.h"
#include "funciones_posicionales.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */


void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */


void autonomous(void) {
  // ..........................................................................
  //PASO 1
  // 1. Recolecta el primer ring y lo mantiene con el de recarga
  moveParabolicV(81, 80, 67, 66);
  // 2. Vuelve a la estaca del color y pone ambos rings
  moveParabolic(19, -95.5, -99);
  // 3. Pone los dos rings en la estaca de color
  recoleccion(100, 2);
  //PASO 2
  // 4. Se acomoda para ir por la estaca
  moveDistance(10, 50);
  rotateOnAxis(36, 98);
  //5. Recoge la estaca 
  moveDistance(38, -93);
  Pinza.open();
  //6. Se acomoda para ir por el ring de arriba
  rotateOnAxis(73, 96);
  //PASO 3
  //Va por los otros dos rings
  RecolectorNeumatica.open();
  moveDistanceN(22, 100);
  // Recolecta la dona azul de arriba 
  moveDistance(5, 50);
  RecolectorNeumatica.close();
  //PASO 4
  //. Recolecta  la dona de abajo
  /*
  moveDistanceN(10, 100);
  rotateOnAxis(25,-100);
  moveDistance(35,100);
  recoleccion(100, 3);
  //PASO 5
  //. Va a la esquina a tumbar la pila de las donas y las recoge
  rotateOnAxis(15, 100);
  moveDistanceB(20, 100);
  //PASO 6
  //. Deja la estaca y se dirige a tocar la escalera
  rotateOnAxis(70, 100);
  Pinza.close(); 
  brazo.close(); 
  moveDistance(50, 100);*/
  // ..........................................................................
}


/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */


void usercontrol(void) {
  // User control code here, inside the loop
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
          Garra.spin(directionType::fwd, 100, velocityUnits::pct);
      } else if (Controller1.ButtonA.pressing()) {
          Garra.spin(directionType::rev, 100, velocityUnits::pct);
      } else {
          Garra.stop(brakeType::hold);
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