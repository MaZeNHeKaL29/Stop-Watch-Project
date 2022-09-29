/*
 ============================================================================
 Name        : C_Project.c
 Author      : Mazen Ehab Mohamed Maher Osman Hekal
 Version     :
 Copyright   : Your copyright notice
 Description : C Project
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <typedef.h>

void char_lowercase(uint8* input); //function to change input to lowercase
void sensor_set_menu(uint8* input); //function to display sensor set menu and waits answer from user
void traffic_light_color(uint8* input); //wait for user to set traffic light color
void room_temperature_data(void); //wait for user to input room temperature data
void engine_temp_data(void); //wait for user to input engine temperature data
void check_vehicle_speed_30(void); //check if vehicle speed is 30 km/hr.
void display_vehicle_state(void); //function to display current vehicle state


#define OFF 0
#define ON  1
#define WITH_ENGINE_TEMP_CONTROLLER 1 /*(Bonus Requirement) related to Engine
Temperature Controller */

//a structure for the states of vehicle
typedef struct{
	bool engine_state; //Engine State
	bool ac;  //AC
	uint16 vehicle_speed; //Vehicle Speed
	float32 room_temp; //Room Temperature
#if WITH_ENGINE_TEMP_CONTROLLER == 1
	bool engine_temp_state;  //Engine Temperature Controller State
	float32 engine_temp;	//Engine Temperature
#endif
}VEHICLE;

VEHICLE vehicle; //a global variable structure for vehicle

int main(void) {
	uint8 input; //a variable to wait an input for user

	/*Initial values for vehicle state*/
	vehicle.vehicle_speed = 0;
	vehicle.room_temp = 35;
#if WITH_ENGINE_TEMP_CONTROLLER == 1
	vehicle.engine_temp = 90;
#endif

	while(1){
		printf("a. Turn on the vehicle engine\n");
		printf("b. Turn off the vehicle engine\n");
		printf("c. Quit the System");
		printf("\n\n");
		fflush( stdout );
		scanf(" %c", &input);
		char_lowercase(&input);
		switch (input){
		case 'a':
			printf("Turn on the vehicle engine\n\n");
			sensor_set_menu(&input);
			break;
		case 'b':
			printf("Turn off the vehicle engine\n\n");
			break;
		case 'c':
			printf("Quit the System\n\n");
			return 0;
/*if user enters an invalid input keeps asking for input until
a valid input is entered
*/
		default:
			printf("Invalid Input\n\n");
			break;
		}
	}
}

/*function to change input to lowercase*/
void char_lowercase(uint8* input){
	if (*input >= 'A' && *input <= 'Z'){
		*input = *input + 'a' - 'A';
	}
}


/*function to display sensor set menu and waits answer from user*/
void sensor_set_menu(uint8* input){
	vehicle.engine_state = ON;
	while(1){
		printf("a. Turn off the engine\n");
		printf("b. Set the traffic light color\n");
		printf("c. Set the room Temperature\n");
#if WITH_ENGINE_TEMP_CONTROLLER == 1
		printf("d. Set the engine Temperature\n");
#endif
		printf("\n\n");
		fflush( stdout );
		scanf(" %c", input);
		char_lowercase(input);
		switch (*input){
		case 'a':
			printf("Turn off the engine\n\n");
			return;
		case 'b':
			traffic_light_color(input);
			printf("\n\n");
			break;
		case 'c':
			room_temperature_data();
			printf("\n\n");
			break;
#if WITH_ENGINE_TEMP_CONTROLLER == 1
		case 'd':
			engine_temp_data();
			printf("\n\n");
			break;
#endif
/*if user enters an invalid input keeps asking for input until
a valid input is entered
*/
		default:
			printf("Invalid Input\n\n");
			break;
		}
	}
}


/*wait for user to set traffic light color*/
void traffic_light_color(uint8* input){
	bool invalid = TRUE;
	while(invalid){
		printf("Set the traffic light color : ");
		fflush( stdout );
		scanf(" %c", input);
		char_lowercase(input);
		switch(*input){
		case 'g':
			vehicle.vehicle_speed = 100;
			invalid = FALSE;
			break;
		case 'o':
			vehicle.vehicle_speed = 30;
			invalid = FALSE;
			break;
		case 'r':
			vehicle.vehicle_speed = 0;
			invalid = FALSE;
			break;
/*if user enters an invalid input keeps asking for input until
a valid input is entered
*/
		default:
			printf("Invalid Input\n");
			break;
		}
	}
	/*display current vehicle state*/
	check_vehicle_speed_30();
	display_vehicle_state();
}


/*wait for user to input room temperature data*/
void room_temperature_data(void){
	printf("Set the room Temperature : ");
	fflush( stdout );
	scanf(" %f", &vehicle.room_temp);
	if (vehicle.room_temp < 10 || vehicle.room_temp > 30){
		vehicle.ac = ON;
		vehicle.room_temp = 20;
	}
	else{
		vehicle.ac = OFF;
	}
	/*display current vehicle state*/
	check_vehicle_speed_30();
	display_vehicle_state();
}

#if WITH_ENGINE_TEMP_CONTROLLER == 1
void engine_temp_data(void){
	printf("Set the engine Temperature : ");
	fflush( stdout );
	scanf(" %f", &vehicle.engine_temp);
	if (vehicle.engine_temp < 10 || vehicle.engine_temp > 30){
		vehicle.engine_temp_state = ON;
		vehicle.engine_temp = 125;
	}
	else{
		vehicle.engine_temp_state = OFF;
	}
	/*display current vehicle state*/
	check_vehicle_speed_30();
	display_vehicle_state();
}
#endif


/*check if vehicle speed is 30 km/hr.*/
void check_vehicle_speed_30(void){
	if (vehicle.vehicle_speed == 30){
		vehicle.ac = ON;
		vehicle.room_temp = (vehicle.room_temp * 1.25 + 1);
#if WITH_ENGINE_TEMP_CONTROLLER == 1
		vehicle.engine_temp_state = ON;
		vehicle.engine_temp = (vehicle.engine_temp * 1.25 + 1);
#endif
	}
}

/*function to display current vehicle state*/
void display_vehicle_state(void){
	printf("\n");
	printf("Engine State : ");
	vehicle.engine_state ? printf("ON\n") : printf("OFF\n");
	printf("AC : ");
	vehicle.ac ? printf("ON\n") : printf("OFF\n");
	printf("Vehicle Speed : %d km\\hr.\n", vehicle.vehicle_speed);
	printf("Room Temperature : %.2f C\n", vehicle.room_temp);
#if WITH_ENGINE_TEMP_CONTROLLER == 1
	printf("Engine Temperature Controller State : ");
	vehicle.engine_temp_state ? printf("ON\n") : printf("OFF\n");
	printf("Engine Temperature : %.2f C\n", vehicle.engine_temp);
#endif
}




