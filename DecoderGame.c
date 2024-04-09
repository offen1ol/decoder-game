#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>
#include <stdbool.h>

//define pins
#define startPin 4 //GPIO 23 - Start Button

//buttons - input
#define redButton 0	   //GPIO 17
#define yellowButton 3 //GPIO 22
#define greenButton 23 //GPIO 13
#define blueButton 25  //GPIO 26

//LEDs - output
#define redLED 5	//GPIO 24
#define yellowLED 6 //GPIO 25
#define greenLED 26 //GPIO 12
#define blueLED 27	//GPIO 16

int main(void)
{
	//wiringPiSetup check
	if (wiringPiSetup() == -1)
	{
		puts("Raspberry Pi setup failed. Unable to execute the program.");
		return 1;
	}

	//Set Pin Modes
	pinMode(startPin, INPUT);

	//buttons
	pinMode(redButton, INPUT);
	pinMode(yellowButton, INPUT);
	pinMode(greenButton, INPUT);
	pinMode(blueButton, INPUT);

	//LEDs
	pinMode(redLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);
	pinMode(greenLED, OUTPUT);
	pinMode(blueLED, OUTPUT);

	puts("Password Decoder! Press the big button to start!");

	int password[] = {redButton, yellowButton, greenButton, blueButton};
	int leds[] = {redLED, yellowLED, greenLED, blueLED};

	int guessed = 0;
	int errors = 0;

	bool redGuessed = false;
	bool yellowGuessed = false;
	bool greenGuessed = false;
	bool blueGuessed = false;

	//makes LEDs blink for a certain number of times
	void blink(int n)
	{
		for (int i = 0; i < n; i++)
		{
			delay(500);

			digitalWrite(redLED, 0);
			digitalWrite(yellowLED, 0);
			digitalWrite(greenLED, 0);
			digitalWrite(blueLED, 0);
			delay(500);

			digitalWrite(redLED, 1);
			digitalWrite(yellowLED, 1);
			digitalWrite(greenLED, 1);
			digitalWrite(blueLED, 1);
			delay(500);
		}
	}

	//resets all variables and lights for a new game
	void newGame()
	{
		guessed = 0;
		errors = 0;

		redGuessed = false;
		yellowGuessed = false;
		greenGuessed = false;
		blueGuessed = false;

		digitalWrite(redLED, 0);
		digitalWrite(yellowLED, 0);
		digitalWrite(greenLED, 0);
		digitalWrite(blueLED, 0);
	}

	//resets all variables and lights after the user gets a guess wrong and counts how many time wrong
	void reset()
	{
		guessed = 0;

		errors++;
		printf("\nTotal errors: %d\n\n", errors);

		digitalWrite(redLED, 0);
		digitalWrite(yellowLED, 0);
		digitalWrite(greenLED, 0);
		digitalWrite(blueLED, 0);

		redGuessed = false;
		yellowGuessed = false;
		greenGuessed = false;
		blueGuessed = false;
	}

	while (1 == 1)
	{
		if (digitalRead(startPin) == HIGH)
		{
			blink(1);
			newGame();

			//generates a random combination for each new game and the LEDs array matches the combination too
			srand(time(NULL));

			for (int i = 0; i < 3; i++)
			{
				int j = (rand() % 3) + 1;

				int passwordTemp = password[i];
				password[i] = password[j];
				password[j] = passwordTemp;

				int ledsTemp = leds[i];
				leds[i] = leds[j];
				leds[j] = ledsTemp;
			}

			//printf("%d %d %d %d\n", password[0], password[1], password[2], password[3]); 	//The randomly generated answer
			//printf("%d, %d %d %d\n", leds[0], leds[1], leds[2], leds[3]); 				//should match with button pins^^

			puts("Start!\n");

			delay(200);

			while (guessed < 4)
			{
				//checks to see if redButton is pressed && redGuessed variable turns true so that the user can't press the button twice
				if (digitalRead(redButton) == HIGH && redGuessed == false)
				{
					//checks to see if guess is in the right order of the password
					if (redButton == password[guessed])
					{
						digitalWrite(leds[guessed], 1);
						guessed++;
						redGuessed = true;
						puts("Correct!");
					}
					else
					{
						//if wrong, calls the reset() to turn off all lights and turns guesses back to zero
						printf("Wrong! Red is not in Position %d\n", guessed + 1);
						reset();
					}
				}

				if (digitalRead(yellowButton) == HIGH && yellowGuessed == false)
				{

					if (yellowButton == password[guessed])
					{
						digitalWrite(leds[guessed], 1);
						guessed++;
						yellowGuessed = true;
						puts("Correct!");
					}
					else
					{
						printf("Wrong! Yellow is not in Position %d\n", guessed + 1);
						reset();
					}
				}

				if (digitalRead(greenButton) == HIGH && greenGuessed == false)
				{

					if (greenButton == password[guessed])
					{
						digitalWrite(leds[guessed], 1);
						guessed++;
						greenGuessed = true;
						puts("Correct!");
					}
					else
					{
						printf("Wrong! Green is not in Position %d\n", guessed + 1);
						reset();
					}
				}

				if (digitalRead(blueButton) == HIGH && blueGuessed == false)
				{

					if (blueButton == password[guessed])
					{
						digitalWrite(leds[guessed], 1);
						guessed++;
						blueGuessed = true;
						puts("Correct!");
					}
					else
					{
						printf("Wrong! Blue is not in Position %d\n", guessed + 1);
						reset();
					}
				}

				delay(200);
			}

			//After all 4 LEDs are lit up and 4 guesses are made, a rating is given depending on how many errors the player made
			puts("\nCongrats! You unlocked the password!");
			printf("Rating: ");
			if (errors >= 5)
			{
				puts("Poor\n");
				blink(1);
			}
			else if (errors > 3)
			{
				puts("Okay\n");
				blink(2);
			}
			else if (errors > 1)
			{
				puts("Good!\n");
				blink(3);
			}
			else
			{
				puts("Amazing!\n");
				blink(4);
			}

			puts("Play again?\n");
		}
	}

	return 0;
}
